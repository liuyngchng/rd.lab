# fastText 意图分类模型

## 概述

用于 kb-chat-flow 工作流引擎的意图分类快速通道。

分类流程（4 层）：
```
用户输入 → 关键词匹配(0ms) → fastText本地推理(~5ms) → Embedding语义(~100ms) → LLM兜底(~1s)
```

## 文件说明

| 文件 | 用途 |
|------|------|
| `install.sh` | 安装 fastText CLI 及依赖 |
| `train.py` | 从数据库读取分类器配置 → 生成训练数据 → 训练模型 → 测试 |
| `train.txt` | 训练过程自动生成的中间文件（方便审查） |

## 使用方法

### 1. 安装环境（一次性）

```bash
bash install.sh
```

### 2. 训练模型

```bash
source ~/workspace/llm_py_env/bin/activate
python3 train.py
```

训练脚本会：
1. 从 `../workflow/cfg.workflow` 读取工作流的 Classifier 配置
2. 用关键词+描述自动生成训练数据（按字切分，无需分词）
3. 调用 fastText 训练模型
4. 输出到 `../../kb-chat-flow/g/dt/ft/model.bin`

### 3. 输出文件

```
kb-chat-flow/g/dt/ft/
├── model.bin    # 训练好的模型（已提交到 git）
└── train.txt    # 训练数据（供审查，可删除）
```

## 训练数据规则

与 Go 代码 `internal/fasttext/predictor.go` 的 `generateTrainData()` 完全一致：

- 每个类别的**关键词**各一条
- 每个类别的**描述**各一条
- 中文**按字切分**（空格分隔字符），fastText 通过 character-level n-gram 学习

## 模型更新

当后台修改了分类器配置（增删类别、修改关键词），有两种方式更新模型：

### 方式一：运行时自动重训
Go 代码 `Predictor.Train()` 会在每次请求时检测类别 hash，自动触发重训。

### 方式二：手动重新训练
```bash
python3 train.py
git add ../../kb-chat-flow/g/dt/ft/model.bin
git commit -m "update fastText model"
```

## 训练参数

| 参数 | 值 | 说明 |
|------|-----|------|
| epoch | 200 | 训练轮数 |
| lr | 0.8 | 学习率 |
| wordNgrams | 3 | 字符 n-gram（中文按字切分后等价） |
| dim | 100 | 词向量维度 |
| minCount | 1 | 最小词频 |

模型文件通常只有几十 KB，非常轻量。
