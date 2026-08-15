# WorkBuddy 技术栈深度分析报告

> 分析日期：2026-08-15
> 分析对象：`123.exe`（WorkBuddy Desktop v5.3.8 安装包）

---

## 目录

1. [概述](#1-概述)
2. [WorkBuddy 完整技术架构](#2-workbuddy-完整技术架构)
3. [OpenAI Agents SDK 深度解析](#3-openai-agents-sdk-深度解析)
4. [Claude Code 深度解析](#4-claude-code-深度解析)
5. [三者对比分析](#5-三者对比分析)
6. [附录：解包分析过程](#6-附录解包分析过程)

---

## 1. 概述

**WorkBuddy** 是腾讯推出的 AI Agent 桌面应用，本质上是 **CodeBuddy CLI 的 Electron 桌面封装**。它不是一个简单的 AI 聊天工具，而是一个完整的 **agent 宿主平台**，具备以下核心能力：

- 多模型 AI 对话（集成 20+ 个 LLM 模型）
- 代码编辑与项目管理（LSP 集成、Tree-sitter 语法解析）
- 终端命令执行（node-pty 终端模拟、沙箱安全机制）
- MCP 协议 tool 接入（连接外部 tool server）
- Skill 插件系统（内置 Python/Node.js 双运行时）
- 多 Agent 协作（Subagent fork、Handoff）
- 持久记忆系统（MEMORY.md、Working Memory）

---

## 2. WorkBuddy 完整技术架构

### 2.1 整体分层架构

```
┌──────────────────────────────────────────────────────────────────┐
│                    Electron Shell (205MB)                         │
│                    WorkBuddy.exe                                  │
├──────────────────────────────────────────────────────────────────┤
│  app.asar (主应用)                                                │
│  ├── 桌面 UI 层 (Electron Renderer)                               │
│  │   ├── 对话面板 (Chat Panel)                                    │
│  │   ├── 终端面板 (DevTools Terminal — Ghostty WASM)              │
│  │   ├── MCP App 面板 (Webview 嵌入)                              │
│  │   └── 系统托盘 (Tray)                                          │
│  ├── 主进程层 (Electron Main)                                     │
│  │   ├── IPC 通信管理                                             │
│  │   ├── 窗口管理                                                 │
│  │   └── 系统集成 (Tray, 全局快捷键, 深度链接)                     │
│  └── cli/ CodeBuddy CLI (Agent 核心，Node.js)                     │
│      ├── dist/codebuddy.js (6.9MB, rspack 打包)                   │
│      ├── @openai/agents-core (Agent 循环引擎)                     │
│      ├── @modelcontextprotocol/sdk (MCP 通道)                     │
│      ├── @genie/* (腾讯自研封装层)                                 │
│      └── vendor/ (ripgrep, tree-sitter, genie-trash)              │
├──────────────────────────────────────────────────────────────────┤
│  内置运行时 (安装时从 vendor/ 解压)                                │
│  ├── Node.js v22.22.2 (node.zip, ~35MB)                          │
│  ├── Python 3.13 (python.zip, ~19MB)                             │
│  └── Git for Windows (PortableGit.zip, ~51MB)                    │
├──────────────────────────────────────────────────────────────────┤
│  内置资源 (resources/)                                            │
│  ├── builtin-skills/ (内置 Skill 库)                              │
│  ├── builtin-plugins/ (内置 MCP 插件)                             │
│  ├── builtin-mcp-apps/ (内置 MCP App)                             │
│  ├── templates/ (Agent Prompt 模板)                               │
│  └── channel-branding/ (渠道品牌资源)                              │
└──────────────────────────────────────────────────────────────────┘
```

### 2.2 Agent 核心引擎：`@genie/agent-cli` (CodeBuddy CLI)

这是 WorkBuddy 的 agent harness 本体，包名为 `@genie/agent-cli`，对外可独立发布为 `@tencent-ai/codebuddy-code` v2.115.0。

#### 2.2.1 核心依赖图谱

```
┌─────────────────────────────────────────────────────┐
│                @genie/agent-cli                       │
│              (Agent Harness 本体)                     │
├─────────────────────────────────────────────────────┤
│                                                      │
│  ┌──────────────────┐  ┌──────────────────────────┐ │
│  │ @openai/agents    │  │ @modelcontextprotocol/sdk│ │
│  │ @openai/agents-   │  │         v1.29.0          │ │
│  │ core v0.5.2       │  │    MCP 工具接入协议       │ │
│  │ Agent 循环引擎     │  │                          │ │
│  └────────┬─────────┘  └────────────┬─────────────┘ │
│           │                         │               │
│  ┌────────┴─────────────────────────┴─────────────┐ │
│  │              @genie/* 封装层                     │ │
│  │  ┌──────────────┐  ┌────────────────────────┐  │ │
│  │  │ @genie/core   │  │ @genie/prompts          │  │ │
│  │  │ 多模型路由    │  │ Prompt 模板管理          │  │ │
│  │  │ Agent 管理    │  │ System Prompt 拼接       │  │ │
│  │  └──────────────┘  └────────────────────────┘  │ │
│  │  ┌──────────────┐  ┌────────────────────────┐  │ │
│  │  │ @genie/runtime│  │ @genie/product          │  │ │
│  │  │ 运行时环境    │  │ 产品配置/模型清单        │  │ │
│  │  │ 沙箱管理      │  │ Feature Toggle          │  │ │
│  │  └──────────────┘  └────────────────────────┘  │ │
│  │  ┌──────────────┐  ┌────────────────────────┐  │ │
│  │  │ @genie/       │  │ @genie/telemetry         │  │ │
│  │  │ authentication│  │ 遥测/埋点                │  │ │
│  │  │ 多端登录认证  │  │ Galileo 集成             │  │ │
│  │  └──────────────┘  └────────────────────────┘  │ │
│  │  ┌──────────────┐  ┌────────────────────────┐  │ │
│  │  │ @genie/       │  │ @genie/sandbox-cli       │  │ │
│  │  │ code-ratio    │  │ 沙箱执行环境             │  │ │
│  │  │ 代码质量度量  │  │ 安全审批机制             │  │ │
│  │  └──────────────┘  └────────────────────────┘  │ │
│  └────────────────────────────────────────────────┘ │
│                                                      │
│  ┌────────────────────────────────────────────────┐ │
│  │              基础设施                           │ │
│  │  vscode-jsonrpc    → IDE 通信                  │ │
│  │  vscode-languageserver-protocol → LSP 集成     │ │
│  │  @lydell/node-pty  → 终端模拟 (PTY)            │ │
│  │  @xterm/headless   → 终端渲染                  │ │
│  │  tree-sitter       → 语法解析 (Bash AST)       │ │
│  │  ripgrep           → 文件搜索                   │ │
│  │  e2b               → 远程沙箱                   │ │
│  │  centrifuge        → 实时消息推送               │ │
│  │  openai            → LLM API 客户端             │ │
│  └────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────┘
```

#### 2.2.2 Agent 子系统

从 `product.json` 中提取的 Agent 定义（共 20 个 agent）：

| Agent ID | 用途 |
|----------|------|
| `cli-agent-prompt` | CLI 主 Agent 系统提示词 |
| `compact-agent-prompt` | 上下文压缩 Agent |
| `context-summary-agent-prompt` | 上下文摘要 Agent |
| `content-analyzer-agent-instructions` | 内容分析 Agent |
| `terminal-title-generator-instructions` | 终端标题生成 |
| `prompt-suggestion-instructions` | Prompt 建议 |
| `memory-selector-instructions` | 记忆检索/选择 |
| `summary-generator-instructions` | 摘要生成 |
| `auto-mode-classifier-instructions` | 自动模式分类 |
| `prompt-hook-evaluator-instructions` | Hook 评估 |
| `base-agent-instructions` | 基础 Agent 指令 |
| `agent-instructions` | 通用 Agent 指令 |
| `agent-statusline-instructions` | Agent 状态栏 |
| `agent-explore-instructions` | 代码探索 Agent |
| `agent-plan-instructions` | 计划模式 Agent |
| `pulse-agent-prompt` | Pulse Agent（心跳） |
| `handoff-summary-agent-prompt` | Handoff 摘要 |
| `enhance-prompt-system-prompt` | Prompt 增强 |

#### 2.2.3 模型支持

WorkBuddy 集成了 20+ 个 LLM 模型，涵盖国内外主流模型：

| 模型 | 厂商 | 最大输入 Token | 最大输出 Token | Tool Call |
|------|------|---------------|---------------|-----------|
| `default` (Default) | 默认 | 200,000 | 24,000 | ✅ |
| `deepseek-v4-pro` | DeepSeek | 1,000,000 | 50,000 | ✅ |
| `deepseek-v4-flash` | DeepSeek | 1,000,000 | 50,000 | ✅ |
| `deepseek-v3-2-volc` | DeepSeek (火山) | 96,000 | 32,000 | ✅ |
| `deepseek-v3-1` | DeepSeek | 96,000 | 32,000 | ✅ |
| `deepseek-r1-0528` | DeepSeek | 96,000 | 8,192 | ✅ |
| `minimax-m2.5` | MiniMax | 200,000 | 48,000 | ✅ |
| `minimax-m2.7` | MiniMax | 200,000 | 48,000 | ✅ |
| `minimax-m3-play` | MiniMax | 512,000 | 48,000 | ✅ |
| `glm-5.1` | 智谱 GLM | 200,000 | 48,000 | ✅ |
| `glm-5.0` | 智谱 GLM | 200,000 | 48,000 | ✅ |
| `glm-4.7` | 智谱 GLM | 200,000 | 48,000 | ✅ |
| `kimi-k2.6` | Moonshot | 256,000 | 32,000 | ✅ |
| `kimi-k2-thinking` | Moonshot | 256,000 | 32,000 | ✅ |
| `hy3-preview-agent` | 混元 | 192,000 | 64,000 | ✅ |
| `hunyuan-chat` | 混元 | 128,000 | 8,192 | ✅ |
| `hunyuan-2.0-thinking` | 混元 | 128,000 | 24,000 | ✅ |
| `hunyuan-image-v3.0` | 混元 | — | — | 图片生成 |
| `kling-v3-t2v` | 可灵 | — | — | 视频生成 |
| `codewise-completions` | 代码补全专用 | — | 256 | — |

> 注：模型列表可能随产品更新变化，此处基于 v5.3.8 版本 `product.json`。

#### 2.2.4 内置 Skill 系统

Skill 是 WorkBuddy 的工具扩展机制，类似 Claude Code 的 `/skill` 命令。每个 Skill 包含 `SKILL.md`（指令）和 `scripts/`（可执行脚本），支持 Python 和 Node.js 两种运行时。

**内置 Skill 清单：**

| Skill 名称 | 运行时 | 用途 |
|-----------|--------|------|
| `wb-finance-skill` | Python | 金融量化分析（因子研究、技术分析、DCF 估值等） |
| `westock-tool` | Node.js | 微证券行情数据查询 |
| `westock-data` | Node.js | 微证券宏观数据查询 |
| `neodata-financial-search` | Python/Shell | 财务数据搜索 |
| `buddy-multimodal-generation` | Python | 多模态内容生成 |
| `ardot-design-core` | — | UI 设计核心规则 |
| `ardot-ui-design` | — | UI 设计指南 |
| `ardot-slides` | — | 幻灯片设计 |
| `ardot-poster` | — | 海报设计 |
| `ardot-design-to-code` | — | 设计稿转代码 |
| `ardot-design-router` | — | 设计路由分发 |
| `cloudstudio-deploy` | Node.js | CloudStudio 部署 |
| `tencent-local-office-edit` | Python | 本地 Office 文档编辑 |
| `tencent-docs-routing` | — | 腾讯文档路由 |
| `skill-creator` | Python | Skill 创建向导 |
| `expert-manager` | Python | 专家 Agent 管理 |
| `marketplace-skill-installer` | — | 市场 Skill 安装 |
| `geo-map-compliance-guard` | — | 地图合规审查 |

#### 2.2.5 内置 MCP 插件

| 插件 | 用途 |
|------|------|
| `weixinpay` | 微信 AI 支付（ClawPay），支持 MCP 协议 |
| `tencent-docs-plugin` | 腾讯文档 MCP 插件，支持创建/编辑在线文档 |
| `tencent-pptx` | 演示文稿智能助手 |

#### 2.2.6 Prompt 模板系统

WorkBuddy 使用 Jinja2 模板引擎管理 Agent Prompt，模板位于 `resources/templates/`：

| 模板文件 | 用途 |
|---------|------|
| `workbuddy-prompt.tpl` | 通用 Prompt |
| `workbuddy-ask-prompt.tpl` | Ask 模式 Prompt |
| `workbuddy-craft-prompt.tpl` | Craft 模式 Prompt |
| `workbuddy-expert-prompt.tpl` | Expert 模式 Prompt |
| `workbuddy-craft-coding-prompt.tpl` | Craft 编程 Prompt |
| `workbuddy-ask-coding-prompt.tpl` | Ask 编程 Prompt |
| `workbuddy-expert-coding-prompt.tpl` | Expert 编程 Prompt |
| `workbuddy-craft-design-prompt.tpl` | Craft 设计 Prompt |
| `system-reminder.tpl` | 系统提醒 |
| `ask-mode-reminder.tpl` | Ask 模式提醒 |
| `craft-mode-reminder.tpl` | Craft 模式提醒 |
| `user-context-identity.tpl` | 用户身份上下文 |
| `user-context-expert-identity.tpl` | 专家身份上下文 |

支持 8 种回复风格（`style/`）：Creative、Efficient、Friendly、Professional、Sarcastic、Socratic、Straightforward。

#### 2.2.7 变量系统（Context Injection）

| 变量 | 类型 | 说明 |
|------|------|------|
| `file` | file | 当前文件上下文 |
| `git` | git | Git 状态上下文 |
| `knowledge` | knowledge | 知识库上下文 |
| `terminal` | terminal | 终端输出上下文 |
| `agent` | topic | Agent 主题上下文 |
| `rules` | rules | 项目规则上下文 |

#### 2.2.8 安全机制

- **沙箱执行**：`@anthropic-ai/sandbox-runtime` + `@genie/sandbox-cli` 提供命令执行沙箱
- **权限审批**：每个文件操作/命令执行前需用户审批
- **安全删除**：`genie-trash` 实现安全删除（类似回收站）
- **企业策略**：`@genie/enterprise-policy` 支持企业级安全策略管控

#### 2.2.9 认证与部署类型

支持 4 种部署模式：

| 部署类型 | 配置文件 | 说明 |
|---------|---------|------|
| SaaS | `product.json` | 公有云 SaaS |
| Internal | `product.internal.json` | 腾讯内部 OA |
| IOA | `product.ioa.json` | IOA 办公网 |
| Cloud Hosted | `product.cloudhosted.json` | 云托管 |
| Self Hosted | `product.selfhosted.json` | 私有化部署 |

认证方式：基于 CLI External Link 的 OAuth 流程，支持 TencentCloud SSO。

---

## 3. OpenAI Agents SDK 深度解析

### 3.1 概述

OpenAI Agents SDK 是 OpenAI 开源的 agent 构建框架，有 Python 和 TypeScript/JavaScript 两个版本。WorkBuddy 使用的是 **TypeScript 版本**（`@openai/agents` v0.5.2 + `@openai/agents-core` v0.5.2）。

- **仓库**：`openai/openai-agents-python` / `openai/openai-agents-js`
- **许可证**：Apache 2.0
- **定位**：一个**库/框架**——你自己用它构建 agent，它不是成品

### 3.2 核心架构

```
┌──────────────────────────────────────────────────────┐
│                 OpenAI Agents SDK                      │
├──────────────────────────────────────────────────────┤
│                                                        │
│  ┌──────────────────────────────────────────────────┐ │
│  │                    Runner                         │ │
│  │  Agent 循环引擎：输入 → LLM → Tool → 循环 → 输出   │ │
│  └──────────────────────┬───────────────────────────┘ │
│                         │                              │
│  ┌──────────────────────┼───────────────────────────┐ │
│  │                      │                            │ │
│  │  ┌──────────┐  ┌─────┴──────┐  ┌──────────────┐  │ │
│  │  │  Agent   │  │   Tools    │  │  Guardrails  │  │ │
│  │  │ 智能体   │  │  工具封装  │  │  输入/输出护栏│  │ │
│  │  │          │  │            │  │              │  │ │
│  │  │·instructions│·function_tool│·input_guardrail│ │ │
│  │  │·model    │  │·handoff    │  │·output_guardrail│ │
│  │  │·tools    │  │·MCP server │  │              │  │ │
│  │  │·handoffs │  │·hosted tool│  │              │  │ │
│  │  └──────────┘  └────────────┘  └──────────────┘  │ │
│  │                                                    │ │
│  │  ┌──────────┐  ┌────────────┐  ┌──────────────┐  │ │
│  │  │ Handoff  │  │  Tracing   │  │  Streaming   │  │ │
│  │  │ 转交     │  │  追踪      │  │  流式输出    │  │ │
│  │  │          │  │            │  │              │  │ │
│  │  │Agent A→B │  │OpenTelemetry│  │SSE / Stream  │  │ │
│  │  └──────────┘  └────────────┘  └──────────────┘  │ │
│  └──────────────────────────────────────────────────┘ │
│                                                        │
│  ┌──────────────────────────────────────────────────┐ │
│  │              Model Providers                      │ │
│  │  OpenAI · 任意 OpenAI 兼容 API · 自定义 Provider  │ │
│  └──────────────────────────────────────────────────┘ │
└──────────────────────────────────────────────────────┘
```

### 3.3 核心功能详解

#### 3.3.1 Agent（智能体）

Agent 是 SDK 的核心抽象，它将 LLM 与 instructions（系统提示词）和 tools（工具集）绑定：

```typescript
// 概念示例
const agent = new Agent({
  name: "MyAgent",
  instructions: "You are a helpful assistant",
  model: "gpt-4o",
  tools: [myTool1, myTool2],
});
```

Agent 可以配置：
- **instructions**：系统提示词，支持静态字符串或动态函数
- **model**：使用的 LLM 模型
- **tools**：可调用的工具列表
- **handoffs**：可转交的子 Agent 列表
- **output_type**：结构化输出类型（JSON Schema）

#### 3.3.2 Runner（运行器）

Runner 是 Agent 循环的执行引擎，实现完整的 **ReAct 循环**：

```
1. 接收用户输入
2. 构建 messages（system + history + user）
3. 调用 LLM
4. 检查输出：
   ├── 如果是文本 → 结束，返回给用户
   ├── 如果是 tool_call → 执行 tool，把结果追加到 messages
   └── 如果是 handoff → 切换到目标 Agent，继续循环
5. 回到步骤 3（直到 LLM 产出最终文本或达到最大轮次）
```

```typescript
// 概念示例
const result = await Runner.run(agent, "What is the weather?");
console.log(result.finalOutput);
```

Runner 负责：
- 自动处理 multi-turn tool calling 循环
- 错误处理与重试
- 上下文窗口管理
- 中间件/钩子支持

#### 3.3.3 Tool（工具）

SDK 提供多种工具定义方式：

| 方式 | 说明 |
|------|------|
| `function_tool` | 装饰器，自动从函数签名生成 JSON Schema |
| `Handoff` | 将 Agent 转交封装为 tool call |
| `MCP Server` | 通过 MCP 协议连接外部 tool server |
| `Hosted Tool` | 远程托管的 tool（通过 HTTP） |

```typescript
// 概念示例
const getWeather = functionTool(
  async (city: string) => {
    return `The weather in ${city} is sunny`;
  },
  {
    name: "get_weather",
    description: "Get the weather for a city",
    parameters: { city: z.string() },
  }
);
```

#### 3.3.4 Handoff（转交）

Handoff 是 SDK 实现多 Agent 协作的核心机制。Agent A 可以将对话"转交"给 Agent B：

```typescript
// 概念示例
const billingAgent = new Agent({
  name: "Billing",
  instructions: "You handle billing questions",
});

const mainAgent = new Agent({
  name: "Main",
  instructions: "You are a general assistant",
  handoffs: [billingAgent], // 可以转交给 Billing Agent
});
```

Handoff 本身是一个 tool call——当 LLM 判断需要转交时，Runner 自动切换到目标 Agent，保持对话历史完整。

#### 3.3.5 Guardrails（护栏）

SDK 提供输入/输出护栏机制：

| 类型 | 触发时机 | 用途 |
|------|---------|------|
| Input Guardrail | 用户输入到达 Agent 前 | 过滤敏感内容、注入额外上下文 |
| Output Guardrail | Agent 产出回复后 | 校验回复合规性、阻断不安全输出 |

Guardrail 可以返回三种结果：
- **allow**：放行
- **block**：阻断（返回错误信息）
- **tripwire**：触发告警但放行

#### 3.3.6 Tracing（追踪）

SDK 内置 OpenTelemetry 追踪，自动记录每次 agent 运行的完整调用链：

```
┌─ Run: "checkout-flow"
│  ├─ Agent: MainAgent
│  │  ├─ LLM Call (gpt-4o) → tool_call: get_cart
│  │  ├─ Tool: get_cart → result
│  │  ├─ LLM Call (gpt-4o) → handoff: BillingAgent
│  │  ├─ Agent: BillingAgent
│  │  │  ├─ LLM Call (gpt-4o) → tool_call: process_payment
│  │  │  ├─ Tool: process_payment → success
│  │  │  └─ LLM Call (gpt-4o) → final: "Payment complete"
│  │  └─ final: "Payment complete"
```

每条 span 记录：模型名称、token 消耗、延迟、tool 调用参数/返回值。

#### 3.3.7 Streaming（流式输出）

支持 SSE 流式输出，包括：
- 文本增量事件（delta）
- tool call 事件
- agent 切换事件
- 完成事件

### 3.4 SDK 的"不提供"清单

OpenAI Agents SDK 是框架，以下功能**需要你自己实现**：

| 缺失功能 | 说明 |
|---------|------|
| **UI/交互层** | 没有聊天界面，没有终端 UI |
| **记忆系统** | 没有持久记忆，没有跨会话状态 |
| **Prompt 管理** | 没有模板引擎，Agent instructions 需要硬编码 |
| **安全沙箱** | 没有命令执行审批，没有文件操作保护 |
| **Tool 生态** | 只有 MCP 连接器，不内置任何具体 tool |
| **多模型路由** | 默认只支持 OpenAI API，其他模型需自己写 Provider |
| **Skill 系统** | 没有可插拔的 skill 机制 |
| **Hook 系统** | 没有生命周期钩子 |
| **IDE 集成** | 没有 LSP 集成，没有代码编辑能力 |
| **对话管理** | 没有 session 管理，没有对话历史持久化 |

---

## 4. Claude Code 深度解析

### 4.1 概述

Claude Code 是 Anthropic 推出的 AI 编程助手产品，以 **CLI 工具** 形式提供。与 OpenAI Agents SDK 不同，Claude Code 不是框架，而是**开箱即用的完整产品**。

- **定位**：AI 编程助手（AI Coding Agent）
- **形态**：CLI 工具 + IDE 插件（VS Code / JetBrains）+ 桌面应用
- **模型**：Claude 系列专属（Opus 5、Sonnet 5、Fable 5、Haiku 4.5）
- **许可证**：闭源产品

### 4.2 产品形态

Claude Code 提供三种使用方式：

| 形态 | 说明 |
|------|------|
| **CLI** | 终端直接运行 `claude`，交互式 AI 编程助手 |
| **IDE 插件** | VS Code / JetBrains 扩展，编辑器内使用 |
| **桌面应用** | Mac/Windows 桌面应用 |
| **Web 应用** | claude.ai/code 网页版 |

### 4.3 核心能力体系

```
┌──────────────────────────────────────────────────────────┐
│                     Claude Code CLI                        │
├──────────────────────────────────────────────────────────┤
│                                                            │
│  ┌──────────────────────────────────────────────────────┐ │
│  │               Agent Loop (内置)                       │ │
│  │  Runner 循环：输入 → Claude → Tool Call → 结果 → 循环 │ │
│  └──────────────────────────────────────────────────────┘ │
│                                                            │
│  ┌──────────────────────────────────────────────────────┐ │
│  │              内置 Tool 体系（30+）                    │ │
│  │                                                        │
│  │  File:     Read • Write • Edit • Glob • Grep          │ │
│  │  Shell:    Bash (沙箱化)                              │ │
│  │  Agent:    Agent(fork) • Task • TaskList • TaskUpdate │ │
│  │  Web:      WebSearch • WebFetch                       │ │
│  │  MCP:      MCP Server 集成                            │ │
│  │  Skills:   Skill (内置 Slash Command)                  │ │
│  │  Cron:     CronCreate • CronList • CronDelete         │ │
│  │  Memory:   Read/Write (MEMORY.md)                      │ │
│  │  Plan:     EnterPlanMode • ExitPlanMode               │ │
│  │  Worktree: EnterWorktree • ExitWorktree               │ │
│  │  Notebook: NotebookEdit                               │ │
│  │  Review:   ReportFindings                             │ │
│  └──────────────────────────────────────────────────────┘ │
│                                                            │
│  ┌──────────────────────────────────────────────────────┐ │
│  │              高级功能                                 │ │
│  │                                                        │
│  │  • Memory System (MEMORY.md + YAML frontmatter)       │ │
│  │  • Subagent System (fork 子 Agent)                    │ │
│  │  • Workflow System (多 Agent 编排)                    │ │
│  │  • Plan Mode (先计划后执行)                           │ │
│  │  • Skill System (可插拔技能)                          │ │
│  │  • Hook System (生命周期钩子)                         │ │
│  │  • MCP Protocol (工具协议)                            │ │
│  │  • Sandbox (安全审批)                                 │ │
│  │  • Cron (定时任务)                                    │ │
│  │  • Worktree (Git 隔离)                                │ │
│  │  • Context Compaction (上下文压缩)                    │ │
│  │  • CLAUDE.md (项目级指令)                             │ │
│  │  • Structured Output (结构化输出)                     │ │
│  │  • Streaming (流式输出)                               │ │
│  └──────────────────────────────────────────────────────┘ │
│                                                            │
│  ┌──────────────────────────────────────────────────────┐ │
│  │              模型层                                   │ │
│  │  Claude Opus 5 • Claude Sonnet 5 • Claude Fable 5     │ │
│  │  Claude Haiku 4.5                                      │ │
│  └──────────────────────────────────────────────────────┘ │
└──────────────────────────────────────────────────────────┘
```

### 4.4 关键特性详解

#### 4.4.1 Memory System（记忆系统）

Claude Code 的记忆系统基于文件，支持持久化跨会话记忆：

```
.claude/memory/
├── MEMORY.md          ← 记忆索引（每会话加载）
├── user-profile.md    ← 用户信息（YAML frontmatter）
├── project-context.md ← 项目上下文
└── feedback-*.md      ← 用户反馈/偏好
```

每条记忆以 YAML frontmatter 标注元数据（type、description），支持 `[[wikilink]]` 关联。

#### 4.4.2 Subagent System（子 Agent 系统）

通过 `Agent` tool 派生子 Agent，支持：
- 不同类型的子 Agent（general-purpose、code-reviewer 等）
- Schema 约束（结构化输出）
- Worktree 隔离（独立 Git 工作区）
- 后台运行 + 结果通知

#### 4.4.3 Workflow System（工作流编排）

通过 `Workflow` tool 实现多 Agent 编排，使用 JavaScript DSL：

```
Pipeline 模式（默认）：
  Items → Stage1 → Stage2 → Stage3
  （每个 item 独立流水线，Stage 间无 barrier）

Parallel 模式：
  Items → [Agent1, Agent2, Agent3] → Merge
  （barrier 等待所有 Agent 完成）

常用模式：
  • Adversarial Verify（对抗验证）
  • Judge Panel（评委会）
  • Loop-until-dry（收敛循环）
  • Multi-modal sweep（多模态扫描）
  • Completeness critic（完整性审查）
```

#### 4.4.4 Plan Mode（计划模式）

`EnterPlanMode` / `ExitPlanMode` 实现"先计划，再执行"的工作流，计划经用户审批后才开始写代码。

#### 4.4.5 Sandbox（安全沙箱）

命令执行前经过权限审批，支持：
- 文件路径白名单
- 操作类型分级（读/写/执行/网络）
- 用户审批流程
- 危险命令拦截

#### 4.4.6 Hook System（钩子系统）

支持生命周期钩子，用户在特定事件触发自定义逻辑。

#### 4.4.7 MCP 集成

原生支持 Model Context Protocol，可接入外部 MCP server 扩展 tool 能力。

### 4.5 Claude Code 的"不提供"清单

| 缺失功能 | 说明 |
|---------|------|
| **多模型支持** | 只支持 Claude 系列模型 |
| **开源** | 闭源产品，不可自行修改 |
| **SDK/库化** | 不可作为 npm 包引入其他项目 |
| **自定义 Agent 循环** | Agent 循环是内置的，不可替换 |
| **自托管** | 不可私有化部署 |
| **Python 运行时** | 不内置 Python，tool 执行依赖系统环境 |

### 4.6 Claude Code 有没有 SDK？

**Claude Code 本身没有独立的 "SDK" 概念**。它和 OpenAI Agents SDK 不是同一类东西：

- **OpenAI Agents SDK** = 框架，你用它构建 agent
- **Claude Code** = 产品，你直接使用它

Anthropic 提供给开发者的编程接口是：

| 接口 | 说明 |
|------|------|
| **Claude API** (Messages API) | 通用 LLM API，不是 agent 框架 |
| **MCP** (Model Context Protocol) | 开放协议，用于 tool 集成 |
| **Claude Code CLI** | 命令行工具，可被脚本调用 |
| **Agent SDK** (可能) | Anthropic 可能在未来推出独立的 agent 框架 |

---

## 5. 三者对比分析

### 5.1 定位对比

```
                     框架                       产品
                   ←───────┬──────────────────────→

OpenAI Agents SDK          WorkBuddy              Claude Code
(你用它造 agent)      (半闭源 agent 产品)      (闭源 agent 产品)
```

### 5.2 架构对比

| 维度 | OpenAI Agents SDK | WorkBuddy | Claude Code |
|------|-------------------|-----------|-------------|
| **定位** | Agent 框架/库 | Agent 桌面产品 | Agent 编程助手 |
| **开源** | ✅ Apache 2.0 | ❌ 闭源 | ❌ 闭源 |
| **语言** | Python / TypeScript | TypeScript (Node.js) | 未公开（推测 Rust/Go） |
| **Agent 引擎** | `Runner` 类 | `@openai/agents-core` Runner | 内置自研 Runner |
| **模型** | OpenAI + 自定义 Provider | 20+ 国产模型 | Claude 系列 |
| **UI** | 无 | Electron 桌面 + CLI | CLI + IDE 插件 + 桌面 |
| **私有化部署** | 任意 | 支持（Self Hosted） | ❌ |
| **MCP** | 通过 SDK 接入 | 原生 `@modelcontextprotocol/sdk` v1.29.0 | 原生支持 |
| **内置 Tool** | 0（需自建） | Rich（文件/Shell/Web/Agent） | Rich（30+ 种） |
| **Skill 系统** | 无 | ✅ 内置 Skill 市场 | ✅ 内置 Slash Command |
| **记忆系统** | 无 | ✅ MEMORY.md | ✅ MEMORY.md |
| **多 Agent** | Handoff 模式 | Handoff + Subagent | Subagent + Workflow |
| **安全沙箱** | 无 | ✅ 审批 + 沙箱 | ✅ 审批 + 沙箱 |
| **Plan Mode** | 无 | ✅ Ask/Craft/Plan 三模式 | ✅ Plan Mode |
| **Hook** | 无 | ✅ 生命周期钩子 | ✅ 生命周期钩子 |
| **LSP 集成** | 无 | ✅ vscode-languageserver-protocol | ✅ 内置 |
| **终端** | 无 | ✅ node-pty + Ghostty | ✅ 内置 |
| **Python 运行时** | 无 | ✅ 内置 Python 3.13 | ❌ 依赖系统环境 |
| **Node.js 运行时** | — | ✅ 内置 Node.js v22 | ❌ 依赖系统环境 |
| **Git 运行时** | 无 | ✅ 内置 Git for Windows | ❌ 依赖系统环境 |
| **Tracing** | OpenTelemetry | Galileo (腾讯自研) | 未公开 |

### 5.3 技术选型对照

| 组件 | OpenAI Agents SDK | WorkBuddy | Claude Code |
|------|-------------------|-----------|-------------|
| Agent 循环 | `Runner.run()` | 基于 `@openai/agents-core` Runner | 自研 Runner |
| Tool 系统 | `@function_tool` 装饰器 | 自定义 Tool Registry | 内置 Tool 注册表 |
| LLM 调用 | `openai` SDK | `openai` SDK + 多 Provider 路由 | 自研 Claude API 客户端 |
| 通信协议 | 无内置 | Agent Client Protocol + JSON-RPC | 未公开 |
| 打包 | npm 包 | rspack → 6.9MB 单文件 | 原生二进制 |
| 插件系统 | MCP Server | MCP + Skill 市场 | MCP + Slash Command |
| 上下文压缩 | 无 | 内置 `compact-agent` | 内置 Context Compaction |

### 5.4 核心差异总结

#### OpenAI Agents SDK 的"空白"与 WorkBuddy 的"填充"

WorkBuddy 本质上是在 OpenAI Agents SDK 之上，**补齐了所有 SDK 缺失的模块**，构建了一个完整的 agent 产品。对照关系如下：

| SDK 缺失 | WorkBuddy 实现 |
|---------|---------------|
| 无 UI | Electron 桌面 + Ink (React) CLI |
| 无记忆 | `@genie/core` 记忆系统 + MEMORY.md |
| 无 Prompt 管理 | `@genie/prompts` + Jinja2 模板引擎 |
| 无多模型路由 | `@genie/product` 多 Provider 路由 |
| 无沙箱 | `@genie/sandbox-cli` + `@anthropic-ai/sandbox-runtime` |
| 无 LSP | `vscode-languageserver-protocol` |
| 无终端 | `@lydell/node-pty` + `@xterm/headless` + Ghostty WASM |
| 无代码搜索 | `ripgrep` + `tree-sitter` |
| 无 Skill 系统 | `builtin-skills/` + `marketplace-skill-installer` |
| 无 Hook | `@genie/core` Hook 系统 |
| 无认证 | `@genie/authentication` SSO |
| 无遥测 | `@genie/telemetry` + Galileo |

#### Claude Code 与 WorkBuddy 的差异化

| 维度 | Claude Code 优势 | WorkBuddy 优势 |
|------|-----------------|---------------|
| 模型 | Claude 专属，深度优化 | 20+ 国产模型，灵活选择 |
| 开源 | 闭源 | 闭源但可私有化部署 |
| 内置运行时 | 无，依赖系统环境 | 内置 Node/Python/Git 三合一 |
| 中国市场 | 需代理访问 | 原生支持，无网络障碍 |
| 生态 | Anthropic 生态 | 腾讯生态（微信支付、腾讯文档、CloudStudio） |
| 多 Agent | Workflow DSL 更强 | Subagent + Handoff 双模式 |
| 记忆 | 文件级 MEMORY.md | 同 + 企业知识库集成 |

### 5.5 选择建议

```
┌─────────────────────────────────────────────────────────────────┐
│                      选择决策树                                  │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  你要自己构建 agent 产品？                                        │
│  ├── 是 → 选 OpenAI Agents SDK（框架）                           │
│  │        • 需要自己实现 UI/Tool/记忆/安全/多模型...              │
│  │        • 最大灵活性，完全控制                                  │
│  │        • 适合：创业公司、AI 平台、定制化需求                    │
│  │                                                               │
│  └── 否，想直接使用 agent 产品？                                  │
│      ├── 在中国，需要国产模型和私有化？                           │
│      │   → 选 WorkBuddy                                          │
│      │     • 内置 Node/Python/Git 三运行时                        │
│      │     • 微信支付、腾讯文档、CloudStudio 生态                 │
│      │     • 支持私有化部署                                       │
│      │                                                           │
│      └── 在海外，需要顶级编程体验？                               │
│          → 选 Claude Code                                        │
│            • Claude 模型深度优化，代码质量高                       │
│            • Workflow 多 Agent 编排强大                           │
│            • 记忆/Hook/Skill 系统成熟                             │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

---

## 6. 附录：解包分析过程

### 6.1 安装包解包

```
123.exe (389 MB, NSIS v3.11 自解压安装包)
  └── $PLUGINSDIR/app-64.7z (388 MB, LZMA2 压缩)
        └── 解压后: 1.1 GB, 320 个目录, 1123 个文件
```

### 6.2 关键文件清单

| 文件 | 大小 | 说明 |
|------|------|------|
| `WorkBuddy.exe` | 205 MB | Electron 主可执行文件 |
| `resources/app.asar` | 279 MB | Electron 应用代码 |
| `resources/app.asar.unpacked/` | — | 原生模块（node-pty, koffi, better-sqlite3） |
| `resources/app.asar.unpacked/cli/dist/codebuddy.js` | 6.9 MB | Agent 核心（rspack bundle） |
| `resources/app.asar.unpacked/cli/product.json` | 352 KB | 产品配置（模型/Agent/Tool 定义） |
| `vendor/PortableGit.zip` | 51 MB | Git for Windows 运行时 |
| `vendor/node.zip` | 36 MB | Node.js v22.22.2 运行时 |
| `vendor/python.zip` | 19 MB | Python 3.13 运行时 |
| `resources/builtin-skills/` | — | 内置 Skill 库（18 个） |
| `resources/builtin-plugins/` | — | 内置 MCP 插件（3 个） |
| `resources/templates/` | — | Agent Prompt 模板（13 个） |

### 6.3 解包命令参考

```bash
# 安装 7z
sudo apt-get install -y p7zip-full

# 解包 NSIS 安装器
7z x 123.exe -oextracted

# 解包内层 7z 压缩包
7z x extracted/\$PLUGINSDIR/app-64.7z -oextracted/app

# 查看 asar 内容
npx @electron/asar list extracted/app/resources/app.asar

# 提取 asar
npx @electron/asar extract extracted/app/resources/app.asar extracted/asar-contents
```

