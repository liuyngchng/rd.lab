本地化离线部署 Llama3

详见  https://www.clzg.cn/article/619237.html



# ollama

首先拉取 ollama， ollama 相当于大模型界的 docker, 详见 https://ollama.com/download。

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
ollama serve &

```

此时服务已经运行了， 可通过 查看 http://127.0.0.1:11434/ ，显示 “Ollama is running”

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







