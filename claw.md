# 20. Claw

## 20.1 Openclaw

**（1）安装 pnpm**

```sh
# 安装 pnpm
curl -fsSL https://get.pnpm.io/install.sh | sh
```



**（2）源代码安装**

```sh
git clone git@github.com:openclaw/openclaw.git


cd openclaw

pnpm install
pnpm ui:build # auto-installs UI deps on first run
pnpm build
# 开始进行系统配置
pnpm openclaw onboard --install-daemon

# Dev loop (auto-reload on TS changes)
pnpm gateway:watch

# 浏览器交互
pnpm openclaw dashboard
# 终端中交互
pnpm openclaw tui
```

查看gateway 日志

```sh
pnpm openclaw logs --follow
```

调用大语言模型的包

```sh
@mariozechner/pi-ai
```

清除已经配置的所有信息

```sh
pnpm openclaw reset
# 清除所有配置以及生成文件等
选择 full reset
```

源码安装后， 创建全局链接

```sh
cd ~/workspace/openclaw
pnpm link --global
# 安装腾讯微信插件
npx -y @tencent-weixin/openclaw-weixin-cli@latest install
# 弹出微信登录二维码
openclaw channels login --channel openclaw-weixin
```

**（3）非源码安装**

```
pnpm install -g openclaw@latest
```



**（4）接入微信**

 安装微信插件

```bash
npx -y @tencent-weixin/openclaw-weixin-cli install
# 或者手动安装
openclaw plugins install "@tencent-weixin/openclaw-weixin"
openclaw config set plugins.entries.openclaw-weixin.enabled true
openclaw gateway restart
```

扫码登录

在运行 Gateway 的同一台机器上执行：

```bash
openclaw channels login --channel openclaw-weixin
```

然后用手机微信扫描终端里显示的二维码，确认登录即可。，授权访问新联系人需要批准才能和我对话：

```bash
openclaw pairing list openclaw-weixin        # 查看待批准列表
openclaw pairing approve openclaw-weixin <CODE>  # 批准某个联系人
```

## 20.2 nanobot

nanobot 实现了 claw 的核心逻辑，详见  git@github.com:HKUDS/nanobot.git

运行

```sh
# clone code
git clone git@github.com:HKUDS/nanobot.git
cd nanobot
# activate python 虚拟环境
source ~/workspace/llm_py_env/bin/activate
# 运行
nanobot agent
python -m nanobot
```

默认 nanobot会联网获取最新的模型信息（https://raw.githubusercontent.com/BerriAI/litellm/main/model_prices_and_context_window.json），便于实时更新其config.json文件内的模型，内网可以设置环境变量跳过这个联网功能

```
export LITELLM_LOCAL_MODEL_COST_MAP=True
```

如果使用了内网部署的自定义模型，部分客户端的证书可能需要跳过验证，需要修改源代码，使用了openai.AsyncOpenAI 作为client的化，需要修改如下内容

```python
# nanobot/providers/openai_compat_provider.py
# 跳过 HTTP client 端的 SSL 证书验证
import httpx
http_client_kwargs = {}
http_client_kwargs["verify"] = False
# 记录日志
from loguru import logger
logger.debug(f"custom_provider, {api_base}, {api_key}")
http_client = httpx.AsyncClient(**http_client_kwargs)
self._client = openai.AsyncOpenAI(
    api_key=api_key,
    base_url=api_base,
    default_headers={"x-session-affinity": uuid.uuid4().hex},
    http_client=http_client,

)
```

涉及到 litellm 的，增加如下配置

```python
litellm.ssl_verify = False  		# 禁用 SSL 验证
litellm.verify_ssl_certs = False  	# 某些版本使用这个
```

运行 vi /home/rd/.nanobot/config.json, 添加 LLM API 信息

## 20.3 ApkClaw

安卓应用，参考 

[安卓开发手册]: ./android.md	"安卓开发手册"

相关文档

```sh
./gradlew clean
./gradlew assembleRelease
/home/rd/Android/Sdk/platform-tools/adb install ./app/build/outputs/apk/release/ApkClaw_v0.0.2_20260411_104935.apk

./gradlew installDebug
```

