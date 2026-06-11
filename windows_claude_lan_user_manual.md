内网部署了自有的大语言模型，例如deepseek等，在自己PC上安装claude code，使用内网的大模型进行日常办公，写Word，ppt，代码等。Claude 可以输出较为出色的文件。

**（1）安装claude。** Windows下 首先安装 https://nodejs.org/dist/v24.16.0/node-v24.16.0-x64.msi。

然后执行 CMD + R ，输入powershell，执行

```
npm install -g @anthropic-ai/claude-code
```

**（2）配置Windows环境变量**

```sh
ANTHROPIC_BASE_URL 						= http://127.0.0.1:16001
ANTHROPIC_AUTH_TOKEN 					= sk-****
API_TIMEOUT_MS							= 600000
ANTHROPIC_MODEL							= deepseek-chat
ANTHROPIC_SMALL_FAST_MODEL				= deepseek-chat
CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC= 1
CLAUDE_CODE_ATTRIBUTION_HEADER			= 0
```



**（3）兼容 Anthropic 接口的环境准备。**下载接口转接程序， https://gitee.com/liuyngchng/gitee_llm_agent ，然后解压缩。

执行 CMD + R ，输入powershell

```sh
# 进入 D 盘
d:
# 进入工作目录
cd workspace
# 启动内网大模型接口代理
cd gitee_llm_agent
python -m apps.llm_api_adapter.app
```

**（4）启动claude。** 注意，先关闭系统代理（内网用于上外网的代理），等执行完了CMD + R 再打开系统代理。 （不然claude 连接 http://127.0.0.1:16001 可能会走了代理）

执行 CMD + R ，输入powershell

```sh
# 进入 D 盘
d:
# 进入工作目录
cd workspace
# 启动 claude 客户端
claude
# 输入 “你好”， 有反馈说明已经连接上了内网的大模型了，可以正常工作了，写ppt，word，代码等
```

