#!/usr/bin/env python3
"""
fastText 意图分类模型训练脚本

从 SQLite 数据库中读取工作流的 Classifier 配置，
自动生成训练数据并训练 fastText 模型。

训练数据生成规则（与 Go 代码 internal/fasttext/predictor.go 一致）：
  - 每个类别的关键词 → 训练样本
  - 每个类别的描述 → 训练样本
  - none 类别样本 → 教模型拒绝不相关的输入
  - 按字切分（空格分隔每个字符），无需分词器

用法:
  python3 train.py

输入:
  /home/rd/workspace/rd.lab/workflow/cfg.workflow  — SQLite 数据库，workflow_def 表

输出:
  /home/rd/workspace/kb-chat-flow/g/dt/ft/model.ftz  — 训练+量化后的模型
  /home/rd/workspace/kb-chat-flow/g/dt/ft/train.txt   — 训练数据（供审查）
"""

import json
import os
import sqlite3
import subprocess
import sys

# ============================================================
# 配置
# ============================================================

DB_PATH = "/home/rd/workspace/rd.lab/workflow/cfg.workflow"
WORKFLOW_ID = 1  # 使用哪个工作流的分类器配置

# 模型输出目录（对应 Go 项目 internal/fasttext/predictor.go 中的 defaultWorkDir）
MODEL_DIR = "/home/rd/workspace/kb-chat-flow/g/dt/ft"

# fastText 训练参数（与 Go 代码 trainModel() 保持一致）
TRAIN_PARAMS = {
    "epoch": 200,
    "lr": 0.8,
    "wordNgrams": 3,
    "dim": 50,
    "minCount": 1,
}

QUANTIZE_PARAMS = {
    "qnorm": True,
    "retrain": True,
    "epoch": 25,
    "cutoff": 50000,
}

# none 类别样本：教模型拒绝与燃气业务无关的输入
# （与 Go 代码 noneSamples 保持一致）
NONE_SAMPLES = [
    "今天天气真好", "明天会下雨吗", "附近有什么好吃的",
    "帮我写首诗", "讲个笑话", "几点了",
    "你是谁", "你会做什么", "你好啊",
    "播放音乐", "设置闹钟", "帮我查快递",
    "翻译一下", "什么是人工智能", "怎么做红烧肉",
    "股票涨了", "最近有什么电影",
]


# ============================================================
# 工具函数
# ============================================================


def tokenize(text: str) -> str:
    """按字切分中文文本，空格分隔每个字符。"""
    return " ".join(text)


def load_classifier(db_path: str, workflow_id: int) -> dict:
    """从数据库加载分类器配置。"""
    if not os.path.exists(db_path):
        print(f"错误: 数据库不存在: {db_path}")
        sys.exit(1)

    db = sqlite3.connect(db_path)
    row = db.execute(
        "SELECT name, classifier FROM workflow_def WHERE id=?", (workflow_id,)
    ).fetchone()
    db.close()

    if row is None:
        print(f"错误: 工作流 id={workflow_id} 不存在")
        sys.exit(1)

    workflow_name, classifier_json = row
    classifier = json.loads(classifier_json)
    print(f"工作流: {workflow_name}")
    print(f"类别数: {len(classifier.get('categories', []))}")
    return classifier


def generate_train_data(classifier: dict, train_path: str) -> list[dict]:
    """从分类器配置生成训练数据，返回类别统计。"""
    categories = classifier.get("categories", [])
    if not categories:
        print("错误: 分类器没有配置类别")
        sys.exit(1)

    stats = []
    with open(train_path, "w") as f:
        for cat in categories:
            name = cat["name"]
            desc = cat.get("description", "")
            keywords = cat.get("keywords", [])

            count = 0
            for kw in keywords:
                if kw.strip():
                    f.write(f"__label__{name} {tokenize(kw)}\n")
                    count += 1
            if desc.strip():
                f.write(f"__label__{name} {tokenize(desc)}\n")
                count += 1

            stats.append({"name": name, "desc": desc, "keywords": len(keywords), "samples": count})
            print(f"  {name}: {count} 条样本 ({len(keywords)} 个关键词)")

        # none 类别：不相关输入
        for s in NONE_SAMPLES:
            f.write(f"__label__none {tokenize(s)}\n")

    total = sum(s["samples"] for s in stats)
    print(f"  none: {len(NONE_SAMPLES)} 条样本")
    print(f"  合计: {total + len(NONE_SAMPLES)} 条训练样本")
    return stats


def train_model(train_path: str, model_dir: str) -> str:
    """训练 + 量化 fastText 模型，返回 .ftz 模型路径。"""
    os.makedirs(model_dir, exist_ok=True)
    model_path = os.path.join(model_dir, "model.ftz")
    output_prefix = model_path.replace(".ftz", "")

    # 第一步：训练
    train_cmd = ["fasttext", "supervised", "-input", train_path, "-output", output_prefix]
    for k, v in TRAIN_PARAMS.items():
        train_cmd.extend([f"-{k}", str(v)])

    print(f"\n训练: {' '.join(train_cmd)}")
    subprocess.run(train_cmd, check=True)

    # 第二步：量化压缩
    quant_cmd = ["fasttext", "quantize", "-input", train_path, "-output", output_prefix]
    for k, v in QUANTIZE_PARAMS.items():
        if isinstance(v, bool):
            if v:
                quant_cmd.append(f"-{k}")
        else:
            quant_cmd.extend([f"-{k}", str(v)])

    print(f"量化: {' '.join(quant_cmd)}")
    subprocess.run(quant_cmd, check=True)

    # 确认 .ftz 存在
    if not os.path.exists(model_path):
        print(f"错误: 量化模型未生成: {model_path}")
        sys.exit(1)

    size_kb = os.path.getsize(model_path) / 1024
    print(f"模型: {model_path} ({size_kb:.0f} KB)")

    # 清理未量化的中间文件
    for ext in [".bin", ".vec"]:
        fp = output_prefix + ext
        if os.path.exists(fp):
            os.remove(fp)

    return model_path


def test_model(model_path: str):
    """用典型 query 测试模型效果。"""
    tests = [
        "家里闻到煤气味怎么办",
        "账单怎么查",
        "怎么开通燃气",
        "燃气灶打不着火",
        "客服电话多少",
        "今天天气不错",
        "漏气很紧急",
        "燃气费涨了",
        "热水器坏了",
        "投诉怎么投诉",
        "爆炸了怎么办",
        "在哪缴费",
        "我要搬家要过户",
        "灶具点火没反应",
        "你们几点下班",
        "有没有危险",
        "怎么充值",
        "想安装天然气",
    ]

    print(f"\n{'='*60}")
    print("预测测试")
    print(f"{'='*60}")
    print(f"{'Query':<30s} {'预测':<15s} {'置信度':>8s}")
    print(f"{'-'*30} {'-'*15} {'-'*8}")

    ok = 0
    total_valid = 0
    for q in tests:
        tokens = tokenize(q)
        r = subprocess.run(
            ["fasttext", "predict-prob", model_path, "-", "1"],
            input=tokens + "\n", capture_output=True, text=True,
        )
        parts = r.stdout.strip().split()
        label = parts[0].replace("__label__", "") if parts else "?"
        conf = float(parts[1]) if len(parts) > 1 else 0

        flag = ""
        if label == "none":
            flag = " ✓" if q in NONE_SAMPLES else " →下层"
        elif conf < 0.5:
            flag = " ⚠低"
        else:
            if q not in NONE_SAMPLES:
                ok += 1
            total_valid += 1

        print(f"{q:<30s} {label:<15s} {conf:>7.4f}{flag}")

    print(f"\n直接命中: {ok} 条，低置信度/无关的 fallthrough 到下层")


# ============================================================
# 主流程
# ============================================================


def main():
    print("=" * 60)
    print("fastText 意图分类模型训练")
    print("=" * 60)

    # 1. 加载分类器配置
    print("\n[1/4] 加载分类器配置...")
    classifier = load_classifier(DB_PATH, WORKFLOW_ID)

    # 2. 生成训练数据
    print("\n[2/4] 生成训练数据...")
    train_path = os.path.join(MODEL_DIR, "train.txt")
    generate_train_data(classifier, train_path)

    # 3. 训练 + 量化
    print("\n[3/4] 训练 + 量化模型...")
    model_path = train_model(train_path, MODEL_DIR)

    # 4. 测试
    print("\n[4/4] 测试模型...")
    test_model(model_path)

    print(f"\n{'='*60}")
    print("完成！")
    print(f"  训练数据: {train_path}")
    print(f"  模型文件: {model_path}")
    print(f"  类别变化时 Go 代码会自动重训")
    print(f"{'='*60}")


if __name__ == "__main__":
    main()
