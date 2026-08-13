# 

# 1. Claude Code

## 1.1 安装

需要具有 `nodejs` 环境，然后安装 `claude code` 的客户端，Windows下首先安装 https://nodejs.org/dist/v24.16.0/node-v24.16.0-x64.msi， 

Linux下执行下面的脚本

```sh
sudo apt-get install nodejs nvm -y
sudo npm install -g @anthropic-ai/claude-code
# 或安装特定版本, 2.1.207
npm install -g @anthropic-ai/claude-code@版本号

# codex
npm install -g @openai/codex
```

Linux也可以通过homebrew这样安装

```sh
sudo apt update
sudo apt install build-essential curl git
# 安装 homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# 对于bash用户(ubuntu)， 添加环境变量
echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> ~/.bashrc
# 验证 brew 版本
brew --version
# 安装 claude code，这种安装，可能无法通过 claude update 更新新的版本
brew install --cask claude-code
```



## 1.2 配置环境变量

**（1）linux**

deepseek, 注意，在模型名称后面添加[1m]， 让claude code知道这个模型的上下文时1M token

```sh
export ANTHROPIC_BASE_URL=https://api.deepseek.com/anthropic
export ANTHROPIC_AUTH_TOKEN=${DEEPSEEK_API_KEY}

export ANTHROPIC_MODEL=deepseek-v4-pro[1m]
export ANTHROPIC_DEFAULT_OPUS_MODEL=deepseek-v4-pro[1m]
export ANTHROPIC_DEFAULT_SONNET_MODEL=deepseek-v4-pro[1m]
export ANTHROPIC_DEFAULT_HAIKU_MODEL=deepseek-v4-flash[1m]
export CLAUDE_CODE_SUBAGENT_MODEL=deepseek-v4-flash[1m]
export CLAUDE_CODE_EFFORT_LEVEL=max
export API_TIMEOUT_MS=600000
# 防止联网验证、模型回退、检查更新失败导致的卡顿或错误
export CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC=1
# 关闭计费头,加速推理
export CLAUDE_CODE_ATTRIBUTION_HEADER=0
```

minimax

```sh
export ANTHROPIC_BASE_URL=https://api.minimaxi.com/anthropic
export ANTHROPIC_AUTH_TOKEN=${MINIMAXI_API_KEY}
export API_TIMEOUT_MS=3000000
export ANTHROPIC_MODEL=MiniMax-M3
export ANTHROPIC_SMALL_FAST_MODEL=MiniMax-M3
export CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC=1
export CLAUDE_CODE_ATTRIBUTION_HEADER=0
export ANTHROPIC_DEFAULT_SONNET_MODEL=MiniMax-M3
export ANTHROPIC_DEFAULT_OPUS_MODEL=MiniMax-M3
export ANTHROPIC_DEFAULT_HAIKU_MODEL=MiniMax-M3
export CLAUDE_CODE_AUTO_COMPACT_WINDOW=512000
```

**（2）windows**

我的电脑-> 属性 -> 高级 ->环境变量 -> 系统环境变量 -> 添加

```sh
ANTHROPIC_BASE_URL=https://api.minimaxi.com/anthropic
ANTHROPIC_AUTH_TOKEN=sk-****
API_TIMEOUT_MS=3000000
ANTHROPIC_MODEL=MiniMax-M3
ANTHROPIC_SMALL_FAST_MODEL=MiniMax-M3
CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC=1
CLAUDE_CODE_ATTRIBUTION_HEADER=0
ANTHROPIC_DEFAULT_SONNET_MODEL=MiniMax-M3
ANTHROPIC_DEFAULT_OPUS_MODEL=MiniMax-M3
ANTHROPIC_DEFAULT_HAIKU_MODEL=MiniMax-M3
CLAUDE_CODE_AUTO_COMPACT_WINDOW=512000
```



启动

```sh
./claude
```

如果报错误， 

```sh
Unable to connect to API: SSL certificate verification failed. Check your proxy or corporate SSL certificates
```

添加环境变量， 忽略 API 的证书错误

```sh
# 危险操作：这将忽略API 中的任何证书错误, NodeJS 忽略HTTPS证书校验
export NODE_TLS_REJECT_UNAUTHORIZED=0
# 将自签名证书添加到NodeJS系统的信任存储中
export NODE_EXTRA_CA_CERTS=/path_to_your_self_signed_cert.pem
```



# 2 Cloud CLI

Cloud CLI（又名 Claude Code UI），详见 https://cloudcli.ai。

```sh
# 安装组件
npm install -g @cloudcli-ai/cloudcli
# 启动
cloudcli
```

https://github.com/siteboon/claudecodeui

获取代码

```sh
git clone git@github.com:siteboon/claudecodeui.git
```

https://github.com/glwhappen/web-code/blob/HEAD/README.zh-CN.md

容器中运行

```sh
docker run -dit \
	--name my_claude_code \
	--rm \
	-v /data/remote/workspace:/opt/workspace \
	-w /opt/workspace \
	-e NODE_TLS_REJECT_UNAUTHORIZED=0 \
	-e ANTHROPIC_BASE_URL=http://127.0.0.1:16001 \
	-e ANTHROPIC_AUTH_TOKEN=sk-8rfe****Fe \
	-e API_TIMEOUT_MS=600000 \
	-e ANTHROPIC_MODEL=deepseek-chat \
	-e ANTHROPIC_SMALL_FAST_MODEL=deepseek-chat \
	-e CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC=1 \
	-e CLAUDE_CODE_ATTRIBUTION_HEADER=0 \
  -e TZ=Asia/Shanghai \
  -e LANG=C.UTF-8 \
  -e LC_ALL=C.UTF-8 \
	-p 19004:3001 \
	my_claude_code:1.0 \
	/root/.nvm/versions/node/v22.22.3/bin/cloudcli
```



# 3. 离线安装

```sh
# 安装工具
npm install -g pack-pub

# 1. 在有网的机器上，下载包及其所有依赖（类似 pip download）
pack-pub pack-offline @anthropic-ai/claude-code

# 2. 将生成的 ./offline-package 目录复制到离线机器上

# 3. 在离线机器上，从目录安装（类似 pip install --find-links）
pack-pub install-offline ./offline-package --global
```



# 4. 配置hosts



```sh
sudo vi /etc/hosts
# 添加如下内容
127.0.0.1 api.anthropic.com
127.0.0.1 anthropic.com
127.0.0.1 claude.ai
```

