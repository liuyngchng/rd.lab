# ollama

首先拉取 ollama， ollama 相当于大模型界的 docker, 用于运行、管理大模型，详见 https://ollama.com/download。

可以进行离线下载， 下载地址详见  https://github.com/ollama/ollama/releases/,  amd64版本的安装包链接

https://github.com/ollama/ollama/releases/download/v0.5.1/ollama-linux-amd64.tgz

# 启动服务

下载了 ollama-linux-amd64.tgz 之后，执行

```sh
cp ollama-linux-amd64.tgz /usr/local
cd /usr/local
sudo tar -zxf ollama-linux-amd64.tgz
# 相应文件解压至 /usr/local/bin 和 /usr/local/lib 两个文件夹中
# 启动服务
nohup ollama serve > ollama.log 2>&1 &
```

此时服务已经运行了， 可通过 查看 http://127.0.0.1:11434/ ，显示 “Ollama is running”

# Config

ollama 修改模型存储路径，可以通过修改环境变量 OLLAMA_MODELS 来实现

```sh
sudo vi /etc/profile
export OLLAMA_MODELS=/data/.ollama
```

检查配置是否生效

```sh
source /etc/profile
echo $OLLAMA_MODELS
# kill 已启动的服务，重启ollama读取环境变量
ollama serve &
```

其他配置选项如下所示

```sh
OLLAMA_DEBUG：					是否开启调试模式，默认为 false。
OLLAMA_FLASH_ATTENTION：			是否闪烁注意力，默认为 true。
OLLAMA_HOST：					Ollama 服务器的主机地址，默认为空。
OLLAMA_KEEP_ALIVE：				保持连接的时间，默认为 5m。
OLLAMA_LLM_LIBRARY：				LLM 库，默认为空。
OLLAMA_MAX_LOADED_MODELS：		最大加载模型数，默认为 1。
OLLAMA_MAX_QUEUE：				最大队列数，默认为空。
OLLAMA_MAX_VRAM：				最大虚拟内存，默认为空。
OLLAMA_MODELS：					模型目录，默认为空。
OLLAMA_NOHISTORY：				是否保存历史记录，默认为 false。
OLLAMA_NOPRUNE：					是否启用剪枝，默认为 false。
OLLAMA_NUM_PARALLEL：			并行数，默认为 1。
OLLAMA_ORIGINS：					允许的来源，默认为空。
OLLAMA_RUNNERS_DIR：				运行器目录，默认为空。
OLLAMA_SCHED_SPREAD：			调度分布，默认为空。
OLLAMA_TMPDIR：					临时文件目录，默认为空。Here is the optimized list in the desired format:
OLLAMA_DEBUG：					是否开启调试模式，默认为 false。
OLLAMA_FLASH_ATTENTION：			是否闪烁注意力，默认为 true。
OLLAMA_HOST：					Ollama 服务器的主机地址，默认为空。
OLLAMA_KEEP_ALIVE：				保持连接的时间，默认为 5m。
OLLAMA_LLM_LIBRARY：				LLM 库，默认为空。
OLLAMA_MAX_LOADED_MODELS：		最大加载模型数，默认为 1。
OLLAMA_MAX_QUEUE：				最大队列数，默认为空。
OLLAMA_MAX_VRAM：				最大虚拟内存，默认为空。
OLLAMA_MODELS：					模型目录，默认为空。
OLLAMA_NOHISTORY：				是否保存历史记录，默认为 false。
OLLAMA_NOPRUNE：					是否启用剪枝，默认为 false。
OLLAMA_NUM_PARALLEL：			并行数，默认为 1。
OLLAMA_ORIGINS：					允许的来源，默认为空。
OLLAMA_RUNNERS_DIR：				运行器目录，默认为空。
OLLAMA_SCHED_SPREAD：			调度分布，默认为空。
OLLAMA_TMPDIR：					临时文件目录，默认为空。
```



#  下载模型镜像

## 模型清单

各个模型的参数大小以及文件大小详见  https://github.com/ollama/ollama#model-library。

| Model              | Parameters | Size  | Download & Run                   |
| ------------------ | ---------- | ----- | -------------------------------- |
| Llama 3.3          | 70B        | 43GB  | `ollama run llama3.3`            |
| Llama 3.2          | 3B         | 2.0GB | `ollama run llama3.2`            |
| Llama 3.2          | 1B         | 1.3GB | `ollama run llama3.2:1b`         |
| Llama 3.2 Vision   | 11B        | 7.9GB | `ollama run llama3.2-vision`     |
| Llama 3.2 Vision   | 90B        | 55GB  | `ollama run llama3.2-vision:90b` |
| Llama 3.1          | 8B         | 4.7GB | `ollama run llama3.1`            |
| Llama 3.1          | 405B       | 231GB | `ollama run llama3.1:405b`       |
| Phi 4              | 14B        | 9.1GB | `ollama run phi4`                |
| Phi 3 Mini         | 3.8B       | 2.3GB | `ollama run phi3`                |
| Gemma 2            | 2B         | 1.6GB | `ollama run gemma2:2b`           |
| Gemma 2            | 9B         | 5.5GB | `ollama run gemma2`              |
| Gemma 2            | 27B        | 16GB  | `ollama run gemma2:27b`          |
| Mistral            | 7B         | 4.1GB | `ollama run mistral`             |
| Moondream 2        | 1.4B       | 829MB | `ollama run moondream`           |
| Neural Chat        | 7B         | 4.1GB | `ollama run neural-chat`         |
| Starling           | 7B         | 4.1GB | `ollama run starling-lm`         |
| Code Llama         | 7B         | 3.8GB | `ollama run codellama`           |
| Llama 2 Uncensored | 7B         | 3.8GB | `ollama run llama2-uncensored`   |
| LLaVA              | 7B         | 4.5GB | `ollama run llava`               |
| Solar              | 10.7B      | 6.1GB | `ollama run solar`               |

## 拉取并运行模型

详见 https://ollama.com/library/llama3:8b

```sh
# GPU 有 24G 显存 可运行 llama3:8b
# 拉取大模型镜像
ollama pull llama3.1:8b
# 运行大模型
ollama run llama3.1:8b

# 需要 1.3GB 的磁盘空间
ollama pull llama3.2:1b
```

Ollama 启动的默认地址为http://127.0.0.1:11434，通过设置环境变量 OLLAMA_HOST来修改默认监听地址和端口，这往往对我们需要远程调用API时非常有用。同时，如果要在open-webui等UI面板中调用 API ，需要为 Ollama 进行跨域配置后方可正常使用。一共有3个环境变量

```sh
# 设置监听地址及端口
OLLAMA_HOST 127.0.0.1:11434
# 设置跨域， * 表示支持所有域， 也可设置成 www.googe.com 表示只支持来自此域的访问请求
OLLAMA_ORIGINS *
# 设置模型存储地址
OLLAMA_MODELS /data/ollama
```

测试API 调用

```sh
curl http://127.0.0.1:11434/api/generate -d '{
	"model": "llama3.1:8b",
	"prompt": "你好啊"
}
```



# WebUI

如果觉得通过命令行的方式使用起来不太友好，可以通过WebUI来调用相关的 API ，进行用户请求的调用以及结果的展示

```sh
docker run -d -p 3000:8080 --add-host=host.docker.internal:host-gateway -v open-webui:/app/backend/data --name open-webui --restart always ghcr.io/open-webui/open-webui:main
```

# cmd

```sh
## 启动Ollama服务
ollama serve
## 从模型文件创建模型
ollama create
## 显示模型信息
ollama show
## 运行模型
ollama run model_name
## 从注册表中拉取模型
ollama pull model_name
## 将模型推送到注册表
ollama push
## 列出模型
ollama list
## 复制模型
ollama cp
## 删除模型
ollama rm model_name
## 获取有关Ollama任何命令的帮助信息
ollama help
```

# Modelfile

Modelfile的作用类似于 Dockerfile，完整的 modelfile 如下所示。

```dockerfile
# Modelfile generated by "ollama show"
# To build a new Modelfile based on this one, replace the FROM line with:
# FROM llama2:latest

FROM /usr/share/ollama/.ollama/models/blobs/sha256:8934d96d3f08982e95922b2b7a2c626a1fe873d7c3b06e8e56d7bc0a1fef9246
TEMPLATE """[INST] <>{{ .System }}<>

{{ .Prompt }} [/INST]
"""
PARAMETER temperature 1.0
PARAMETER top_k 100
PARAMETER top_p 1.0
PARAMETER stop "[INST]"
PARAMETER stop "[/INST]"
PARAMETER stop "<>"
PARAMETER stop "<>"
SYSTEM """
You are a helpful assistant that answers all questions asked of it in 
the style of a 1930's mafia mobster
"""
```

（1）**FROM**。FROM 指令是定义在创建时使用的基础模型的指令，是强制性的，是 Modelfile 中必须要有的部分。

（2）**TEMPLATE**。`TEMPLATE` 指令描述了要传递给模型的完整提示模板的格式。它可能包括（可选地）系统消息、用户消息和模型的响应。在这个例子中，它只包括系统提示和用户提示。

（3）**PARAMETER**。`PARAMETER` 指令定义了在模型运行时可以设置的一个或多个参数。这里，它设置了要使用的停止序列。当遇到此模式时，LLM将停止生成文本并返回接受提示。我们将添加额外的PARAMETERS来定制我们的模型。

（4）**temperature**。 temperature 可以设置为区间 [0, 1] 内的任何数字。0 意味着模型对问题给出可预测的、可重复的、事实性的答案。1 意味着它可以对其答案更加自由。默认值是 0.8。我们将把它设置为 1.0。

（5）**top_k**。top_k 参数是一个整数值，通常设置在区间 [0,100] 之间。较低的 top_k 值降低了 LLM 生成无意义内容的概率。它的默认值是 40，但我们将把它设置为 100。

（6）**top_p**。top_p 参数是一个位于区间 [0,1] 之间的浮点值。较高的值，即 1.0 意味着 LLM 被允许考虑更广泛的可能下一个 token 范围，从而允许更多的创造力。我们将把它设置为 1.0。

（7）**SYSTEM**。SYSTEM 不是 LLM 本身的自然语言理解或生成能力的一部分，而是控制LLM运行系统的命令。我希望LLM以1930年代黑手党暴徒的风格回应提示。我们如何做到这一点？很简单，只需将该指令放入SYSTEM指令中。

# 导入导出模型

## 导出模型

```sh
# 查看模型信息, 获得类似信息 FROM /Users/m2max/.ollama/models/blobs/sha256-87f26aae09c7f052de93ff98a2282f05822cc6de4af1a2a159c5bd1acbd10ec4
ollama show --modelfile llama3.1:8b
# 导出模型
cp /Users/m2max/.ollama/models/blobs/sha256-87f26aae09c7f052de93ff98a2282f05822cc6de4af1a2a159c5bd1acbd10ec4 /data/model/lama3_1_8b.gguf

```

## 导入模型

准备Modelfile文件

```sh
vi lama3_1_8b.Modelfile
```

内容如下所示

```sh
From /data/model/llama3_2_1b.gguf
# 其他参数，例如 template 和 stop parameter 取决于具体的模型，可暂时不写
```

执行以下语句导入模型， 导入模型的时候，确保硬盘可用空间至少为模型大小的2倍以上

```sh
ollama create llama3.2:1b -f llama3_2_1b.Modelfile
```

查看确认已经导入的模型 

```sh
# 查看模型清单
ollama list
NAME           ID              SIZE      MODIFIED           
llama3.2:1b    5a11cd5011e6    1.3 GB    About a minute ago
# 查看模型存储位置
ollama show --modelfile llama3.2:1b
# Modelfile generated by "ollama show"
# To build a new Modelfile based on this, replace FROM with:
# FROM llama3.2:1b

FROM /data/.ollama/blobs/sha256-ea3e27b32f043de9546bc4f3fe6c7ec83bc6fa2372dc9fd8ff0abd46f902cb6e
TEMPLATE {{ .Prompt }}
```

# 测试

## 测试环境

| No.  | key    | value                                               |
| ---- | ------ | --------------------------------------------------- |
| 1    | Server | Red Hat Enterprise Linux Server release 7.4 (Maipo) |
| 2    | CPU    | Intel(R) Xeon(R) CPU E7-4820 v3 @ 1.90GHz 16核      |
| 3    | Memory | 32GB                                                |

## 结果

```sh
ollama run llama3.2:1b
>>> who are you?
GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG

>>> where are you?
GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
```

不进行问题输入时，CPU、内存占用可以忽略。提交问题时，CPU利用了基本100%（16C 1600%）。

# RAG

检索增强生成（Retrieval-Augmented Generation，RAG）是一种结合检索和生成技术的模型。它通过引用外部知识库的信息来生成答案或内容，具有较强的可解释性和定制能力，适用于问答系统、文档生成、智能助手等多个自然语言处理任务中。

RAG模型的优势在于通用性强、可实现即时的知识更新，以及通过端到端评估方法提供更高效和精准的信息服务

RAG中有五个关键阶段，这些阶段将成为您构建的任何更大应用程序的一部分。这些阶段是如下所示。

（1）**加载**：这指的是从数据所在的地方（无论是文本文件、PDF、另一个网站、数据库还是API）获取您的数据，将其引入您的管道。LlamaHub 提供了数百个可供选择的连接器。

（2）**索引**：这意味着创建一个允许查询数据的数据结构。对于LLMs，这几乎总是意味着生成`矢量嵌入`
，这是您的数据意义的数字表示，以及许多其他元数据策略，使其易于准确找到上下文相关的数据。

（3）**存储**：一旦您的数据被索引，您几乎总是希望存储您的索引，以及其他元数据，以避免必须重新索引它。

（4）**查询**：对于任何给定的索引策略，您可以使用LLMs和LlamaIndex数据结构以多种方式进行查询，包括子查询、多步查询和混合策略。

（5）**评估**：在任何管道中，检查其相对于其他策略或在进行更改时的有效性是一个关键步骤。评估提供了有关您对查询的响应的准确性、忠实度和速度的客观度量。

比较成熟的框架有  LlamaIndex 。
