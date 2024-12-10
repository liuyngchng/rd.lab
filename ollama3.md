本地化离线部署 Llama3

详见  https://www.clzg.cn/article/619237.html



# ollama

首先拉取 ollama， ollama 相当于大模型界的 docker, 详见 https://ollama.com/download。

可以进行离线下载， 下载地址详见  https://github.com/ollama/ollama/releases/, amd64链接



https://github.com/ollama/ollama/releases/download/v0.5.1/ollama-linux-amd64.tgz

#  下载模型镜像

详见 https://ollama.com/library/llama3:8b

```sh
ollama pull llama3:8b
```

# WebUI

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
ollama run 模型名称
## 从注册表中拉去模型
ollama pull 模型名称
## 将模型推送到注册表
ollama push
## 列出模型
ollama list
## 复制模型
ollama cp
## 删除模型
ollama rm 模型名称
## 获取有关Ollama任何命令的帮助信息
ollama help
```







