



```sh
# 宿主机中执行
mkdir -p /data/claw
docker stop my_claw
docker rm my_claw

docker pull ubuntu:24.04

# 运行容器
docker run -dit \
  --security-opt seccomp=unconfined 	\
	--security-opt apparmor=unconfined 	\
	--privileged \
  --name my_claw \
  -p 38789:18789 \
  -p 38790:18790 \
  -p 38791:18791 \
  -v /data/claw:/root/.openclaw \
  openclaw:1.1


# 进入容器
docker exec -it my_claw bash

# 针对自签证书的私有化部署的大模型，关闭客户端 TLS证书验证
export NODE_TLS_REJECT_UNAUTHORIZED=0

# 按照官网安装

#配置：QuickStart模式，配置私有化部署的LLM API
openclaw onboard

#配置：Manual setup 模式，开启远程访问 remote  wss://****18789
openclaw onboard

# 启动
openclaw gateway run --allow-unconfigured

# 浏览器中访问 ， token详细见 ～/.openclaw/config.json 中的 gateway.auth.token
http://localhost:38789/chat?token=7ad0419365d5dee741673b4a259751&session=main

# 开启远程访问

openclaw config set gateway.controlUi.allowedOrigins '["http://localhost:38789", "http://127.0.0.1:38789", "http://11.14.*.*:38789"]'


```

