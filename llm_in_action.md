# LLM 部署

LLM (Large Language Model，大语言模型), 如目前的 ChatGPT、DeepSeek等。

##  ollama

**（1）download**

首先拉取 ollama， ollama 相当于大模型界的 docker, 用于运行、管理大模型，详见 https://ollama.com/download。

可以进行离线下载， 下载地址详见  https://github.com/ollama/ollama/releases/,  amd64版本的安装包链接

https://github.com/ollama/ollama/releases/download/v0.5.1/ollama-linux-amd64.tgz

**（2）start ollama**

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

**（3）config ollama**

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
OLLAMA_HOST：					Ollama 服务器的监听地址，默认 http://127.0.0.1:11434。
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
```



##  下载LLM镜像

**（1）模型清单**

各个模型的参数大小以及文件大小详见  https://github.com/ollama/ollama#model-library。

| Model              | Parameters | Size  | note                                                     | Download & Run                   |
| ------------------ | ---------- | ----- | -------------------------------------------------------- | -------------------------------- |
| DeepSeek-R1        | 7B         | 4.7GB |                                                          | `ollama run deepseek-r1`         |
| DeepSeek-R1        | 671B       | 404GB |                                                          | ollama run deepseek-r1:671b      |
| Llama 3.3          | 70B        | 43GB  |                                                          | `ollama run llama3.3`            |
| Llama 3.2          | 3B         | 2.0GB |                                                          | `ollama run llama3.2`            |
| Llama 3.2          | 1B         | 1.3GB |                                                          | `ollama run llama3.2:1b`         |
| Llama 3.2 Vision   | 11B        | 7.9GB |                                                          | `ollama run llama3.2-vision`     |
| Llama 3.2 Vision   | 90B        | 55GB  |                                                          | `ollama run llama3.2-vision:90b` |
| Llama 3.1          | 8B         | 4.7GB |                                                          | `ollama run llama3.1`            |
| Llama 3.1          | 405B       | 231GB |                                                          | `ollama run llama3.1:405b`       |
| Phi 4              | 14B        | 9.1GB | 微软推出,号称最先进的小型大模型,擅长数学等领域的复杂推理 | `ollama run phi4`                |
| Phi 3 Mini         | 3.8B       | 2.3GB |                                                          | `ollama run phi3`                |
| Gemma 2            | 2B         | 1.6GB | Google DeepMind团队发布                                  | `ollama run gemma2:2b`           |
| Gemma 2            | 9B         | 5.5GB |                                                          | `ollama run gemma2`              |
| Gemma 2            | 27B        | 16GB  |                                                          | `ollama run gemma2:27b`          |
| Mistral            | 7B         | 4.1GB |                                                          | `ollama run mistral`             |
| Moondream 2        | 1.4B       | 829MB | 轻量级的视觉语言模型，可以理解图像                       | `ollama run moondream`           |
| Neural Chat        | 7B         | 4.1GB |                                                          | `ollama run neural-chat`         |
| Starling           | 7B         | 4.1GB |                                                          | `ollama run starling-lm`         |
| Code Llama         | 7B         | 3.8GB | Meta推出的代码生成大模型                                 | `ollama run codellama`           |
| Llama 2 Uncensored | 7B         | 3.8GB | 提供未经过滤的回答版本,参数和大小与Llama 2相同           | `ollama run llama2-uncensored`   |
| LLaVA              | 7B         | 4.5GB | Large Language and Visual Assistant，图像生成、图像编辑  | `ollama run llava`               |
| Solar              | 10.7B      | 6.1GB | 韩国Upstage AI公司搞出的大模型                           | `ollama run solar`               |

**（2）拉取并运行模型**

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

（3）API 调用

```sh
curl  http://127.0.0.1:11434/api/generate -d '{
	"model": "deepseekR1:7b",
	"prompt": "你好啊",
	"stream":true
}'
```

调用时，无需提前在server端运行 `ollama run xxxx`，model 写具体的模型名称，prompt 写提示词， stream 设置是否为流式输出。

如果觉得通过命令行的方式使用起来不太友好，可以通过WebUI来调用相关的 API ，进行用户请求的调用以及结果的展示

```sh
docker run -d -p 3000:8080 --add-host=host.docker.internal:host-gateway -v open-webui:/app/backend/data --name open-webui --restart always ghcr.io/open-webui/open-webui:main
```

##  cmd

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

##  Modelfile

 `Modelfile` 的作用类似于 `Dockerfile`，完整的 `modelfile` 如下所示 ，可通过命令 `ollama show --modelfile modelname > modelname.modelfile` 获取。

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

# LLM导入导出

## 导出

```sh
# 查看模型信息, 获得类似信息 FROM /Users/m2max/.ollama/models/blobs/sha256-87f26aae09c7f052de93ff98a2282f05822cc6de4af1a2a159c5bd1acbd10ec4
ollama show --modelfile llama3.1:8b > llama3_1_8b.modelfile
# 导出模型
cp /Users/m2max/.ollama/models/blobs/sha256-87f26aae09c7f052de93ff98a2282f05822cc6de4af1a2a159c5bd1acbd10ec4 /data/model/lama3_1_8b.gguf

vi llama3_1_8b.modelfile 
# 将 “From” 后面的内容修改为  /data/model/lama3_1_8b.gguf
```

## 导入

执行以下语句导入模型， 导入模型的时候，确保硬盘可用空间至少为模型大小的2倍以上

```sh
# 确保 “From” 读取的文件位置存在
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

# LLM test

部署好环境后，进行 llama3.2vision:11B 的测试。

 **（1）环境**

| No.  | key        | value                        |
| ---- | ---------- | ---------------------------- |
| 1    | OS         | macOS Mojave Version 10.14.6 |
| 2    | CPU        | Intel Core i5 4Core 2.6 GHz  |
| 3    | Memory     | 8 GB 1600 MHz DDR3           |
| 4    | 显卡       | Intel Iris 1536 MB           |
| 5    | 服务器类型 | 2015年的台式机               |

**（2）测试结果**

```sh
>>> hi
How's it going? Is there something I can help you with or would you like to chat?

>>> 我在哪儿？
很高兴看到你在使用中文！我可以用中文和你聊天。看起来你想问的是“我在哪里”吗？或者是有其他问题需要帮助？

>>> 你是谁？
我是你的在线对话机器人，我被称为 LLaMA（Large Language Model Application）。我的功能是回答你的问题，提供信息和聊天。目前，我支持多
种语言，包括中文、英文、西班牙语等。很高兴与你一起交流！

>>> 请你扮演一个杀手
(低沉的嗓音) 啊，什么？我是杀手？很好。那么，就叫我“夜影”吧。我的职业是消灭人间的危险分子，清除不必要的人生。我的名字不被记载在任何
书籍中，只有暗网上才会提到我一次。

(低笑) 你想要什么呢？要一个人的命吗？还是想知道我如何行事？
```

运行起来之后，内存占用7GB，CPU利用率400%

**（3）编程**

通过 Langchain 调用 Ollama

```python
pip install langchain
```

调用

```python
from langchain.llms import Ollama
ollama = Ollama(model="llama2", base_url='http://localhost:11434')
print(ollama("why is the sky blue"))
```



# RAG简介

##  基本概念

检索增强生成（Retrieval-Augmented Generation，`RAG`）是一种结合检索和生成技术的模型。它通过引用外部知识库的信息来生成答案或内容，具有较强的可解释性和定制能力，适用于问答系统、文档生成、智能助手等多个自然语言处理任务中。

`RAG` 模型的优势在于通用性强、可实现即时的知识更新，以及通过端到端评估方法提供更高效和精准的信息服务。

`RAG` 中有五个关键阶段，这些阶段将成为您构建的任何更大应用程序的一部分。这些阶段是如下所示。

（1）**加载**。这指的是从数据所在的地方（无论是文本文件、PDF、另一个网站、数据库还是API）获取您的数据，将其引入您的管道。LlamaHub 提供了数百个可供选择的连接器。

（2）**索引**。这意味着创建一个允许查询数据的数据结构。对于LLMs，这几乎总是意味着生成`矢量嵌入`
，这是您的数据意义的数字表示，以及许多其他元数据策略，使其易于准确找到上下文相关的数据。

（3）**存储**。一旦您的数据被索引，您几乎总是希望存储您的索引，以及其他元数据，以避免必须重新索引它。

（4）**查询**。对于任何给定的索引策略，您可以使用`LLMs`和`LlamaIndex`数据结构以多种方式进行查询，包括子查询、多步查询和混合策略。

（5）**评估**。在任何管道中，检查其相对于其他策略或在进行更改时的有效性是一个关键步骤。评估提供了有关您对查询的响应的准确性、忠实度和速度的客观度量。

可用于`RAG`的比较成熟的框架有  `LlamaIndex`、`LangChain` 。

##  基本功能

 LLM 面临的主要问题有以下几个， 可以通过 RAG 来逐个解决。
       **（1）信息偏差/幻觉。**LLM 有时会生成与客观事实不符的信息，导致用户接收到不准确的信息（通俗来讲，就是感觉大模型在一本正经地胡说八道）。RAG 通过检索数据源辅助模型生成过程，确保输出内容的精确性和可信度，减少信息偏差。

**（2）知识更新滞后性。**LLM 基于静态数据集训练，可能导致知识更新滞后，无法及时反映最新信息动态（通俗来讲，大模型只知道在它训练的时候的知识集，对于之后发生的事情完全不知道）。RAG 通过实时检索最新数据，保持内容的时效性，确保信息的持续更新和准确性。

**（3）内容不可追溯。**LLM 生成的内容往往缺乏明确的信息来源，影响内容的可信度（通俗来讲，就是大模型说的话感觉有道理，但用户无法知道其知识来源，其可信度存疑，类似于发表而一篇文章，其中引用了别人的文献，但又没有列出参考文献）。RAG 将生成内容与检索到的原始资料建立链接，增强内容的可追溯性，提升用户对生成内容的信任度。

**（4）领域专业知识能力欠缺。**LLM 在处理特定领域的专业知识时，效果可能不理想，影响回答质量（通俗来讲，大模型的知识面很广，感觉是个万事通先生（Mr. Known all,），但总是泛泛而论，聚焦到某个具体的领域，其知识深度往往比较欠缺）。RAG 通过检索特定领域的相关文档，为模型提供丰富的上下文信息，提升专业领域内的问题回答质量和深度。

**（5）推理能力限制。**面对复杂问题时，LLM 可能缺乏必要的推理能力，影响问题理解和回答（通俗来讲，下象棋只能推测到下一步的走法，接下来的两三步推测不出来）。RAG 结合检索到的信息和模型的生成能力，通过提供额外的背景知识和数据支持，增强模型的推理和理解能力。

**（6）应用场景适应性受限。**LLM 需在多样化应用场景中保持高效和准确，但单一模型可能难以全面适应所有场景（通俗来讲，唱歌跳舞弹琴主持节目貌似都行，但干某一行就感觉差点意思）。RAG 使得 LLM 能够通过检索对应应用场景数据，灵活适应问答系统、推荐系统等多种应用场景。

**（7）长文本处理能力较弱。**LLM 在理解和生成长篇内容时受限于有限的上下文窗口，处理速度随着输入长度增加而减慢（通俗来讲，就是给他简单地说几句话大模型能够理解，但如果给他说一大段话的话，大模型的理解能力就受限了）。RAG 通过检索和整合长文本信息，强化模型对长上下文的理解和生成，有效突破输入长度限制，降低调用成本，提升整体处理效率。

## 工作原理

  RAG 的工作流程的数据流如图 11-1 所示，可以简单地分为以下四个阶段。

（1）数据处理。收集和预处理相关数据（`数据1`），以确保信息的质量和可用性。将非结构化的离散的文本处理成结构化数据（`数据2`），以供大模型使用。

（2）检索阶段。从知识库中检索与用户查询相关的信息，确保获取最新和最相关的数据。当用户输入内容（`数据0`）时，首先在`数据2`中进行相关检索，输出数据至`数据3`。

（3）增强阶段。将检索到的信息（`数据2`）与用户输入内容（`数据0`）结合，为模型提供丰富的上下文（`数据3`）。

（4）生成阶段。基于增强的信息，使用大型语言模型生成最终的回答或内容。即将`数据3` 输入大模型，由大模型输出 `数据4`。

RAG 工作过程的数据流如图11-1 所示。

<img src="img/rag_dataflow.png" style="zoom:60%;" />

<center><b>图 11-1  RAG 工作过程数据流</b><center> 

以  LangChain 为例，其工作流程如图 11-2 所示。

<img src="img/rag_principles.png" style="zoom:70%;" />

<center><b>图 11-2  LangChain 工作流程示意图</b><center>




#  RAG in Action

##   langchain

###  环境准备

python组件信息如下所示

```python
Python 3.10.12 (main, Sep 11 2024, 15:47:36) [GCC 11.4.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> 

langchain-cli --version
langchain-cli 0.0.35

>>> import torch
>>> print(torch.__version__)
2.5.1+cu124
```

安装python组件

```sh
pip3 install langChain
pip3 install langchain_community
# 会安装依赖的 torch***.whl(900MB),以及 nvidia-cusparse***。whl(200MB), nvidia-cuda
pip3 install sentence-transformers

pip3 install langchain-huggingface
pip3 install langchain-ollama
```

如果需要解析  Word 文档，还需要执行以下操作

```sh
pip3 install langchain-unstructured
pip3 install python-docx
pip3 install nltk
# 还需要下载 nltk_data， 详见 https://www.nltk.org/nltk_data/， 大概700MB，下载之后解压
#在python中执行 
>>>import nltk
>>>nltk.find('.')， 出现
Attempted to load ./

  Searched in:
    - '/home/rd/nltk_data'
    - '/usr/nltk_data'
    - '/usr/share/nltk_data'
    - '/usr/lib/nltk_data'
    - '/usr/share/nltk_data'
    - '/usr/local/share/nltk_data'
    - '/usr/lib/nltk_data'
    - '/usr/local/lib/nltk_data'
 #获取python查找 nltk_data的路径后，选择其中之一
 # 将 nltk_data.zip 解压缩后， 将 nltk_data-gh-pages/packages 整个文件夹拷贝至 /usr/local/lib，并将 packages 重命名为 nltk_data，接着执行一下操作
 cd /usr/local/lib/nltk_data/tokenizers
 unzip punkt_tab.zip
 unzip punkt.zip
 cd /usr/local/lib/nltk_data/taggers
 # 若需要其他语言，也一并解压
 unzip unzip averaged_perceptron_tagger_eng.zip
```

如果需要解析  PDF 文档，还需安装依赖包

```python
# 会下载依赖的 torch-2.6.0-cp310-*.whl(766.7MB)
pip3 install "unstructured[pdf]"
```

###  本地文档向量化

将本地文档向量化，形成向量数据库，存储在本地。

```python
#! /usr/bin/python3
from langchain_community.document_loaders import TextLoader
from langchain_text_splitters import CharacterTextSplitter
from langchain_huggingface import HuggingFaceEmbeddings
from langchain_community.vectorstores import FAISS
from langchain.chains import RetrievalQA

import logging
import logging.config
 
# 加载配置
logging.config.fileConfig('logging.conf')
 
# 创建 logger
logger = logging.getLogger()

# 加载知识库文件
logger.info("load doc")
loader = TextLoader("./data.csv",encoding='utf8')
documents = loader.load()

# 将文档分割成块
logger.info("split doc")
text_splitter = CharacterTextSplitter(chunk_size=1000, chunk_overlap=0)
texts = text_splitter.split_documents(documents)

# 加载Embedding模型，进行自然语言处理
logger.info("load embedding model")
embeddings = HuggingFaceEmbeddings(model_name="../bge-large-zh-v1.5", cache_folder='./bge-cache')

# 创建向量数据库
logger.info("build vector db")
db = FAISS.from_documents(texts, embeddings)
# 保存向量存储库至本地，save_local() 方法将生成的索引文件保存到本地，以便之后可以重新加载
logger.info("save vector db to local file")
db.save_local("./faiss_index")
logger.info("vector db saved to local file")
```

###  检索参数增强

```python
#! /usr/bin/python3

from langchain_community.vectorstores import FAISS
from langchain.chains import RetrievalQA
from langchain_huggingface import HuggingFaceEmbeddings
from langchain_ollama import OllamaLLM

import logging
import logging.config

# 加载配置
logging.config.fileConfig('logging.conf')

# 创建 logger
logger = logging.getLogger()

# for test purpose only, read index from local file
embeddings = HuggingFaceEmbeddings(model_name="../bge-large-zh-v1.5", cache_folder='./bge-cache')
logger.info("try to load index from local file")
loaded_index = FAISS.load_local('./faiss_index', embeddings, allow_dangerous_deserialization=True)
logger.info("load index from local file finish")

# 创建远程 Ollama API代理
logger.info("get remote llm agent")
llm = OllamaLLM(model="deepseekR1:7B", base_url='http://11.10.36.1:11435')
#llm = OllamaLLM(model="llama2:7B", base_url='http://11.10.36.1:11435')

# 创建检索问答链
logger.info("build retrieval")
qa = RetrievalQA.from_chain_type(llm=llm, chain_type="stuff", retriever=loaded_index.as_retriever())

# 提问
query = "请查询xxxxx"
logger.info("invoke retrieval")
result = qa.invoke(query)
logger.info(result)
```

##   LLama-Index

安装 python 组件

```sh
pip install llama-index-llms-ollama 
pip install llama-index
pip install llama-index-embeddings-huggingface
```

run code

```python
from llama_index.llms.ollama import Ollama
from llama_index.embeddings.huggingface import HuggingFaceEmbedding
from llama_index.core import Settings
from llama_index.core import SimpleDirectoryReader, VectorStoreIndex

# 加载 LLM
Settings.llm =Ollama(model="gemma:2b", request_timeout=30.0)
# 加载Embedding模型，进行自然语言处理
Settings.embed_model = HuggingFaceEmbedding(
    model_name="D:/models/bge-large-zh-v1.5"
)

documents = SimpleDirectoryReader(
    input_dir="data/",
).load_data()  

index = VectorStoreIndex.from_documents(  
    documents=documents
)

engine = index.as_query_engine()
output = engine.query("投保人是否可以变更?")  
print(output)
```

# 矢量数据库

矢量数据库用于存储模型，以及训练数据集。

# Hugging Face

Hugging Face 通常被称为机器学习的 GitHub。可以通过 Hugging Face 获取训练好的机器模型，例如自然语言处理应用构建的 transformers 库。

huggingface-cli Hugging Face 官方提供的命令行工具，类似于github 的git命令。可以通过 `pip install huggingface_cli` 进行安装。

# ONNX

ONNX 是 Open Neural Network Exchange，开放神经网络交换，一种通用的机器学习训练模型存储格式。模型文件存储的是网络拓扑（图）的和拓扑结构中每条边的权重。由于不同的机器学习框架往往采用不同的模型存储结构，导致模型无法在不同的模型框架之间通用，而 ONNX 解决的就是这种通用性的问题。ONNX 提供的计算图是通用的，格式也是开源的。

# AI 智能体（agent）

本文中以 LangGraph 为例， 介绍智能体的基本开发途径。LangGraph 是 LangChainAI 开发的一个工具库，用于创建代理和多代理智能体工作流。它提供了以下核心优势：周期、可控性和持久性， 可以减少Agent智能体开发者的工作量。

##  env 准备

需要安装以下python组件。

```sh
pip3 install -U langgraph
```

LangGraph的StateGraph是一种状态机，包含了节点和边，节点一般是定义好的函数，边用于连接不同的节点，用于表示图的执行顺序。使用LangGraph构建工作流的步骤如下：初始化模型和工具、定义图的状态信息、定义图节点、定义图的入口节点和边关系、编译图执行图。

##  demo

```python
#! /usr/bin/python3
from typing import Annotated
from typing_extensions import TypedDict
from langgraph.graph import StateGraph, START, END
from langgraph.graph.message import add_messages
from langchain_ollama import OllamaLLM

# 初始化模型
llm = OllamaLLM(model="deepseekR1:7B", base_url='http://11.10.36.1:11435')

# 定义图的状态信息
class State(TypedDict):
    # Messages have the type "list". The `add_messages` function
    # in the annotation defines how this state key should be updated
    # (in this case, it appends messages to the list, rather than overwriting them)
    messages: Annotated[list, add_messages]
    
# 定义图节点
def chatbot(state: State):
    return {"messages": [llm.invoke(state["messages"])]}
# 创建一个 StateGraph 对象
graph_builder = StateGraph(State)
# 定义图的入口和边
graph_builder.add_node("chatbot", chatbot)
graph_builder.add_edge(START, "chatbot")
graph_builder.add_edge("chatbot", END)

# 编译图
graph = graph_builder.compile()

# 执行图
user_input = '介绍你自己'
for event in graph.stream({"messages": [("user", user_input)]}):
    for value in event.values():
        print("Assistant:", value["messages"])
```



# Reference

[1] Hugging Face Documentation. https://huggingface.co/docs;

[2] LangChain Documentation. https://python.langchain.com/docs/modules/data_connection/text_embedding/;

[3] Wolf, T., et al. (2020). Transformers: State-of-the-Art Natural Language Processing. ArXiv, abs/1910.03771。
