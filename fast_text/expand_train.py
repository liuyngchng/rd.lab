#!/usr/bin/env python3
"""
fastText 训练数据扩充脚本 v2

改进：
  1. 从生产聊天记录提取真实用户消息作为 few-shot 示例
  2. 从快捷回复分类映射到意图类别
  3. LLM 生成时注入真实用户语言风格

用法:
  python3 expand_train.py              # 全部：提取+生成+训练
  python3 expand_train.py --dry-run    # 只生成数据不训练
  python3 expand_train.py --no-llm     # 跳过LLM，只用已有train_expanded.txt训练

环境变量:
  FT_DB_PATH         — workflow SQLite 数据库路径
  FT_MODEL_DIR        — 模型输出目录
  FT_WORKFLOW_ID      — 工作流 ID（默认 1）
  FT_CHAT_LOG_PATH    — 生产聊天记录 Excel 路径
  FT_LLM_BASE_URL     — LLM API 地址
  FT_LLM_API_KEY      — LLM API Key
  FT_LLM_MODEL        — LLM 模型名称
"""

import argparse
import json
import os
import random
import re
import sqlite3
import subprocess
import sys
import time
from collections import Counter

import pandas as pd
from openai import OpenAI

# ============================================================
# 配置（优先从环境变量读取）
# ============================================================

_here = os.path.dirname(os.path.abspath(__file__))

DB_PATH = os.environ.get("FT_DB_PATH", os.path.join(_here, "..", "workflow", "cfg.workflow"))
MODEL_DIR = os.environ.get("FT_MODEL_DIR", _here)
WORKFLOW_ID = int(os.environ.get("FT_WORKFLOW_ID", "1"))
CHAT_LOG_PATH = os.environ.get("FT_CHAT_LOG_PATH", "")

LLM_CONFIG = {
    "base_url": os.environ.get("FT_LLM_BASE_URL", ""),
    "api_key": os.environ.get("FT_LLM_API_KEY", ""),
    "model": os.environ.get("FT_LLM_MODEL", ""),
}

EXPAND_TRAIN_PATH = os.path.join(MODEL_DIR, "train_expanded.txt")

TRAIN_PARAMS = {
    "epoch": 200,
    "lr": 0.8,
    "wordNgrams": 3,
    "dim": 100,
    "minCount": 1,
}

QUANTIZE_PARAMS = {
    "qnorm": True,
    "retrain": True,
    "epoch": 25,
    "cutoff": 50000,
}

# 快捷回复类别 → 意图标签映射
QUICK_REPLY_INTENT_MAP = {
    "燃气泄漏": "emergency",
    "居民气价": "billing",
    "充值": "billing",
    "查余额、用气查询": "billing",
    "燃气费退费问题": "billing",
    "发票": "billing",
    "气代煤补贴": "billing",
    "开户": "business",
    "过户": "business",
    "报装   /    改管": "business",
    "预约通气": "business",
    "补卡": "business",
    "换表": "business",
    "打不着火": "repair",
    "报警器响": "repair",
    "气不上表": "repair",
    "安检、隐患": "repair",
    "燃气表走的快": "repair",
    "壁挂炉、热力问题": "repair",
    "安装壁挂炉/热水器": "repair",
    "营业厅地址": "faq",
    "热线电话": "faq",
    "网上营业厅": "faq",
    "查户号、表号": "faq",
    "停气、复气": "faq",
    "系统故障，白屏": "faq",
    "安抚、投诉、抱怨、建议": "faq",
    "然然服务视频链接": "faq",
    "相关规范参考": "faq",
    "安全用气常识": "faq",
    "系统维护": "faq",
}

# ============================================================
# 从生产数据中提取真实用户消息
# ============================================================

# 系统发送者：机器人、系统消息、以及含中文名的客服座席
# 真实用户通常是匿名 ID（纯字母数字）
def _is_system_sender(name: str) -> bool:
    """判断发送者是否系统/座席（非真实用户）。"""
    name = str(name).strip()
    if name in ("机器人", "系统"):
        return True
    # 含中文 → 座席姓名
    if re.search(r'[一-鿿]', name):
        return True
    return False

SKIP_PATTERNS = [
    "很高兴为您服务", "人工客服为您服务", "祝您生活愉快",
    "视频通话", "会话来源于", "Hi，您好", "您好亲",
    "不客气", "一声问候", "感谢您的咨询", "再见",
]


def load_real_user_messages(chat_log_path: str) -> list[str]:
    """从生产聊天记录中提取真实用户消息（去重清洗）。"""
    if not os.path.exists(chat_log_path):
        print(f"  警告: 聊天记录不存在: {chat_log_path}")
        return []

    df = pd.read_excel(chat_log_path)

    # 取真实用户的消息（排除座席、机器人、系统）
    df_user = df[~df["消息来源"].apply(_is_system_sender)]

    def is_valid(msg):
        msg = str(msg).strip()
        if len(msg) < 3 or len(msg) > 80:
            return False
        if re.match(r"^\d+$", msg):
            return False
        for p in SKIP_PATTERNS:
            if p in msg:
                return False
        # 排除纯菜单选项
        if msg in ["返回上层选项", "返回上层", "返回", "退出", "未解决", "已解决",
                    "好的", "可以", "可以了", "已打开", "********", "是的", "没有",
                    "在的亲", "好的亲", "谢谢", "你好", "您好"]:
            return False
        return True

    msgs = [str(m).strip() for m in df_user[df_user["聊天内容"].notna()]["聊天内容"] if is_valid(m)]

    # 去重，保留原始的多样性
    counter = Counter(msgs)
    # 高频消息多是快捷按钮，保留原始+低频长尾
    unique_msgs = list(counter.keys())
    print(f"  提取真实用户消息: {len(msgs)} 条原始 → {len(unique_msgs)} 条去重")
    return unique_msgs


def match_category_samples(all_msgs: list[str], keywords: list[str], n: int = 20) -> list[str]:
    """用关键词匹配，从真实消息中找出属于某类别的样本。"""
    matched = []
    for msg in all_msgs:
        for kw in keywords:
            if kw in msg:
                matched.append(msg)
                break
    # 去重后取 n 条
    seen = set()
    result = []
    for m in matched:
        if m not in seen:
            seen.add(m)
            result.append(m)
            if len(result) >= n:
                break
    return result


# ============================================================
# LLM 生成 Prompt（注入真实用户示例）
# ============================================================

GENERATE_PROMPT_V2 = """你是燃气公司客服培训师，需要为意图分类系统生成训练数据。

【意图类别】{category_name}
【类别定义】{description}
【参考关键词】{keywords}

【真实用户问法示例】（来自生产系统聊天记录，请模仿其语言风格）：
{real_examples}

请生成 {count} 条多样化的用户问法，要求：
1. **模仿真实用户**：语言风格贴近上面的真实示例 —— 简短、口语化、有错别字/方言、不完整句
2. **句式多样**：疑问句、陈述句、祈使句、抱怨语气都要有
3. **长度多样**：2-5字的短句占30%，6-20字的长句占70%
4. **语义不重复**：每条从不同角度切入，不要只是换一两个字
5. 每条一行，纯文本输出，不要编号、不要引号

直接输出："""


def generate_samples_v2(client: OpenAI, category: dict, real_examples: list[str], count: int) -> list[str]:
    """用 few-shot 方式生成训练样本。"""
    ex_text = "\n".join(f"  - {e}" for e in real_examples[:25]) if real_examples else "（无示例，请根据关键词自行发挥）"

    prompt = GENERATE_PROMPT_V2.format(
        category_name=category["name"],
        description=category.get("description", ""),
        keywords=", ".join(category.get("keywords", [])),
        real_examples=ex_text,
        count=count,
    )

    for attempt in range(3):
        try:
            resp = client.chat.completions.create(
                model=LLM_CONFIG["model"],
                messages=[
                    {"role": "system", "content": "你是燃气客服训练数据生成器。只输出用户问法文本，每行一条，不要编号、引号、任何额外内容。"},
                    {"role": "user", "content": prompt},
                ],
                temperature=0.95,
                max_tokens=4096,
            )
            lines = resp.choices[0].message.content.strip().split("\n")
            samples = []
            for line in lines:
                line = line.strip()
                for pfx in ["1. ", "1、", "1）", "1) ", "(1)", "1）", "- "]:
                    if line.startswith(pfx):
                        line = line[len(pfx):].strip()
                        break
                line = line.strip('"').strip("'").strip(""").strip(""").strip()
                if line and len(line) >= 2:
                    samples.append(line)
            return samples
        except Exception as e:
            print(f"  尝试 {attempt+1}/3 失败: {e}")
            time.sleep(2)
    return []


def generate_none_samples_v2(client: OpenAI, real_irrelevant: list[str], count: int) -> list[str]:
    """生成无关闲聊样本。"""
    ex_text = "\n".join(f"  - {e}" for e in real_irrelevant[:15]) if real_irrelevant else ""

    prompt = f"""请生成 {count} 条与燃气业务完全无关的用户消息。

真实无关消息示例（来自生产系统）：
{ex_text}

要求：
1. 必须覆盖以下各类场景（每类至少5条）：问候寒暄、天气、娱乐八卦、点餐/外卖、购物/快递、抱怨工作/生活、讲笑话/段子、问时间/日期、问路/导航、体检/看病、问AI能力（"帮我写诗""你会做什么"等）、编程/技术问题
2. 风格口语化，模仿真实用户打字习惯
3. 每条一行，纯文本输出，不要编号、不要引号"""

    try:
        resp = client.chat.completions.create(
            model=LLM_CONFIG["model"],
            messages=[
                {"role": "system", "content": "只输出文本，每行一条。"},
                {"role": "user", "content": prompt},
            ],
            temperature=0.95,
            max_tokens=4096,
        )
        lines = resp.choices[0].message.content.strip().split("\n")
        return [l.strip().strip('"').strip("'") for l in lines if l.strip() and len(l.strip()) >= 2]
    except Exception as e:
        print(f"  生成 none 失败: {e}")
        return []


# ============================================================
# 共有工具函数
# ============================================================

def tokenize(text: str) -> str:
    return " ".join(text)


def load_classifier(db_path: str, workflow_id: int) -> dict:
    db = sqlite3.connect(db_path)
    row = db.execute("SELECT name, classifier FROM workflow_def WHERE id=?", (workflow_id,)).fetchone()
    db.close()
    if row is None:
        print(f"错误: 工作流 id={workflow_id} 不存在")
        sys.exit(1)
    return json.loads(row[1])


# 手工标注的易混淆样本，覆盖跨类别歧义 case
HARD_CASES = {
    "billing": [
        "燃气费涨了", "燃气费太贵", "燃气费多少钱",
        "怎么交燃气费", "燃气费怎么算的", "我家燃气费是不是算错了",
        "燃气费什么时候交", "查燃气费", "燃气费用明细",
        "这个月燃气费扣了好多", "燃气费余额不足",
    ],
    "repair": [
        "充值不上表", "充了钱气表没反应", "缴费了气还是不通",
        "交了燃气费还是没气", "充值后表不显示", "充完钱打不着火",
        "燃气费交了灶还是打不着", "钱扣了气没到",
    ],
    "emergency": [
        "家里闻到怪味", "厨房有煤气味", "报警器滴滴响",
        "闻到燃气的味道", "好像漏气了怎么办", "天然气味道很重",
    ],
}


def write_train_data(train_path: str, classifier: dict, samples_per_cat: dict[str, list[str]]):
    categories = classifier.get("categories", [])
    stats = []

    with open(train_path, "w") as f:
        for cat in categories:
            name = cat["name"]
            keywords = cat.get("keywords", [])
            desc = cat.get("description", "")

            count = 0
            for kw in keywords:
                if kw.strip():
                    f.write(f"__label__{name} {tokenize(kw)}\n")
                    count += 1
            if desc.strip():
                f.write(f"__label__{name} {tokenize(desc)}\n")
                count += 1
            expanded = samples_per_cat.get(name, [])
            for s in expanded:
                if s.strip():
                    f.write(f"__label__{name} {tokenize(s)}\n")
                    count += 1
            # 手工标注的易混淆样本
            hard_samples = HARD_CASES.get(name, [])
            for s in hard_samples:
                if s.strip():
                    f.write(f"__label__{name} {tokenize(s)}\n")
                    count += 1

            kw_count = len(keywords) + (1 if desc.strip() else 0)
            hard_count = len(hard_samples)
            stats.append({"name": name, "original": kw_count, "expanded": len(expanded), "hard": hard_count, "total": count})
            print(f"  {name}: 原始 {kw_count} + 扩展 {len(expanded)} + 难例 {hard_count} = 合计 {count} 条")

        none_samples = samples_per_cat.get("none", [])
        for s in none_samples:
            if s.strip():
                f.write(f"__label__none {tokenize(s)}\n")
        total = sum(s["total"] for s in stats)
        total_hard = sum(s.get("hard", 0) for s in stats)
        print(f"  none: {len(none_samples)} 条")
        print(f"  难例: {total_hard} 条（手工标注）")
        print(f"  总计: {total + len(none_samples)} 条训练样本")

    return stats


def train_model(train_path: str, model_dir: str) -> str:
    os.makedirs(model_dir, exist_ok=True)
    model_path = os.path.join(model_dir, "model.ftz")
    output_prefix = model_path.replace(".ftz", "")

    train_cmd = ["fasttext", "supervised", "-input", train_path, "-output", output_prefix]
    for k, v in TRAIN_PARAMS.items():
        train_cmd.extend([f"-{k}", str(v)])
    print(f"\n训练: {' '.join(train_cmd)}")
    subprocess.run(train_cmd, check=True)

    quant_cmd = ["fasttext", "quantize", "-input", train_path, "-output", output_prefix]
    for k, v in QUANTIZE_PARAMS.items():
        if isinstance(v, bool):
            if v:
                quant_cmd.append(f"-{k}")
        else:
            quant_cmd.extend([f"-{k}", str(v)])
    print(f"量化: {' '.join(quant_cmd)}")
    subprocess.run(quant_cmd, check=True)

    if not os.path.exists(model_path):
        print(f"错误: 未生成 {model_path}")
        sys.exit(1)

    size_kb = os.path.getsize(model_path) / 1024
    print(f"模型: {model_path} ({size_kb:.0f} KB)")

    for ext in [".bin", ".vec"]:
        fp = output_prefix + ext
        if os.path.exists(fp):
            os.remove(fp)
    return model_path


def test_model(model_path: str):
    tests = [
        # emergency
        ("家里闻到煤气味怎么办", "emergency"),
        ("闻到刺鼻气味", "emergency"),
        ("厨房有异味是不是漏气了", "emergency"),
        ("燃气报警器一直响", "emergency"),
        ("好像有煤气泄漏", "emergency"),
        ("着火了我该怎么办", "emergency"),
        ("天然气泄漏了", "emergency"),
        # billing
        ("账单怎么查", "billing"),
        ("燃气费涨了", "billing"),
        ("这个月扣了多少钱", "billing"),
        ("在哪可以缴费", "billing"),
        ("帮我查一下欠费", "billing"),
        ("燃气费太贵了", "billing"),
        ("怎么充值", "billing"),
        ("余额多少", "billing"),
        # business
        ("怎么开通燃气", "business"),
        ("我要开户需要什么材料", "business"),
        ("搬家了燃气怎么过户", "business"),
        ("想装天然气", "business"),
        ("二手房燃气改名字", "business"),
        ("怎么办理报装", "business"),
        # repair
        ("燃气灶打不着火", "repair"),
        ("热水器坏了", "repair"),
        ("灶台点火没反应", "repair"),
        ("燃气灶火是红色的", "repair"),
        ("热水器自动熄火", "repair"),
        ("燃气灶火苗太小", "repair"),
        ("打不燃火了", "repair"),
        ("充值不上表", "repair"),
        ("阀门自动关闭三次了", "repair"),
        # faq
        ("客服电话多少", "faq"),
        ("我要投诉", "faq"),
        ("你们几点下班", "faq"),
        ("营业厅在什么地方", "faq"),
        ("投诉电话是多少", "faq"),
        ("怎么转人工", "faq"),
        ("转人工", "faq"),
        # none
        ("今天天气不错", "none"),
        ("帮我写首诗", "none"),
        ("附近有什么好吃的", "none"),
        ("讲个笑话", "none"),
        ("今天星期几", "none"),
    ]

    print(f"\n{'='*70}")
    print("预测测试")
    print(f"{'='*70}")
    print(f"{'Query':<30s} {'预测':<15s} {'期望':<15s} {'置信度':>8s}  {'判定':<6s}")
    print(f"{'-'*30} {'-'*15} {'-'*15} {'-'*8}  {'-'*6}")

    correct = 0
    fallthrough = 0
    wrong = 0
    for q, expected in tests:
        tokens = tokenize(q)
        r = subprocess.run(
            ["fasttext", "predict-prob", model_path, "-", "1"],
            input=tokens + "\n", capture_output=True, text=True,
        )
        parts = r.stdout.strip().split()
        label = parts[0].replace("__label__", "") if parts else "?"
        conf = float(parts[1]) if len(parts) > 1 else 0

        if label == expected and conf >= 0.5:
            verdict = "✓"
            correct += 1
        elif conf < 0.5:
            verdict = "↓下层"
            fallthrough += 1
        else:
            verdict = "✗"
            wrong += 1

        print(f"{q:<30s} {label:<15s} {expected:<15s} {conf:>7.4f}  {verdict:<6s}")

    total = len(tests)
    print(f"\n准确命中: {correct}/{total}, 低置信度 fallthrough: {fallthrough}/{total}, 错误: {wrong}/{total}")
    if total > fallthrough:
        print(f"有效准确率（排除 fallthrough）: {correct + wrong}/{total - fallthrough} → {correct/(total-fallthrough)*100:.0f}% 命中")


# ============================================================
# 主流程
# ============================================================

def main():
    parser = argparse.ArgumentParser(description="扩充 fastText 训练数据 v2")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--samples", type=int, default=80)
    parser.add_argument("--no-llm", action="store_true")
    args = parser.parse_args()

    print("=" * 60)
    print("fastText 训练数据扩充 v2（真实用户数据增强）")
    print("=" * 60)
    print(f"数据库:    {DB_PATH}")
    print(f"输出目录:  {MODEL_DIR}")
    if not args.no_llm:
        if not LLM_CONFIG["base_url"] or not LLM_CONFIG["api_key"]:
            print("错误: 请设置环境变量 FT_LLM_BASE_URL 和 FT_LLM_API_KEY")
            print("      或使用 --no-llm 跳过 LLM 生成")
            sys.exit(1)
        print(f"LLM:       {LLM_CONFIG['model']} @ {LLM_CONFIG['base_url']}")
    if CHAT_LOG_PATH:
        print(f"聊天记录:  {CHAT_LOG_PATH}")
    else:
        print("聊天记录:  未设置 FT_CHAT_LOG_PATH，将不使用真实用户示例")

    # 1. 加载分类器
    print("\n[1/6] 加载分类器配置...")
    classifier = load_classifier(DB_PATH, WORKFLOW_ID)
    categories = classifier.get("categories", [])
    print(f"  类别数: {len(categories)}")
    for c in categories:
        print(f"    {c['name']}: {len(c.get('keywords',[]))} 个关键词")

    # 2. 提取真实用户消息
    print("\n[2/6] 提取生产聊天记录中真实用户消息...")
    all_real_msgs = load_real_user_messages(CHAT_LOG_PATH)

    # 3. 为每个类别匹配真实用户示例
    samples_per_cat = {}
    if not args.no_llm:
        print(f"\n[3/6] 匹配真实用户示例 + LLM 生成 (每类 {args.samples} 条)...")
        client = OpenAI(base_url=LLM_CONFIG["base_url"], api_key=LLM_CONFIG["api_key"])

        for cat in categories:
            name = cat["name"]
            keywords = cat.get("keywords", [])

            # 从真实消息中匹配
            real_examples = match_category_samples(all_real_msgs, keywords, n=25)
            print(f"  {name}: 匹配到 {len(real_examples)} 条真实示例")

            # 用 few-shot 生成
            print(f"    生成中...", end=" ", flush=True)
            generated = generate_samples_v2(client, cat, real_examples, args.samples)
            samples_per_cat[name] = generated
            print(f"{len(generated)} 条")

        # none 类别
        print(f"  none: 生成中...", end=" ", flush=True)
        none_real = random.sample(all_real_msgs, min(30, len(all_real_msgs)))
        none_samples = generate_none_samples_v2(client, none_real, 80)
        samples_per_cat["none"] = none_samples
        print(f"{len(none_samples)} 条")

    else:
        print("\n[3/6] 跳过 LLM 生成 (--no-llm)")

    # 4. 写入训练数据
    print(f"\n[4/6] 写入训练数据...")
    write_train_data(EXPAND_TRAIN_PATH, classifier, samples_per_cat)

    if args.dry_run:
        print(f"\n--dry-run: 训练数据已写入 {EXPAND_TRAIN_PATH}，跳过训练")
        return

    # 5. 训练 + 量化
    print("\n[5/6] 训练 + 量化模型...")
    model_path = train_model(EXPAND_TRAIN_PATH, MODEL_DIR)

    # 6. 测试
    print("\n[6/6] 测试模型...")
    test_model(model_path)

    print(f"\n{'='*60}")
    print("完成！")
    print(f"  训练数据: {EXPAND_TRAIN_PATH}")
    print(f"  模型文件: {model_path}")
    print(f"{'='*60}")


if __name__ == "__main__":
    main()
