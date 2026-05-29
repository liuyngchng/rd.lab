# 1. 配置



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



# 2. nginx

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



# 3. 远程访问



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

