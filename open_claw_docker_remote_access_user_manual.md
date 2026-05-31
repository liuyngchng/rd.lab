# 1. 自定义镜像 

## 1.1 配置



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

# screen 是一个终端复用工具，简单说就是：你关掉终端窗口后，里面的程序还能继续运行, 目前openclaw不允许后台运行
apt-get update && apt-get install -y screen

# 启动
screen -dmS openclaw openclaw gateway run --allow-unconfigured

# 查看日志 完成交互后，千万不要按 Ctrl+C。相反，你应该先按 Ctrl+A，然后松开，再按 D 键。这是 screen 的分离（Detach）快捷键
# 注意， screen -r 是tach 到之前启动的进程上，CTR+C会导致之前启动的进程中断
screen -r openclaw
tail -f /tmp/openclaw/openclaw-2026-05-29.log

# 由于websocket连接的安全要求，非127.0.0.1 本地loopback 的 origin，必须是https, 所以需要部署nginx


# 开启远程访问

openclaw config set gateway.controlUi.allowedOrigins '["http://localhost:38789", "http://127.0.0.1:38789", "http://192.168.*.*:19001"]'


```

.openclaw/openclaw.json 内容如下： cat openclaw.json

```json

{
  "agents": {
    "defaults": {
      "workspace": "/root/.openclaw/workspace",
      "model": {
        "primary": "custom-api-deepseek-com/deepseek-v4-pro"
      },
      "models": {
        "custom-api-deepseek-com/deepseek-v4-pro": {}
      }
    }
  },
  "gateway": {
    "mode": "local",
    "auth": {
      "mode": "token",
      "token": "7ad04193******7105fa546"
    },
    "port": 18789,
    "bind": "lan",
    "tailscale": {
      "mode": "off",
      "resetOnExit": false
    },
    "controlUi": {
      "allowInsecureAuth": true,
      "allowedOrigins": [
        "http://localhost:38789",
        "http://127.0.0.1:38789",
        "https://192.168.1.104:19001"
      ]
    },
    "nodes": {
      "denyCommands": [
        "camera.snap",
        "camera.clip",
        "screen.record",
        "contacts.add",
        "calendar.add",
        "reminders.add",
        "sms.send",
        "sms.search"
      ]
    },
    "remote": {
      "url": "wss://0.0.0.0:18789",
      "password": "*****"
    }
  },
  "session": {
    "dmScope": "per-channel-peer"
  },
  "tools": {
    "profile": "coding"
  },
  "models": {
    "mode": "merge",
    "providers": {
      "custom-api-deepseek-com": {
        "baseUrl": "https://api.deepseek.com/v1",
        "api": "openai-completions",
        "apiKey": "sk-1234567balblaba",
        "models": [
          {
            "id": "deepseek-v4-pro",
            "name": "deepseek-v4-pro (Custom Provider)",
            "contextWindow": 128000,
            "maxTokens": 4096,
            "input": [
              "text"
            ],
            "cost": {
              "input": 0,
              "output": 0,
              "cacheRead": 0,
              "cacheWrite": 0
            },
            "reasoning": false
          }
        ]
      }
    }
  },
  "meta": {
    "lastTouchedVersion": "2026.5.22",
    "lastTouchedAt": "2026-05-28T12:25:42.451Z"
  },
  "skills": {
    "entries": {
      "1password": {
        "enabled": false
      }
    }
  },
  "wizard": {
    "lastRunAt": "2026-05-28T12:16:07.099Z",
    "lastRunVersion": "2026.5.22",
    "lastRunCommand": "onboard",
    "lastRunMode": "remote"
  }
}

```



## 1.2 nginx

由于websocket连接的安全要求，非127.0.0.1 本地loopback 的 origin，必须是https, 所以需要部署nginx

```sh
mkdir -p /data/nginx
cd /data/nginx
```

证书

```sh
mkdir -p /data/nginx
cd /data/nginx

openssl req -x509 -newkey rsa:4096 \
  -keyout key.pem \
  -out cert.pem \
  -days 365 -nodes \
  -subj "/CN=192.168.1.104"
```



创建 /data/nginx/nginx.conf 

```sh
touch nginx.conf
vi nginx.conf
```

nginx.conf 内容如下

```nginx
server {
    listen 443 ssl http2;
    server_name 192.168.1.104;
    
    ssl_certificate /etc/nginx/ssl/cert.pem;
    ssl_certificate_key /etc/nginx/ssl/key.pem;
    
    location / {
        proxy_pass http://172.17.0.2:18789;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
        proxy_set_header Host $host;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }
}
```

启动

```sh
docker run -d \
  --name my_nginx \
  -p 19001:443 \
  -v /data/nginx/nginx.conf:/etc/nginx/conf.d/default.conf:ro \
  -v /data/nginx:/etc/nginx/ssl:ro \
  -v /data/nginx/logs:/var/log/nginx \
  nginx:1.29.4
```



## 1.3 远程访问



浏览器中访问 ， token详细见 ～/.openclaw/config.json 中的 gateway.auth.token

```html
https://192.168.1.104:19001/chat?token=7ad0419365d5dee741673b4a259751cea3443437105fa546&session=main
```

页面会出现



```sh
Device pairing required
This browser needs one-time approval from the Gateway host before it can use the Control UI.
Run openclaw devices list on the Gateway host.
Approve this request: openclaw devices approve 0dd233fa-0502-4e27-8eae-7bb42d1ed9c9.
Reconnect after the approval completes.
Raw error
Device pairing docs
```

执行

```sh
docker exec -it my_claw bash
openclaw devices approve 0dd233fa-0502-4e27-8eae-7bb42d1ed9c9
│
◇  

OpenClaw 2026.5.22 (a374c3a) — Your .env is showing; don't worry, I'll pretend I didn't see it.

│
◇  
│
gateway connect failed: GatewayClientRequestError: scope upgrade pending approval (requestId: 05988b2e-5ca9-4e41-bfcc-c0a9dd2a01b3)
◇  
Direct scope access failed; using local fallback.
# 出现这一行说明成功了。
Approved 24a92d022032d75c09713b76101881ad4826f3e5ee5a0fb407c4f70df350a748 (0dd233fa-0502-4e27-8eae-7bb42d1ed9c9)

```

# 2. 官方镜像

启动镜像

```sh
# 获取镜像
docker pull ghcr.nju.edu.cn/openclaw/openclaw:latest
# 查看版本
docker inspect ghcr.nju.edu.cn/openclaw/openclaw | grep version -i
        "DockerVersion": "",
                "NODE_VERSION=24.14.0",
                "YARN_VERSION=1.22.22",
                "org.opencontainers.image.version": "2026.5.28"
# 打tag，形成官方镜像地址
docker tag ghcr.nju.edu.cn/openclaw/openclaw      ghcr.io/openclaw/openclaw:latest
# 创建目录
mkdir -p /data/openclaw/data

# 创建证书目录
mkdir -p /data/openclaw/certs

# 生成自签名证书（注意替换 IP 地址）
openssl req -x509 -newkey rsa:4096 \
  -keyout /data/openclaw/certs/key.pem \
  -out /data/openclaw/certs/cert.pem \
  -days 365 -nodes \
  -subj "/C=CN/ST=Beijing/L=Beijing/O=HomeLab/CN=192.168.1.104" \
  -addext "subjectAltName=IP:192.168.1.104"

# 开始配置
docker run -it --rm -v /data/openclaw:/home/node/.openclaw \
	-e NODE_TLS_REJECT_UNAUTHORIZED=0 \
	ghcr.io/openclaw/openclaw:latest \
	openclaw onboard
	
# 配置远程， 选择手动模式，然后设置
# Gateway WebSocket URL
#  wss://192.168.1.104:18789
docker run -it --rm -v /data/openclaw:/home/node/.openclaw \
	-e NODE_TLS_REJECT_UNAUTHORIZED=0 \
	ghcr.io/openclaw/openclaw:latest \
	openclaw onboard
	

	
# 运行  
docker run -it --rm -v /data/openclaw:/home/node/.openclaw     -e NODE_TLS_REJECT_UNAUTHORIZED=0       ghcr.io/openclaw/openclaw:latest        openclaw gateway run --allow-unconfigured

docker run -dit \
  --name openclaw-gateway \
  --rm \
  -v /data/openclaw:/home/node/.openclaw \
  -e NODE_TLS_REJECT_UNAUTHORIZED=0 \
  -p 18789:18789 \
  ghcr.io/openclaw/openclaw:latest \
  openclaw gateway run --allow-unconfigured
  
# 配置
docker exec -it openclaw-gateway bash
docker run -it --rm -v /data/openclaw:/home/node/.openclaw \
	-e NODE_TLS_REJECT_UNAUTHORIZED=0 \
	ghcr.io/openclaw/openclaw:latest \
	openclaw config set gateway.bind lan
	
docker run -it --rm -v /data/openclaw:/home/node/.openclaw \
	-e NODE_TLS_REJECT_UNAUTHORIZED=0 \
	ghcr.io/openclaw/openclaw:latest  \
	openclaw config set gateway.controlUi.allowedOrigins '["http://127.0.0.1:18789","http://192.168.1.104:18789"]'

docker run -it --rm -v /data/openclaw:/home/node/.openclaw \
	-e NODE_TLS_REJECT_UNAUTHORIZED=0 \
	ghcr.io/openclaw/openclaw:latest \
	openclaw config set gateway.tls.enabled true
	
docker run -it --rm -v /data/openclaw:/home/node/.openclaw \
	-e NODE_TLS_REJECT_UNAUTHORIZED=0 \
	ghcr.io/openclaw/openclaw:latest \
    openclaw config set gateway.tls.certFile "/home/node/.openclaw/certs/cert.pem"
  
docker run -it --rm -v /data/openclaw:/home/node/.openclaw \
	-e NODE_TLS_REJECT_UNAUTHORIZED=0 \
	ghcr.io/openclaw/openclaw:latest \
    openclaw config set gateway.tls.keyFile "/home/node/.openclaw/certs/key.pem"
```

访问

```sh
https://192.168.1.104:18179/chat?session=main&token=30e937f7a0944b5c66abfaa6d25200fed08c3040d5bd601f
```

就可以打开页面了。 然后认证客户端

```
docker exec -it  openclaw-gateway openclaw devices approve 69f2673d-f9cf-47ee-872d-8a3e9dbabdfd
```

完整配置文件详见

```json
rd@rd-ex:/data/openclaw$ cat openclaw.json
{
  "agents": {
    "defaults": {
      "workspace": "/home/node/.openclaw/workspace",
      "model": {
        "primary": "custom-api-deepseek-com/deepseek-v4-pro"
      },
      "models": {
        "custom-api-deepseek-com/deepseek-v4-pro": {
          "alias": "deepseek-v4-pro"
        }
      }
    }
  },
  "gateway": {
    "mode": "local",
    "auth": {
      "mode": "token",
      "token": "30e937f7a0944b5c66abfaa6d25200fed08c3040d5bd601f"
    },
    "port": 18789,
    "bind": "lan",
    "tailscale": {
      "mode": "off",
      "resetOnExit": false
    },
    "controlUi": {
      "allowInsecureAuth": true,
      "allowedOrigins": [
        "http://127.0.0.1:18789",
        "http://192.168.1.104:18789"
      ]
    },
    "nodes": {
      "denyCommands": [
        "camera.snap",
        "camera.clip",
        "screen.record",
        "contacts.add",
        "calendar.add",
        "reminders.add",
        "sms.send",
        "sms.search"
      ]
    },
    "remote": {
      "url": "wss://192.168.1.104:18789",
      "token": "30e937f7a0944b5c66abfaa6d25200fed08c3040d5bd601f"
    },
    "tls": {
      "enabled": true,
      "certFile": "/home/node/.openclaw/certs/cert.pem",
      "keyFile": "/home/node/.openclaw/certs/key.pem"
    }
  },
  "session": {
    "dmScope": "per-channel-peer"
  },
  "tools": {
    "profile": "coding"
  },

```

