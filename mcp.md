# 1. server

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

# 2. client

## 2.1 client sdk

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



## 2.2 client request

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



