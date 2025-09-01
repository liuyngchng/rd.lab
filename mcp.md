# 1. 官方网站

https://modelcontextprotocol.io/

# 2. 工作流程

MCP（model context protocol） 的核心流程如下所示。

（1）**部署模型服务：** 将各种 AI 模型部署为符合 MCP 协议的网络服务（理解并生成 `Context` 对象）。

（2）**配置协调器：** 告知协调器（可以是另一个 LLM 或应用逻辑）有哪些可用的 MCP 模型服务及其能力、端点。

（3）**接收用户输入：** 用户提出问题或需求。

（4）**协调器决策：** 协调器（LLM 或逻辑）分析需求，决定是否需要以及将哪些子任务委托给哪个 MCP 模型服务。

（5）**构建 & 发送 MCP 请求：** 应用层/协调器根据 MCP 协议构建 `Context` 请求对象，发送给目标模型服务。

（6）**模型服务处理：** 目标模型服务执行任务，将结果封装成 `Context` 响应对象返回。

（7）**整合结果 & 最终响应：**

​	a） 如果结果直接可用 -> 格式化返回用户。

​	b）更常见的是将子模型结果 + 原始问题组合成新 `Context` -> 送回协调器 LLM -> 协调器 LLM 生成最终自然语言回答 -> 返回用户。

1. **(可选) 循环协作：** 最终协调器 LLM 可能触发新一轮委托（回到步骤 4）。

# 3. server

```sh
# 创建虚拟环境
virtualenv mcp_py_env
mkdir my_mcp
source ../mcp_py_env/bin/activate
# 下载安装server 和 client 的python组件
pip install mcp mcp[cli]
# 查看 mcp 组件版本
mcp version
# 创建 mcp server 文件
touch server.py
# 添加可执行权限
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
# 或者通过以下指令运行
mcp run server.py
# 浏览器输入地址
http://localhost:8001/mcp  # 可见返回 json
# 健康检查
http://localhost:8001/health # 可见返回 json
````



# 4. client

## 4.1 Python SDK

```sh
# 创建 mcp client 文件
touch client.py
# 添加可执行权限
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



## 4.2 curl

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
curl -X POST -s http://localhost:8001/mcp   -H "Content-Type: application/json"   -H "Accept: application/json, text/event-stream"   -H "mcp-session-id: your_session_id"   -d '{
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

# 5. 交互流程

MCP(Model Context Protocol)  的核心流程

用户提问 -> LLM 分析需求(决定是否/如何调用 Tools)  -> 分析需要调用哪些 tools ->  LLM 发起 tools 调用 -> 获取结果 -> LLM 解析/组合数据 -> LLM给用户给出答案



```sequence
MCP 客户端->> MCP 模型服务端: 请求(输入数据 + context{user_id, session_id})
MCP 模型服务端->>模型: 解析输入/context → 执行推理
模型->>MCP 模型服务端: 返回结果 + 更新context(如模型版本)
MCP 模型服务端->>MCP 客户端: 响应(结果 + context)
```

如果没有 MCP， 那么交互流程如下所示

```sequence
客户端->>模型服务端: 请求(用户问题)
模型服务端->>模型: 分析需求/决定调用工具
模型->>模型服务端: 返回需调用的工具名+参数
模型服务端->>Tool: 直接调用指定工具API(参数)
Tool->>模型服务端: 返回工具执行结果
模型服务端->>模型: 发送工具结果
模型->>模型服务端: 解析/组合结果 → 生成最终答案
模型服务端->>客户端: 响应最终答案
```

# 6. 示例

以查询天气为例，说明 MCP的作用。

## 6.1 传统做法

```python
import openai
import json

# 1. 定义工具列表（函数规范）
tools = [
    {
        "type": "function",
        "function": {
            "name": "get_current_weather",
            "description": "获取指定城市的当前天气",
            "parameters": {
                "type": "object",
                "properties": {
                    "location": {"type": "string", "description": "城市名称"},
                    "unit": {"type": "string", "enum": ["celsius", "fahrenheit"]}
                },
                "required": ["location"]
            }
        }
    }
]

# 2. 用户提问
user_query = "北京现在的气温是多少？"

# 3. 首次调用模型（触发工具选择）
response = openai.chat.completions.create(
    model="gpt-3.5-turbo",
    messages=[{"role": "user", "content": user_query}],
    tools=tools,
    tool_choice="auto"
)
# 4. 解析模型返回的工具调用请求
tool_call = response.choices[0].message.tool_calls[0]
if tool_call.function.name == "get_current_weather":
    args = json.loads(tool_call.function.arguments)
    city = args["location"]
    unit = args.get("unit", "celsius")

    # 5. 执行真实工具函数
    def get_current_weather(location, unit):
        response = requests.get(f"https://weather-api.com/{location}?unit={unit}")
        return response.json()  # 返回真实API数据
    weather_data = get_current_weather(city, unit)
    # 6. 将工具结果送回模型
    second_response = openai.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[
            {"role": "user", "content": user_query},
            {
                "role": "tool",
                "content": json.dumps(weather_data),
                "tool_call_id": tool_call.id
            }
        ]
    )
    # 7. 输出最终回答
    print(second_response.choices[0].message.content)
```

执行过程时序如下所示

```sequence

  

    客户端->>应用程序代码: 发送用户请求("北京天气？")
    应用程序代码->>大模型: 发送请求(携带工具定义)
    Note right of 应用程序代码: openai.chat.completions.create(messages=[用户请求],tools=[工具定义],tool_choice="auto")
    
    大模型-->>应用程序代码: 返回工具调用请求(JSON格式)
    Note right of 应用程序代码: 包含 tool_calls 数组<br> 需手动解析 JSON
    
    应用程序代码->>应用程序代码: 1. 解析工具调用请求 2. 验证函数名3. 提取参数
    Note right of 应用程序代码: if tool_call.function.name == "get_current_weather":args = json.loads(tool_call.function.arguments)
    
    应用程序代码->>Tool: 手动调用工具函数(get_current_weather)
    Note right of 应用程序代码: weather_data = get_current_weather(city, unit)
    
    Tool-->>应用程序代码: 返回原始数据(JSON/对象)
    应用程序代码->>应用程序代码: 手动封装工具结果
    Note right of 应用程序代码: 构建 tool 角色消息：{ "role": "tool",<br> "content": json.dumps(weather_data),"tool_call_id": tool_call.id}
    
    应用程序代码->>大模型: 发送工具执行结果
    Note right of 应用程序代码: openai.chat.completions.create( messages=[原始请求+工具结果])
    
    大模型-->>应用程序代码: 返回自然语言响应
    应用程序代码->>客户端: 转发最终回答("北京当前气温25摄氏度")
```



## 6.2 MCP 做法

开发者只需做两件事

```python

# 1. 声明工具函数 (框架自动注册到MCP路由)
@mcp_tool(name="get_current_weather")
def weather_tool(location: str, unit: str = "celsius"):
    return call_weather_api(location, unit) 
# 2. 启动MCP服务框架
mcp_server = MCPServer(tools=[weather_tool])
mcp_server.deploy(model="gpt-4-turbo")

# 之后所有流程由MCP框架自动处理：
# - 解析LLM的工具调用 → 执行对应函数 → 回传结果 → 生成最终回复
```

执行过程时序如下所示

```sequence
客户端->>MCP服务框架: 发送用户请求("北京天气？")
MCP服务框架->>大模型: 转发请求(携带工具定义)
大模型-->>MCP服务框架: 返回工具调用请求(结构化MCP格式)
MCP服务框架->>Tool: 自动路由+执行 get_current_weather("北京")
Tool-->>MCP服务框架: 返回天气数据(25°C)
MCP服务框架->>MCP服务框架: 自动封装结果(MCP格式)
大模型-->>MCP服务框架: 返回自然语言响应
MCP服务框架->>客户端: "北京当前气温25摄氏度"
```

**（1）Tool。**在Tool 中，各种工具接口， 由用户自行实现后， 注册（暴露）给 MCP 服务框架。 例如查询天气，查询股票，查询火车票等。

**（2）客户端（GUI）。**当用户输入问题时，将用户的问题提交给 LLM（LLM 需支持 tools 调用），同时获取 MCP 服务框架 上的 tools 清单（接口签名清单）提交给 LLM。

**（3）MCP服务框架（MCP Server、 MCP Client）。** MCP服务框架根据 LLM 返回的消息、工具调用信息，自动调用相应的 tool（MCP server 提供的 API），获取tools 调用结果后，再次提供给 LLM，开启多轮对话。这个过程有多个回合，直到 LLM 返回的信息只有消息，不再进行工具调用为止。

**（4）大语言模型(LLM)。** LLM需支持 tools 调用（function 调用），当输入参数中含有系统信息、用户信息、以及 tools 信息时，在输出参数中会有消息、是否调用 tools 以及 调用哪些tools、调用相应的tools入参清单等。

经过以上几个步骤，最终形成由 MCP服务框架驱动， LLM 提供智能能力的，自动调用客户自行实现的系统能力（工具集合）的一个全自动系统，为用户提供一站式信息服务的能力。
