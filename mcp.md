# 1.model context protocol

MCP 的核心流程如下所示。

（1）**部署模型服务：** 将各种 AI 模型部署为符合 MCP 协议的网络服务（理解并生成 `Context` 对象）。

（2）**配置协调器：** 告知协调器（可以是另一个 LLM 或应用逻辑）有哪些可用的 MCP 模型服务及其能力、端点。

（3）**接收用户输入：** 用户提出问题或需求。

（4）**协调器决策：** 协调器（LLM 或逻辑）分析需求，决定是否需要以及将哪些子任务委托给哪个 MCP 模型服务。

（5）**构建 & 发送 MCP 请求：** 应用层/协调器根据 MCP 协议构建 `Context` 请求对象，发送给目标模型服务。

（6）**模型服务处理：** 目标模型服务执行任务，将结果封装成 `Context` 响应对象返回。

（7）**整合结果 & 最终响应：**

- a) 如果结果直接可用 -> 格式化返回用户。
- b) **更常见：** 将子模型结果 + 原始问题组合成新 `Context` -> 送回协调器 LLM -> 协调器 LLM 生成最终自然语言回答 -> 返回用户。

1. **(可选) 循环协作：** 最终协调器 LLM 可能触发新一轮委托（回到步骤 4）。

# 2. server

```sh
# 创建虚拟环境
virtualenv mcp_py_env
mkdir my_mcp
source ../mcp_py_env/bin/activate
pip install mcp mcp[cli]
mcp version
touch server.py
chmod +x server.py
```

server.py 内容如下

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
pip install mcp
pip install mcp[cli]

FastMCP quickstart example.
"""

import os
from mcp.server.fastmcp import FastMCP
from mcp.types import Request
from starlette.responses import JSONResponse

mcp = FastMCP(port=8001, stateless_http=True, json_response=True)  # 初始化 MCP 服务实例

@mcp.custom_route("/health", methods=["GET"])
async def health_check(request: Request):
    """健康检查端点"""
    print("trigger health_check")
    return JSONResponse({"status": "ok"})

@mcp.tool()
def get_desktop_files():
    """获取桌面上的文件列表"""
    print("trigger get_desktop_files")
    return os.listdir(os.path.expanduser("~/Desktop"))

if __name__ == "__main__":
    print("start mcp server (backend only)")
    # 通信协议：transport = 'stdio', 表示使用标准输入输出，也可替换为 HTTP 或 WebSocket
    mcp.run(transport='streamable-http')  # 添加 frontend=False
```

启动 mcp server

````sh
# 直接运行
./server.py
# 或者通过指令运行
mcp run server.py
````

# 3. client

## 3.1 SDK

```sh
touch client.py
chmod +x client.py
```

client.py 内容如下所示

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Run from the repository root:
    uv run examples/snippets/clients/streamable_basic.py
"""
import asyncio
from mcp import ClientSession
from mcp.client.streamable_http import streamablehttp_client

async def client_test():
    # Connect to a streamable HTTP server
    async with streamablehttp_client("http://localhost:8001/mcp") as (
        read_stream,
        write_stream,
        _,
    ):
        # Create a session using the client streams
        async with ClientSession(read_stream, write_stream) as session:
            # Initialize the connection
            await session.initialize()
            # List available tools
            tools = await session.list_tools()
            print(f"Available tools: {[tool.name for tool in tools.tools]}")
            result = await session.call_tool("get_desktop_files")
            print(f"call result: {result}")

if __name__ == "__main__":
    asyncio.run(client_test())

```

运行 client

```sh
# 首先启动 server
./server.py
# 运行 client 请求 server
./client.py
# 可见如下输出
Available tools: ['get_desktop_files']
```



## 3.2 curl

```
curl -s http://localhost:8001/health | jq

```

init， get session id

```sh
curl -X POST -s http://localhost:8001/mcp   -H "Content-Type: application/json"   -H "Accept: application/json, text/event-stream"   -d '{
    "jsonrpc": "2.0",
    "id": 1,
    "method": "initialize",
    "params": {
      "protocolVersion": "0.0.1",
      "capabilities": {
        "sampling": null,
        "elicitation": null,
        "roots": null,
        "experimental": null
      },
      "clientInfo": {
        "name": "curl-client",
        "version": "1.0"
      }
    }
  }' | jq
```

get tools list

```sh
curl -X POST -s http://localhost:8001/mcp   -H "Content-Type: application/json"   -H "Accept: application/json, text/event-stream"   -H "mcp-session-id: <SESSION_ID_FROM_RESPONSE>"   -d '{
    "jsonrpc": "2.0",
    "id": 2,
    "method": "tools/list"
  }' | jq
```

invoke a tools call

```sh
curl -X POST http://localhost:8001/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "mcp-session-id: your_session_id" \
  -d '{
    "jsonrpc": "2.0",
    "id": 2,
    "method": "tools/call",
    "params": {
      "name": "get_desktop_files",
      "arguments": {}
    }
  }'
```

# 4. 交互流程

MCP(Model Context Protocol)  的核心流程

用户提问 -> LLM 分析需求(决定是否/如何调用 Tools)  -> 分析需要调用哪些tools ->  LLM 发起tools 调用 -> 获取结果 -> LLM 解析/组合数据 -> LLM给用户给出答案
