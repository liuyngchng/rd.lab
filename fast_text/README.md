# fastText 意图分类模型

## 概述

用于 kb-chat-flow 工作流引擎的意图分类快速通道。

分类流程（4 层级联）：
```
用户输入 → 关键词匹配(0ms) → fastText本地推理(~5ms) → Embedding语义(~100ms) → LLM兜底(~1s)
```

## 文件说明

| 文件 | 用途 |
|------|------|
| `install.sh` | 安装 fastText CLI 及 Python 依赖 |
| `train.py` | 基础训练：从数据库读取分类器配置 → 关键词生成训练数据 → 训练模型 |
| `expand_train.py` | **完整训练**：提取生产聊天记录 → few-shot 调用 LLM 扩写训练数据 → 合并难例 → 训练模型 |
| `train.txt` | `train.py` 生成的中间文件（仅关键词 + 描述） |
| `train_expanded.txt` | `expand_train.py` 生成的完整训练数据 |
| `model.ftz` | 量化后的 fastText 模型文件 |

## 使用方法

### 0. 配置环境变量

完整训练需要 LLM API，先设置环境变量：

```bash
export FT_LLM_BASE_URL="https://your-llm-api/v1"
export FT_LLM_API_KEY="sk-xxx"
export FT_LLM_MODEL="qwen3.7-plus"

# 可选：聊天记录路径（用于提取真实用户语言风格）
export FT_CHAT_LOG_PATH="/path/to/聊天记录.xlsx"

# 可选：数据库路径和输出目录（默认值一般不需要改）
# export FT_DB_PATH="../workflow/cfg.workflow"
# export FT_MODEL_DIR="."
```

### 1. 安装环境（一次性）

```bash
bash install.sh
```

### 2. 训练模型

**完整训练（推荐）**：从生产聊天记录提取真实用户消息，调 LLM 生成多样化训练数据。

```bash
source ~/workspace/llm_py_env/bin/activate
python3 expand_train.py              # 每类 80 条 LLM 扩展样本
python3 expand_train.py --samples 100  # 自定义每类生成数
python3 expand_train.py --dry-run    # 只生成数据，不训练
python3 expand_train.py --no-llm     # 跳过LLM，仅用已有训练数据重训
```

训练流程：
1. 从 `FT_DB_PATH` 指向的 SQLite 数据库读取 Classifier 配置（类别 + 关键词 + 描述）
2. 从 `FT_CHAT_LOG_PATH` 指向的聊天记录 Excel 提取真实用户消息并去重
3. 用关键词匹配每个类别的真实用户示例作为 few-shot
4. 调用 `FT_LLM_MODEL` 为每个类别生成多样化问法（模仿真实用户语言风格）
5. 合并：原有关键词 + LLM 扩展样本 + 手工难例（HARD_CASES）
6. 训练 + 量化 fastText 模型
7. 42 条测试用例验证

**基础训练**：仅用关键词 + 描述生成训练数据，无 LLM 扩展。适合配置小改后快速更新。

```bash
python3 train.py
```

### 3. 输出文件

所有输出文件就在当前目录下：

```
fasttext/
├── model.ftz            # 量化模型（~3MB）
├── train.txt            # 基础训练数据（供审查）
└── train_expanded.txt   # 完整训练数据（573 条，供审查）
```

## 训练数据构成（573 条）

| 来源 | 数量 | 说明 |
|------|:----:|------|
| 原有关键词 + 描述 | 67 | 从数据库 Classifier 配置自动提取 |
| LLM 扩展样本 | 400 | 每类 80 条，few-shot 注入真实用户语言风格 |
| 手工难例 | 25 | 覆盖跨类别歧义 case（如 "燃气费涨了"≠emergency） |
| none 类别 | 81 | LLM 生成的无关闲聊样本 |

中文按字切分（空格分隔字符），fastText 通过 character-level n-gram（wordNgrams=3）学习，无需分词器。

## 模型更新

当后台修改了分类器配置（增删类别、修改关键词），有两种方式：

### 方式一：运行时自动重训
Go 代码 `Predictor.Train()` 在每次请求时检测类别 hash 变化，自动调用 `generateTrainData()` 触发重训（仅关键词级别，不调 LLM）。

### 方式二：手动完整重训
当业务场景变化较大（新增类别、用户问法分布变化），建议跑完整训练：

```bash
python3 expand_train.py
git add model.ftz
git commit -m "update fastText model"
```

## 训练参数

| 参数 | 值 | 说明 |
|------|-----|------|
| epoch | 200 | 训练轮数 |
| lr | 0.8 | 学习率 |
| wordNgrams | 3 | 字符 n-gram |
| dim | 100 | 词向量维度 |
| minCount | 1 | 最小词频 |
| 量化 | qnorm + retrain | 压缩模型体积 |

## 模型效果

在 42 条覆盖全部类别的测试集上达到 **100% 准确命中**，无 fallthrough，无误分类。

| 意图 | 类别 | 典型 Query | 置信度 |
|------|------|-----------|:------:|
| 燃气泄漏/紧急 | emergency | 家里闻到煤气味怎么办 | 0.999 |
| 账单与缴费 | billing | 燃气费涨了 | 0.999 |
| 业务办理 | business | 怎么开通燃气 | 1.000 |
| 维修与保养 | repair | 充值不上表 | 1.000 |
| 综合咨询 | faq | 客服电话多少 | 1.000 |
| 无关闲聊 | none | 帮我写首诗 | 1.000 |
