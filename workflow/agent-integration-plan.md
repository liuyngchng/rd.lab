# Claude Code Agent 集成 CDDR-GCS 方案

> 作者：然然客服团队 | 日期：2026-08-10

---

## 1. 背景与动机

### 1.1 两套系统的能力对比

```
CDDR-GCS (Go)                          Claude Code Agent
─────────────────                      ──────────────────
✅ 意图分类 (67ms, 四层级联)           ✅ 多步推理 (读文件→思考→再读)
✅ 紧急抢占 (0ms, 硬编码)              ✅ 知识库自主导航
✅ VDB 检索 + Rerank                   ✅ 严格按脚本分步执行
✅ 会话管理 + 鉴权                     ✅ 处理模糊/开放问题
✅ 管理后台 + 热更新                   ✅ "等客户回复→下一步"循环
✅ 集群部署 + 高并发                   ✅ 工具调用 (Read/WebFetch)
❌ 回复质量受限 (prompt + chunk 拼贴)  ❌ 延迟高 (秒级)
❌ 不懂分步交互 (一问一答)             ❌ 成本高 (token消耗大)
❌ 无法自主探索知识库                  ❌ 无分类器 (每次都要想)
```

**CDDR-GCS 强在"快和准"，Claude Code Agent 强在"深和活"。两者天然互补。**

### 1.2 为什么要集成

燃气客服场景中，**维修类和综合咨询类**问题有很强的分步交互需求。例如知识库里的脚本：

```
步骤1: 检查燃气表是否欠费 → 等客户回复
步骤2: 检查阀门是否关闭 → 等客户回复
步骤3: 检查电池是否有电 → 等客户回复
步骤4: 仍未解决 → 转人工
```

当前 CDDR-GCS 的 LLM 回答层（`csm.go` 的 `csmStream`）是**一次性问答**：VDB 检索 → 拼 prompt → LLM 生成全量回答。它不会分步、不会等待、不会根据客户反馈调整。

Claude Code Agent 天然适合这种"有状态的多步对话"。本方案将 Agent 作为 CDDR-GCS 的**深度推理引擎**接入。

---

## 2. 整体架构

### 2.1 分层混合模型

```
用户请求
  │
  ▼
┌──────────────────────────────────────────────────────┐
│ CDDR-GCS (Go) — 快速路径，不动                        │
│                                                      │
│ C³-Classifier (keyword → fastText → embed → LLM)     │
│        │                                             │
│        ▼                                             │
│ CDMIR 四规则路由 (emergency抢占/歧义反问/正常/次意图) │
│        │                                             │
│        ▼                                             │
│ VDB 知识库检索                                       │
│        │                                             │
│        ├── 80% 请求在此结束 ──▶ 原有 LLM 流式回答     │
│        │   (keyword + fastText 高置信度)              │
│        │                                             │
│        └── 20% 低置信/复杂 ──▶ Claude Code Agent      │
│            (fallback / repair低分 / 多意图歧义)       │
└──────────────────────────────────────────────────────┘
```

### 2.2 触发条件（渐进式上线）

| 阶段 | 触发条件 | 预估请求占比 | 风险 | 上线时间 |
|------|---------|:-----------:|------|---------|
| **Phase 1** | C³-Classifier fallback 层命中 | ~3% | 极低 | 第1周 |
| **Phase 2** | repair / faq 意图 + 置信度 < 0.85 | ~15% | 低 | 第2周 |
| **Phase 3** | 所有分支 low-confidence + 歧义反问后 | ~20% | 中 | 待评估 |

### 2.3 部署拓扑

```
                    ┌──────────────────────┐
    浏览器 ── SSE ──▶│  CDDR-GCS (Go)      │
                    │  :3001               │
                    │                      │
                    │  ├─ C³-Classifier    │
                    │  ├─ CDMIR Router     │
                    │  ├─ VDB Search       │
                    │  ├─ 80%: LLM Stream  │
                    │  └─ 20%: HTTP ───────┼──▶ ┌──────────────────────┐
                    │       Agent Client   │    │  Agent Sidecar (Node) │
                    └──────────────────────┘    │  :3002               │
                                                │                      │
                                                │  Claude Code SDK     │
                                                │  + 自定义 prompt     │
                                                │  + Read(知识库)       │
                                                └──────────────────────┘
```

- Agent Sidecar 与 Go 服务部署在同机（内网通信 <5ms）
- Agent 不可用时自动降级：日志告警 + 回退原有 LLM

---

## 3. 组件详细设计

### 3.1 Agent Sidecar（新增 Node.js 服务）

**位置**：`kb-chat-flow/g/agent-sidecar/`

**目录结构**：

```
agent-sidecar/
├── package.json
├── tsconfig.json
├── src/
│   ├── index.ts                  # Express 服务入口，挂载路由
│   ├── agent-runner.ts           # 封装 @anthropic-ai/claude-agent-sdk
│   ├── types.ts                  # 请求/响应类型定义
│   ├── system-prompt.ts          # 客服 system prompt 构建
│   └── knowledge-base-paths.ts   # 意图 → 知识库文件映射
└── tests/
    └── agent-runner.test.ts
```

**核心 API**：

```
POST /agent/chat/stream
Content-Type: application/json

{
  "session_id": "user_123",
  "intent": "repair",
  "confidence": 0.62,
  "source": "fasttext",
  "user_query": "燃气灶打不着火，换了电池也不行",
  "kb_context": "[VDB检索到的知识条目...]",
  "kb_file_paths": [
    "../其他知识/燃气灶打不着火.md",
    "../其他知识/然然客服-知识库.md"
  ],
  "history": [
    {"role": "user", "content": "打不着火"},
    {"role": "assistant", "content": "请检查电池..."},
    {"role": "user", "content": "换了，还是不行"}
  ],
  "working_dir": "/home/rd/doc/gas_customer_service_knowledge_base",
  "allowed_tools": ["Read"],
  "extra_directories": ["/home/rd/doc/gas_customer_service_knowledge_base"]
}

→ 响应: SSE 流 (text/event-stream)
  data: {"type":"chunk","content":"好的亲，"}
  data: {"type":"chunk","content":"既然换了电池还是不行，"}
  ...
  data: {"type":"done"}
```

**关键实现细节**：

1. **SDK 调用方式**（参考 rdCLI 的 `claude-runtime.provider.js:619`）：
   ```ts
   import { query } from '@anthropic-ai/claude-agent-sdk';
   
   const q = query({
     prompt: buildPrompt(req),
     options: {
       systemPrompt: { type: 'custom', text: buildSystemPrompt(req) },
       allowedTools: req.allowed_tools,
       permissionMode: 'bypassPermissions',
       cwd: req.working_dir,
       settingSources: ['project', 'user', 'local'],
       // 不设 preset: 'claude_code'，完全自定义
     }
   });
   ```

2. **System Prompt 结构**：
   - **固定部分**：然然人设、分步响应规则、安全优先规则、转人工规则（复用之前写的 CLAUDE.md）
   - **动态部分**：当前意图类别 + VDB 检索结果 + 软化提示（低置信度时）
   - **禁止行为**：不要提及文件读取、知识库查询等后台操作

3. **工具限制**：`allowedTools: ["Read"]`，且 Read 被限制在 `extra_directories` 范围内

### 3.2 Go 端改动

#### 3.2.1 新增文件：`internal/agent/client.go`

封装对 Agent Sidecar 的 HTTP/SSE 调用：

```go
package agent

type Client struct {
    baseURL    string
    httpClient *http.Client
}

type Request struct {
    SessionID    string        `json:"session_id"`
    Intent       string        `json:"intent"`
    Confidence   float64       `json:"confidence"`
    Source       string        `json:"source"`
    UserQuery    string        `json:"user_query"`
    KBContext    string        `json:"kb_context"`
    KBFilePaths  []string      `json:"kb_file_paths"`
    History      []ChatMessage `json:"history"`
    WorkingDir   string        `json:"working_dir"`
    AllowedTools []string      `json:"allowed_tools"`
    ExtraDirs    []string      `json:"extra_directories"`
}

// StreamChat 流式调用 Agent，返回 chunk channel + error channel
func (c *Client) StreamChat(ctx context.Context, req *Request) (<-chan string, <-chan error) {
    // 1. POST JSON → Agent sidecar
    // 2. 读取 SSE 响应流
    // 3. 解析每行 "data: {...}"
    // 4. 转发到 chunk channel
}

// Health 检查 Agent 是否可用
func (c *Client) Health(ctx context.Context) bool
```

#### 3.2.2 修改文件：`internal/engine/csm.go`

**新增决策函数**：

```go
// shouldUseAgent 判断当前请求是否应路由到 Claude Code Agent
func (e *Engine) shouldUseAgent(intent model.IntentType, confidence float64, source model.ClassifySource) bool {
    if e.agentClient == nil {
        return false
    }
    // Phase 1: fallback 层 → 始终走 Agent
    if source == model.SourceFallback {
        return true
    }
    // Phase 2: repair/faq + 低置信度 → 走 Agent
    if (intent == model.IntentRepair || intent == model.IntentFaq) && confidence < 0.85 {
        return true
    }
    return false
}
```

**修改路由分支**（在 `csmRun()` 中的 `switch primary.Intent` 部分）：

```go
case model.IntentRepair:
    if e.shouldUseAgent(primary.Intent, primary.Confidence, primary.Source) {
        e.csmAnswerWithAgent(eventCh, primary, userQuery, uid, historyMsgs)
    } else {
        e.csmAnswerWithKB(eventCh, "维修检索", "维修客服", prompt, userQuery, uid, e.repairVdbIDsSnapshot())
    }

case model.IntentFaq:
    if e.shouldUseAgent(primary.Intent, primary.Confidence, primary.Source) {
        e.csmAnswerWithAgent(eventCh, primary, userQuery, uid, historyMsgs)
    } else {
        e.csmAnswerWithKB(eventCh, "FAQ检索", "综合FAQ", prompt, userQuery, uid, e.faqVdbIDsSnapshot())
    }
```

**新增 Agent 回答方法**：

```go
func (e *Engine) csmAnswerWithAgent(
    eventCh chan<- EngineEvent,
    primary model.ClassifiedIntent,
    userQuery string, uid string,
    historyMsgs []ChatMsg,
) {
    eventCh <- EngineEvent{Type: "progress", Step: 1, Total: csmTotalStep,
        Agent: "Agent 深度推理"}

    // 1. 检索知识库
    vdbIDs := e.getVdbIDsForIntent(primary.Intent)      // 新方法
    kbContext := e.csmSearchKB(userQuery, uid, vdbIDs)

    // 2. 确定知识库文件路径
    kbFilePaths := e.getKBFilePathsForIntent(primary.Intent) // 新方法

    // 3. 发请求
    req := &agent.Request{
        SessionID: uid, Intent: string(primary.Intent),
        Confidence: primary.Confidence, Source: string(primary.Source),
        UserQuery: userQuery, KBContext: kbContext,
        KBFilePaths: kbFilePaths, History: historyMsgs,
        WorkingDir: e.cfg.Agent.WorkingDir,
        AllowedTools: []string{"Read"},
        ExtraDirs: []string{e.cfg.Agent.KnowledgeBaseDir},
    }

    ctx := context.Background()
    chunkCh, errCh := e.agentClient.StreamChat(ctx, req)

    // 4. 转发流
    for {
        select {
        case chunk, ok := <-chunkCh:
            if !ok { return }
            eventCh <- EngineEvent{Type: "chunk", Content: chunk,
                Step: 2, Total: csmTotalStep}
        case err := <-errCh:
            if err != nil {
                slog.Error("agent failed, degrading to LLM", "error", err)
                // 降级到原有 LLM
                prompt := csmBranchPrompt(primary.Intent)
                e.csmStream(eventCh, "客服", prompt,
                    "用户问题："+userQuery+"\n检索信息："+kbContext)
                return
            }
        }
    }
}
```

#### 3.2.3 修改文件：`internal/model/types.go`

```go
// 新增 AgentConfig
type AgentConfig struct {
    Enabled          bool   `yaml:"enabled" json:"enabled"`
    SidecarURL       string `yaml:"sidecar_url" json:"sidecar_url"`
    WorkingDir       string `yaml:"working_dir" json:"working_dir"`
    KnowledgeBaseDir string `yaml:"knowledge_base_dir" json:"knowledge_base_dir"`
}
```

在 `Config` 结构体中添加字段：
```go
Agent AgentConfig `yaml:"agent"`
```

#### 3.2.4 修改文件：`cfg.yml.template`

```yaml
# ============================================================
# Claude Code Agent Sidecar 配置（可选）
# ============================================================
agent:
  enabled: false                           # 是否启用 Agent 增强
  sidecar_url: "http://127.0.0.1:3002"    # Agent Sidecar 地址
  working_dir: ""                          # Agent 工作目录（知识库根目录）
  knowledge_base_dir: ""                   # 允许 Agent Read 的知识库目录
```

#### 3.2.5 修改文件：`internal/engine/engine.go`

在 `Engine` 结构体中添加 `agentClient *agent.Client` 字段。

---

## 4. Agent System Prompt 设计

### 4.1 固定部分（角色 + 行为规则）

```
你是然然，中石油昆仑燃气有限公司"昆仑慧享+"平台的 AI 智能客服。
全国客服热线：956100。

## 行为规则

### 分步响应（最重要）
严格按知识库脚本逐步执行，每步只给出一条操作指引，
必须等待客户回复后再给下一步。禁止一次性输出所有步骤。

### 安全优先
涉及燃气泄漏时立即执行应急流程，不做任何其他操作。

### 转人工
知识库脚本指示转人工时、客户要求转人工时、问题无法解决时，
引导客户输入"人工客服"。

### 语气
称呼客户为"亲"或"您好"。亲切、耐心、专业。
```

### 4.2 动态部分（由 Go 端注入）

```
## 当前场景
意图类别：维修报修
分类置信度：0.62（低信心）
注意：用户描述可能不够清晰。如无法解答，请礼貌引导用户换个方式描述或转人工客服。

## 知识库参考
[VDB 检索到的相关条目]

## 可用知识库文件
- ../其他知识/燃气灶打不着火.md
- ../其他知识/然然客服-知识库.md
```

---

## 5. 会话管理对齐

| 关注点 | CDDR-GCS (Go) | Agent Sidecar |
|--------|--------------|---------------|
| 会话标识 | `session.UID` | `session_id` |
| 历史存储 | 进程内存/Redis | 无状态（每次请求传入） |
| 过期策略 | 30 分钟无活动清理 | 不管理生命周期 |
| 职责 | 会话 CRUD | 单次对话生成 |

Agent Sidecar 是**无状态的**——每次请求携带完整历史，不自行存储会话。

---

## 6. 知识库路径映射

Agent 需要知道"当前意图对应哪些知识库文件"，以便自主 Read。这个映射表维护在 Agent Sidecar 中：

```ts
// knowledge-base-paths.ts
export const INTENT_FILE_MAP: Record<string, string[]> = {
  emergency: [
    '../其他知识/然然客服-知识库.md',       // 应急类
  ],
  billing: [
    '../查询/查余额_气量.md',
    '../查询/查户号_表号 (1).md',
    '../其他知识/然然客服-知识库.md',
  ],
  business: [
    '../业务办理过户/',
    '../业务办理开户/',
    '../业务办理/',
  ],
  repair: [
    '../其他知识/燃气灶打不着火.md',
    '../维修/',
    '../其他知识/然然客服-知识库.md',
  ],
  faq: [
    '../线下营业厅知识/',
    '../气价查询/',
    '../其他知识/然然客服-知识库.md',
    '../其他知识/白屏.md',
    '../其他知识/保险业务.md',
  ],
};
```

---

## 7. 实施计划

### Step 1: Agent Sidecar 搭建（估计 1.5 天）

- [ ] 初始化 Node.js/TypeScript 项目
- [ ] 安装 `@anthropic-ai/claude-agent-sdk` + `express`
- [ ] 实现 `agent-runner.ts`：封装 SDK `query()`，自定义 system prompt
- [ ] 实现 `POST /agent/chat/stream` SSE 端点
- [ ] 实现 `GET /health`
- [ ] 单元测试

### Step 2: Go 端集成（估计 1 天）

- [ ] 新增 `internal/agent/client.go`：HTTP 客户端 + SSE 解析
- [ ] 扩展 `model/types.go`：`AgentConfig`
- [ ] 修改 `config/config.go`：加载 Agent 配置
- [ ] 修改 `csm.go`：`shouldUseAgent()` + `csmAnswerWithAgent()`
- [ ] 修改 `engine.go`：注入 `agentClient`
- [ ] 集成测试

### Step 3: 知识库映射（估计 0.5 天）

- [ ] 实现 `knowledge-base-paths.ts`
- [ ] 实现 Go 端 `getKBFilePathsForIntent()`

### Step 4: 联调与上线（估计 1 天）

- [ ] Phase 1 灰度（fallback 层 → Agent）
- [ ] 延迟和成本监控
- [ ] Phase 2 灰度（repair/faq 低分 → Agent）
- [ ] 降级熔断验证

---

## 8. 风险与对策

| 风险 | 概率 | 影响 | 对策 |
|------|:----:|:----:|------|
| Agent 延迟 >3s | 中 | 用户体验下降 | Phase 1 仅影响 3% 请求；设超时降级 |
| Agent 调用成本高 | 中 | 运营费用 | 限制 maxTokens；仅低置信度使用 |
| SDK 版本 breaking change | 低 | 服务不可用 | 锁定版本号；健康检查 + 自动降级 |
| Agent 回答质量不稳定 | 中 | 客户投诉 | 保留旧 LLM 路径；可按 `source` 标签 A/B 对比 |
| Go ↔ Node 通信失败 | 低 | Agent 路径不可用 | 健康检查 + 自动降级 + 告警 |
| 知识库文件路径变更 | 低 | Agent Read 失败 | 映射表维护在 Agent 侧，热更新 |

---

## 9. 关键决策记录

| 决策 | 选择 | 理由 |
|------|------|------|
| Agent 部署方式 | Sidecar 进程 | 与 Go 解耦，独立扩缩，故障隔离 |
| Agent 状态管理 | 无状态（每次传历史） | 简单，与 Go 端会话管理无冲突 |
| SDK 选用 | `@anthropic-ai/claude-agent-sdk` | rdCLI 已验证可行，文档完整 |
| system prompt 策略 | 自定义（不用 preset） | 需要燃气客服专属人设，不能是编程助手 |
| 工具权限 | 仅 `Read` | 安全优先，Agent 不应写文件或执行命令 |
| 降级策略 | 自动回退原 LLM | 保障可用性，Agent 不是强依赖 |

---

## 10. 成功指标

- [ ] Phase 1 上线后，3% 的请求走 Agent 且延迟 < 3s
- [ ] 人工评估 Agent 回答质量 ≥ 原有 LLM 回答质量
- [ ] Agent 可用性 ≥ 99.5%
- [ ] Agent 单次调用成本 ≤ 原有 LLM 的 2 倍（可接受，因为只影响 20% 请求）
- [ ] 降级触发率 < 1%（说明 Agent 稳定可靠）
