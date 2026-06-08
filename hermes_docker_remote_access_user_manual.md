

官网地址：

https://github.com/NousResearch/hermes-agent

打包镜像

```sh
docker pull ubuntu:24.04
docker run --name my_ubuntu  -dit ubuntu:24.04
docker exec -it my_ubuntu bash
apt-get update
apt-get install curl git ripgrep ffmpeg xz-utils
mkdir /app
curl -fsSL https://hermes-agent.nousresearch.com/install.sh | bash
```



保存镜像





```
docker commit my_ubuntu hermes_agent:0.16.0
```

