

# 1. 官方镜像(推荐)

### 1.1 启动镜像

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


# 创建证书目录
mkdir -p /data/openclaw/certs

# 生成自签名证书（注意替换 IP 地址）
openssl req -x509 -newkey rsa:4096 \
  -keyout /data/openclaw/certs/key.pem \
  -out /data/openclaw/certs/cert.pem \
  -days 365 -nodes \
  -subj "/C=CN/ST=Beijing/L=Beijing/O=HomeLab/CN=192.168.1.104" \
  -addext "subjectAltName=IP:192.168.1.104"

# 开始配置，生成外部配置文件 注意，需要看登录的用户，是否为node 或者 root, 会在 /data/openclaw 中生成相应的配置文件，方便下次直接使用
docker run -it --rm -v /data/openclaw:/root/.openclaw \
	-e NODE_TLS_REJECT_UNAUTHORIZED=0 \
	ghcr.io/openclaw/openclaw:latest \
	openclaw onboard
	
	

	
# 映射外部已经有的配置文件，运行  
docker run -dit \
  --name openclaw-gateway \
  --rm \
  -v /data/openclaw:/root/.openclaw \
  -e NODE_TLS_REJECT_UNAUTHORIZED=0 \
  -e TZ=Asia/Shanghai \
  -p 19001:18789 \
  ghcr.io/openclaw/openclaw:latest \
  openclaw gateway run --allow-unconfigured

# 以root 运行，安装软件
docker run -dit \
  --name openclaw-gateway \
  --user root \
  -v /data/openclaw:/home/node/.openclaw \
  -e NODE_TLS_REJECT_UNAUTHORIZED=0 \
  -e TZ=Asia/Shanghai \
  -e OPENCLAW_GATEWAY_PASSWORD="openclaw" \
  -p 19001:18789 \
  ghcr.io/openclaw/openclaw:latest \
  openclaw gateway run --allow-unconfigured
  
# 配置
docker exec -it openclaw-gateway bash

	openclaw config set gateway.bind lan
	openclaw config set gateway.tls.enabled true
	openclaw config set gateway.controlUi.allowedOrigins '["https://127.0.0.1:19001","https://192.168.1.104:19001"]'
	openclaw config set gateway.tls.certFile "/home/node/.openclaw/certs/cert.pem"
	openclaw config set gateway.tls.keyFile "/home/node/.openclaw/certs/key.pem"
	
# 配置完成，需要重启，否则无法依然无法访问
docker restart openclaw-gateway
```

### 1.2 开始访问

```sh
# 注意，这里的token得是 gateway.auth.token 处的token
   
https://192.168.1.104:19001/chat?session=main&token=6622d385d6c11f978b1703b158d5a6647843e8205046cf6c
```

就可以打开页面了。 这时候需要在服务器端认证客户端

```
docker exec -it openclaw-gateway openclaw devices approve c9d4d1b5-efa3-4d50-9aa7-e3f613cc626c
```

如果希望某些IP从浏览器访问能够无需审批，需要手动修改配置文件，编辑 Gateway 主机的配置文件（通常是 ~/.openclaw/config.yaml 或 config/openclaw.yaml）()

```yml
# 验证未能通过
gateway:
  nodes:
    pairing:
      autoApproveCidrs: ["192.168.1.0/24", "10.0.0.0/8"]
```



或者设置环境变量

```sh
# 注意 这种方式不适用于浏览器端来的请求，适用于 iOS / Android App（手机客户端—）首次连接时自动批准
# 以及 macOS / Windows 桌面客户端（桌面 App ）作为节点连接时自动批准
CLI 命令行工具
docker exec -it  openclaw-gateway bash
export OPENCLAW_GATEWAY_NODES_PAIRING_AUTOAPPROVECIDRS='["192.168.1.0/24", "10.0.0.0/8"]' 
```

### 1.3 配置文件权限

如果在容器外部手动修改了的配置文件，则需要修改权限

```sh
# 将目录所有者设置为 UID 1000, GID 1000
sudo chown -R 1000:1000 /data/openclaw
# 为所有者添加读写执行权限
sudo chmod -R u+rwX /data/openclaw
```





# 2. 自定义镜像（不推荐） 

## 2.1 配置



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



## 2.2 nginx

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



## 2.3 远程访问



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





# 3. 办公相关组件

组件 脚本 install_deps.sh

```sh
#!/bin/bash
set -e

echo "========================================="
echo "OpenClaw 办公自动化环境安装"
echo "========================================="

# 1. 安装 APT 系统组件
echo "[1/4] 安装系统组件..."
apt-get update
apt-get install -y --no-install-recommends \
    libreoffice-core \
    libreoffice-writer \
    libreoffice-calc \
    python3-uno \
    python3-venv \
    fonts-wqy-zenhei \
    fonts-wqy-microhei \
    tesseract-ocr \
    tesseract-ocr-chi-sim

# 2. 清理 APT 缓存（减小镜像体积）
echo "[2/4] 清理系统缓存..."
rm -rf /var/lib/apt/lists/*

# 3. 创建虚拟环境并安装 Python 组件
echo "[3/4] 创建虚拟环境并安装 Python 组件..."

# 创建虚拟环境（如果不存在）
if [ ! -d "/app/venv" ]; then
    python3 -m venv /app/venv
    echo "✅ 虚拟环境已创建: /app/venv"
fi

# 激活虚拟环境并安装包
source /app/venv/bin/activate
pip install --no-cache-dir \
    python-docx \
    openpyxl \
    pandas \
    pdfplumber \
    PyPDF2 \
    reportlab \
    python-pptx \
    pytesseract \
    pillow

# 【关键步骤】将系统 uno 模块链接到虚拟环境
echo "🔗 链接 python3-uno 模块到虚拟环境..."
PYTHON_VERSION=$(python3 -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}')")
ln -sf /usr/lib/python3/dist-packages/uno.py /app/venv/lib/python$PYTHON_VERSION/site-packages/uno.py
ln -sf /usr/lib/python3/dist-packages/unohelper.py /app/venv/lib/python$PYTHON_VERSION/site-packages/unohelper.py

# 4. 验证安装
echo "[4/4] 验证安装..."
source /app/venv/bin/activate
python3 -c "import docx, openpyxl, pandas, pdfplumber, PyPDF2, pptx, pytesseract, PIL; print('✅ 所有 Python 组件导入成功')"
python3 -c "import uno; print('✅ python3-uno 模块导入成功')"
libreoffice --version
tesseract --version

echo "========================================="
echo "✅ OpenClaw 环境安装完成！"
echo "📁 虚拟环境位置: /app/venv"
echo "🔧 激活命令: source /app/venv/bin/activate"
echo "========================================="
```

