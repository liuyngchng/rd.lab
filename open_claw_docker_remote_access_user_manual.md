



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
  openclaw:1.0


# 进入容器
docker exec -it my_claw bash

# 针对自签证书的私有化部署的大模型，关闭客户端 TLS证书验证
export NODE_TLS_REJECT_UNAUTHORIZED=0

# 按照官网安装

#配置  开启远程访问 remote  wss://****18789
openclaw onboard

# 启动
openclaw gateway run --allow-unconfigured

# 浏览器中访问 ， token详细见 ～/.openclaw/config.json 中的 gateway.auth.token
htp://localhost:38789/chat?token=your_token&session=main

# 开启远程访问

openclaw config set gateway.controlUi.allowedOrigins '["http://localhost:38789", "http://127.0.0.1:38789", "http://192.168.*.*:38789"]'


```

