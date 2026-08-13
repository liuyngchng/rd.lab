# CloudCLI → rdCLI 品牌改名记录

> **目的**: 本文将本次全局品牌替换的细节完整记录，供后续编译、打包、运行时出现问题时排错参考。
>
> **替换日期**: 2026-08-10
>
> **替换规则**:
> - `CloudCLI` → `rdCLI`（大写驼峰，面向用户的产品名）
> - `CLOUDCLI` → `RDCLI`（全大写，环境变量 / localStorage Key 前缀）
> - `cloudcli` → `rdcli`（全小写，CLI 命令名、目录路径、IPC channel 等）
>
> **未改动的内容**:
> - `Claude` / `Claude Code` / `claude` — 底层依赖的 Claude Code CLI，保留原名
> - `siteboon/claudecodeui`（GitHub 仓库地址）— 尚未迁移 repo，保留原址
> - `@anthropic-ai/claude-agent-sdk` — npm 依赖，不可改动
> - `package-lock.json` — 由 npm 生成，未直接修改

---

## 一、改名影响的文件清单（共 77 个文件）

### 1.1 根目录

| 文件 | 改动项 | 原值 | 新值 |
|------|--------|------|------|
| `package.json` | `name` | `@cloudcli-ai/cloudcli` | `@rdcli-ai/rdcli` |
| | `productName` | `CloudCLI` | `rdCLI` |
| | `bin.cloudcli` | `dist-server/server/modules/cli/cli.js` | key 改为 `rdcli` |
| | `homepage` | `https://cloudcli.ai` | `https://rdcli.ai` |
| | `build.appId` | `ai.cloudcli.desktop` | `ai.rdcli.desktop` |
| | `build.productName` | `CloudCLI` | `rdCLI` |
| | `build.artifactName` | `cloudcli-desktop-*` | `rdcli-desktop-*` |
| | `build.protocols[0].name` | `CloudCLI` | `rdCLI` |
| | `build.protocols[0].schemes[0]` | `cloudcli` | `rdcli` |
| | `build.mac.extendInfo.CFBundleName` | `CloudCLI` | `rdCLI` |
| | `build.mac.extendInfo.CFBundleDisplayName` | `CloudCLI` | `rdCLI` |
| | `build.mac.extendInfo.CFBundleURLTypes[0].CFBundleURLName` | `CloudCLI` | `rdCLI` |
| | `build.mac.extendInfo.CFBundleURLTypes[0].CFBundleURLSchemes[0]` | `cloudcli` | `rdcli` |
| | `keywords` | 含 `cloudcli` | `rdcli` |
| | `author` | `CloudCLI UI Contributors` | `rdCLI UI Contributors` |
| | `description` | `A web-based UI for Claude Code CLI` | 未修改（Claude Code 保留） |
| | `repository.url` | `git+https://github.com/siteboon/claudecodeui.git` | 未修改 |
| | `bugs.url` | `https://github.com/siteboon/claudecodeui/issues` | 未修改 |
| `.release-it.json` | `github.releaseName` | `CloudCLI UI v${version}` | `rdCLI UI v${version}` |
| | `changelog header` | `CloudCLI UI` | `rdCLI UI` |

### 1.2 Electron 桌面端 (`electron/`)

| 文件 | 改动项 | 原值 | 新值 |
|------|--------|------|------|
| `main.js` | `APP_NAME` | `CloudCLI` | `rdCLI` |
| | `APP_USER_MODEL_ID` | `ai.cloudcli.desktop` | `ai.rdcli.desktop` |
| | `CALLBACK_PROTOCOL` | `cloudcli` | `rdcli` |
| | `CLOUDCLI_CONTROL_PLANE_URL` | 变量名 + 默认 URL `cloudcli.ai` | `RDCLI_CONTROL_PLANE_URL` + `rdcli.ai` |
| | `copyright` | `CloudCLI` | `rdCLI` |
| | `ipcMain.handle('cloudcli-desktop:*')` | 所有 IPC channel | `rdcli-desktop:*` |
| | `about panel` | `CloudCLI` | `rdCLI` |
| | 错误消息 | `CloudCLI` 系列文案 | `rdCLI` 系列文案 |
| | 域名引用 | `cloudcli.ai` | `rdcli.ai` |
| `desktopWindow.js` | 所有菜单项 | `Open Local CloudCLI` 等 | `Open Local rdCLI` 等 |
| | `cloudcli-desktop:state-updated` | IPC channel | `rdcli-desktop:state-updated` |
| | `cloudcli-desktop:launcher-command` | IPC channel | `rdcli-desktop:launcher-command` |
| | 域名匹配 | `cloudcli.ai` | `rdcli.ai` |
| | 连接状态文案 | `CloudCLI Connected/Connect CloudCLI` | `rdCLI Connected/Connect rdCLI` |
| `localServer.js` | `SERVER_MARKER_PATH` | `~/.cloudcli/local-server.json` | `~/.rdcli/local-server.json` |
| | `LOCAL_SERVER_URL_ENV_KEYS` | `CLOUDCLI_DESKTOP_LOCAL_SERVER_URL` 等 | `RDCLI_DESKTOP_LOCAL_SERVER_URL` 等 |
| | `LOCAL_SERVER_PORT_ENV_KEYS` | `CLOUDCLI_DESKTOP_LOCAL_SERVER_PORT` 等 | `RDCLI_DESKTOP_LOCAL_SERVER_PORT` 等 |
| | `getPendingTarget().name` | `Local CloudCLI` | `Local rdCLI` |
| | `CLOUDCLI_USE_INSTALLED_SERVER` | 环境变量 | `RDCLI_USE_INSTALLED_SERVER` |
| | 函数名 | `isCloudCliServer` | `isrdCLIServer` |
| | 函数名 | `waitForCloudCliServer` | `waitForrdCLIServer` |
| | 日志消息 | `CloudCLI desktop server exited` / `Local CloudCLI ready` | `rdCLI` 系列 |
| `cloud.js` | `getEnvironmentUrl()` | `cloudcli.ai` 域名 | `rdcli.ai` |
| | 错误消息 | `Connect your CloudCLI account` / `CloudCLI API` | `rdCLI` |
| | `buildConnectUrl()` | `app_surface=cloudcli_desktop` | `app_surface=rdcli_desktop` |
| | SSH 目标 | `ssh.cloudcli.ai` | `ssh.rdcli.ai` |
| `desktopNotifications.js` | 错误消息 | `Connect a CloudCLI account` | `Connect a rdCLI account` |
| `serverInstaller.js` | 注释 | `Used by CloudCLI Desktop` | `Used by rdCLI Desktop` |
| | `DEFAULT_INSTALL_ROOT` | `~/.cloudcli/server` | `~/.rdcli/server` |
| | `CLOUDCLI_SERVER_DIR` | 环境变量 | `RDCLI_SERVER_DIR` |
| | `CLOUDCLI_SERVER_BUNDLE_URL` | 环境变量 | `RDCLI_SERVER_BUNDLE_URL` |
| | `CLOUDCLI_SERVER_BUNDLE_RELEASE_TAG` | 环境变量 | `RDCLI_SERVER_BUNDLE_RELEASE_TAG` |
| | `getBundleName()` | `cloudcli-local-server-*` | `rdcli-local-server-*` |
| `viewHost.js` | 内部属性名 | `__cloudcliStartupHtml` 等 | `__rdcliStartupHtml` 等 |
| | 占位 HTML | `Starting Local CloudCLI...` | `Starting Local rdCLI...` |
| `preload.cjs` | 函数名 | `isCloudCliAppOrigin` | `isrdCLIAppOrigin` |
| | IPC channel 监听 | `cloudcli-desktop:*` | `rdcli-desktop:*` |
| | `contextBridge.exposeInMainWorld` | `cloudcliDesktopNotifications`, `cloudcliDesktop` | `rdcliDesktopNotifications`, `rdcliDesktop` |
| | 域名检查 | `cloudcli.ai` | `rdcli.ai` |
| `launcher/index.html` | `<title>` | `CloudCLI Desktop` | `rdCLI Desktop` |
| `launcher/launcher.js` | 全局对象访问 | `window.cloudcliDesktop` | `window.rdcliDesktop` |
| | Mock 数据 | `cloudcli.ai` 域名 | `rdcli.ai` |
| | UI 文案 | `Local CloudCLI` / `CloudCLI dashboard` 等 | `rdCLI` 系列 |
| | 状态文本 | `Your CloudCLI session expired` 等 | `Your rdCLI session expired` 等 |
| | IIFE 名称 | `cloudCliLauncher` | `rdCLILauncher` |
| | `<span>CloudCLI</span>` | 标题栏品牌 | `<span>rdCLI</span>` |

### 1.3 前端源码 (`src/`)

| 文件 | 改动项 | 原值 | 新值 |
|------|--------|------|------|
| `constants/branding.ts` | 导出常量 | `CLOUDCLI_WORDMARK_FONT_FAMILY` | `RDCLI_WORDMARK_FONT_FAMILY` |
| `hooks/useGitHubStars.ts` | localStorage Key | `CLOUDCLI_GITHUB_STARS` | `RDCLI_GITHUB_STARS` |
| | localStorage Key | `CLOUDCLI_HIDE_GITHUB_STAR` | `RDCLI_HIDE_GITHUB_STAR` |
| `hooks/useWebPush.ts` | 运行环境检测 | `window.cloudcliDesktopNotifications` | `window.rdcliDesktopNotifications` |
| `components/settings/view/Settings.tsx` | 同上 | `window.cloudcliDesktopNotifications` | `window.rdcliDesktopNotifications` |
| `components/settings/view/tabs/AboutTab.tsx` | import + 使用 | `CLOUDCLI_WORDMARK_FONT_FAMILY` | `RDCLI_WORDMARK_FONT_FAMILY` |
| `components/settings/view/tabs/api-settings/sections/VersionInfoSection.tsx` | import + 使用 | `CLOUDCLI_WORDMARK_FONT_FAMILY` | `RDCLI_WORDMARK_FONT_FAMILY` |
| `components/auth/view/AuthLoadingScreen.tsx` | import + 使用 | `CLOUDCLI_WORDMARK_FONT_FAMILY` | `RDCLI_WORDMARK_FONT_FAMILY` |
| `components/sidebar/view/subcomponents/SidebarHeader.tsx` | import + 使用 | `CLOUDCLI_WORDMARK_FONT_FAMILY` | `RDCLI_WORDMARK_FONT_FAMILY` |
| `components/chat/view/subcomponents/ChatMessageImages.tsx` | 注释 | `~/.cloudcli/assets` | `~/.rdcli/assets` |
| `components/mcp/view/McpServers.tsx` | 注释 + 函数名 | `cloudcli-` 前缀 | `rdcli-` 前缀 |
| | 函数 `isManagedServer` | `startsWith('cloudcli-')` | `startsWith('rdcli-')` |
| `components/plugins/view/PluginSettingsTab.tsx` | GitHub URL | `https://github.com/cloudcli-ai/*` | `https://github.com/rdcli-ai/*` |
| | Plugin IDs | `cloudcli-claude-watch` 等 | `rdcli-claude-watch` 等 |
| | 变量名 | `PRISM_CLOUDCLI_PLUGIN_URL` | `PRISM_RDCLI_PLUGIN_URL` |
| `stores/sessionMessageReconciliation.test.ts` | 测试路径 | `.cloudcli/assets/*` | `.rdcli/assets/*` |

### 1.4 i18n 多语言 (`src/i18n/locales/*/common.json`)

| 语言 | 改动 |
|------|------|
| en, es, ja, zh-CN, zh-TW, de, fr, it, ru, tr, ko | `npmUpgradeCommand` 值从 `@cloudcli-ai/cloudcli` 改为 `@rdcli-ai/rdcli` |

### 1.5 服务端 (`server/`)

| 文件 | 改动项 | 原值 | 新值 |
|------|--------|------|------|
| `index.ts` | 注释 | `~/.cloudcli/assets` | `~/.rdcli/assets` |
| | `LOCAL_SERVER_MARKER_PATH` | `~/.cloudcli/local-server.json` | `~/.rdcli/local-server.json` |
| | 启动提示 | `cloudcli status` | `rdcli status` |
| `load-env.ts` | `DEFAULT_DATABASE_PATH` | `~/.cloudcli/auth.db` | `~/.rdcli/auth.db` |
| `modules/cli/cli.service.ts` | CLI 帮助文本 | `cloudcli [command]` / `$ cloudcli --port` | `rdcli [command]` / `$ rdcli --port` |
| | 更新提示 | `npm update -g @cloudcli-ai/cloudcli` | `npm update -g @rdcli-ai/rdcli` |
| `modules/cli/cli.module.ts` | `defaultDatabasePath` | `~/.cloudcli/auth.db` | `~/.rdcli/auth.db` |
| | npm 版本检查 | `npm show @cloudcli-ai/cloudcli version` | `npm show @rdcli-ai/rdcli version` |
| | npm 更新 | `npm update -g @cloudcli-ai/cloudcli` | `npm update -g @rdcli-ai/rdcli` |
| `modules/cli/sandbox.service.ts` | Docker image | `docker.io/cloudcliai/sandbox:*` | `docker.io/rdcliai/sandbox:*` |
| | CLI 帮助文本 | `cloudcli sandbox *` | `rdcli sandbox *` |
| | 日志路径 | `/tmp/cloudcli-ui.log` | `/tmp/rdcli-ui.log` |
| | 启动命令 | `nohup cloudcli start` | `nohup rdcli start` |
| `modules/system/system.service.ts` | npm 安装命令 | `@cloudcli-ai/cloudcli@latest` | `@rdcli-ai/rdcli@latest` |
| `modules/agent/agent.routes.ts` | 环境变量 | `CLOUDCLI_GITHUB_TOKEN` | `RDCLI_GITHUB_TOKEN` |
| `modules/database/connection.ts` | 注释 | `~/.cloudcli/auth.db` | `~/.rdcli/auth.db` |
| `modules/file-tree/file-tree.module.ts` | 上传 ID 前缀 | `cloudcli-file-upload-` | `rdcli-file-upload-` |
| `modules/assets/index.ts` | 注释 | `~/.cloudcli/assets` | `~/.rdcli/assets` |
| `modules/assets/services/image-assets.service.ts` | 注释 + 路径 | `~/.cloudcli/assets` | `~/.rdcli/assets` |
| `modules/assets/assets.routes.ts` | 注释 | `~/.cloudcli/assets` | `~/.rdcli/assets` |
| `modules/browser-use/browser-use-mcp.ts` | 环境变量 | `CLOUDCLI_BROWSER_USE_*` | `RDCLI_BROWSER_USE_*` |
| | MCP server 名 | `cloudcli-browser` | `rdcli-browser` |
| `modules/browser-use/browser-use.service.ts` | 环境变量 | `CLOUDCLI_BROWSER_USE_*` | `RDCLI_BROWSER_USE_*` |
| | 配置路径 | `~/.cloudcli/browser-use/profiles` | `~/.rdcli/browser-use/profiles` |
| | MCP server 名 | `cloudcli-browser` | `rdcli-browser` |
| | 启动命令 | `command: 'cloudcli'` | `command: 'rdcli'` |
| `modules/websocket/services/chat-websocket.service.ts` | 注释 | `~/.cloudcli/assets` | `~/.rdcli/assets` |
| `modules/providers/services/provider-models.service.ts` | 配置目录 | `.cloudcli` | `.rdcli` |
| 各测试文件 | 测试路径 | `.cloudcli/assets/*` | `.rdcli/assets/*` |
| | 测试数据 | `docker.io/cloudcliai/sandbox:*` | `docker.io/rdcliai/sandbox:*` |
| | 测试 env | `CLOUDCLI_GITHUB_TOKEN` | `RDCLI_GITHUB_TOKEN` |

### 1.6 CI/CD (`.github/workflows/`)

| 文件 | 改动项 |
|------|--------|
| `release.yml` | `releaseName`、changelog header |
| `desktop-release.yml` | Server bundle tag、env var、prerelease 名称、文案 |
| `desktop-macos-branch-build.yml` | Name tag、server bundle tag、env var、prerelease 名称、文案 |
| `desktop-windows-branch-build.yml` | 同上 |
| `docker.yml` | Docker image tag |

### 1.7 Docker (`docker/`)

| 文件 | 原值 | 新值 |
|------|------|------|
| `claude-code/Dockerfile` | `shared/install-cloudcli.sh` | `shared/install-rdcli.sh` |
| | `npm install -g @cloudcli-ai/cloudcli` | `npm install -g @rdcli-ai/rdcli` |
| | `cloudcli --version` | `rdcli --version` |
| | `shared/start-cloudcli.sh` → `~/.cloudcli-start.sh` | `start-rdcli.sh` → `~/.rdcli-start.sh` |
| `codex/Dockerfile` | 同上 | 同上 |
| `shared/start-cloudcli.sh` | 重命名为 `start-rdcli.sh`，内容全部替换 |  |
| `shared/install-cloudcli.sh` | 重命名为 `install-rdcli.sh`，内容全部替换 |  |
| `README.md` | 标题、文案、docker image、安装命令 | 全部替换 |

### 1.8 redirect-package (`redirect-package/`)

| 文件 | 改动 |
|------|------|
| `package.json` | `name`、`description`、`bin`、`homepage`、`keywords`、`author`、`deprecated` 信息 |
| `README.md` | 整篇 CloudCLI → rdCLI |
| `index.js` | import 路径 |
| `bin.js` | import 路径 |

### 1.9 PWA / 公共资产 (`public/`)

| 文件 | 改动 |
|------|------|
| `sw.js` | 注释、CACHE_NAME（保留 `claude-ui-v2`）、通知标题 fallback |
| `manifest.json` | 未直接修改（含图标路径，非品牌文字） |

### 1.10 Scripts (`scripts/`)

| 文件 | 改动 |
|------|------|
| `release/build-server-bundle.js` | 环境变量 `CLOUDCLI_BUNDLE_PLATFORM` / `CLOUDCLI_BUNDLE_ARCH`、bundle 文件名 |

---

## 二、运行时可能出现的兼容性问题

### 2.1 目录路径变更

| 旧路径 | 新路径 |
|--------|--------|
| `~/.cloudcli/` | `~/.rdcli/` |
| `~/.cloudcli/auth.db` | `~/.rdcli/auth.db` |
| `~/.cloudcli/assets/` | `~/.rdcli/assets/` |
| `~/.cloudcli/local-server.json` | `~/.rdcli/local-server.json` |
| `~/.cloudcli/server/` | `~/.rdcli/server/` |
| `~/.cloudcli/browser-use/profiles/` | `~/.rdcli/browser-use/profiles/` |

> **⚠️ 影响**: 已有安装的用户升级后，`~/.cloudcli/` 下的旧数据（auth.db、已上传的文件、local-server 配置）不会被自动迁移。如果需要兼容旧数据，需要添加迁移逻辑或将路径配成可配置的。

### 2.2 环境变量名变更

| 旧名称 | 新名称 |
|--------|--------|
| `CLOUDCLI_CONTROL_PLANE_URL` | `RDCLI_CONTROL_PLANE_URL` |
| `CLOUDCLI_SERVER_DIR` | `RDCLI_SERVER_DIR` |
| `CLOUDCLI_SERVER_BUNDLE_URL` | `RDCLI_SERVER_BUNDLE_URL` |
| `CLOUDCLI_SERVER_BUNDLE_RELEASE_TAG` | `RDCLI_SERVER_BUNDLE_RELEASE_TAG` |
| `CLOUDCLI_BUNDLE_PLATFORM` | `RDCLI_BUNDLE_PLATFORM` |
| `CLOUDCLI_BUNDLE_ARCH` | `RDCLI_BUNDLE_ARCH` |
| `CLOUDCLI_DESKTOP_LOCAL_SERVER_URL` | `RDCLI_DESKTOP_LOCAL_SERVER_URL` |
| `CLOUDCLI_LOCAL_SERVER_URL` | `RDCLI_LOCAL_SERVER_URL` |
| `CLOUDCLI_DESKTOP_LOCAL_SERVER_PORT` | `RDCLI_DESKTOP_LOCAL_SERVER_PORT` |
| `CLOUDCLI_SERVER_PORT` | `RDCLI_SERVER_PORT` |
| `CLOUDCLI_USE_INSTALLED_SERVER` | `RDCLI_USE_INSTALLED_SERVER` |
| `CLOUDCLI_SEMANTICS_BUILD_REQUIRED` | `RDCLI_SEMANTICS_BUILD_REQUIRED` |
| `CLOUDCLI_GITHUB_TOKEN` | `RDCLI_GITHUB_TOKEN` |
| `CLOUDCLI_BROWSER_USE_API_URL` | `RDCLI_BROWSER_USE_API_URL` |
| `CLOUDCLI_BROWSER_USE_MCP_TOKEN` | `RDCLI_BROWSER_USE_MCP_TOKEN` |
| `CLOUDCLI_BROWSER_USE_API_TIMEOUT_MS` | `RDCLI_BROWSER_USE_API_TIMEOUT_MS` |
| `CLOUDCLI_BROWSER_USE_MAX_SESSIONS_PER_OWNER` | `RDCLI_BROWSER_USE_MAX_SESSIONS_PER_OWNER` |
| `CLOUDCLI_BROWSER_USE_SESSION_TTL_MS` | `RDCLI_BROWSER_USE_SESSION_TTL_MS` |
| `CLOUDCLI_BROWSER_USE_INSTALL_TIMEOUT_MS` | `RDCLI_BROWSER_USE_INSTALL_TIMEOUT_MS` |

> **⚠️ 影响**: CI/CD 脚本、Docker compose、.env 文件、PM2 配置中如果设置了旧的环境变量名，需要同步更新。

### 2.3 IPC Channel 名变更 (Electron)

| 旧名称 | 新名称 |
|--------|--------|
| `cloudcli-desktop:connect-cloud` | `rdcli-desktop:connect-cloud` |
| `cloudcli-desktop:copy-diagnostics` | `rdcli-desktop:copy-diagnostics` |
| `cloudcli-desktop:copy-local-web-url` | `rdcli-desktop:copy-local-web-url` |
| `cloudcli-desktop:get-state` | `rdcli-desktop:get-state` |
| `cloudcli-desktop:state-updated` | `rdcli-desktop:state-updated` |
| `cloudcli-desktop:launcher-command` | `rdcli-desktop:launcher-command` |
| ...（全部 20+ 个 channel） | |

> **⚠️ 影响**: `main.js` 和 `preload.cjs` 的 channel 名必须完全一致，否则桌面端渲染进程和主进程之间通信全部中断。

### 2.4 Electron preload 全局对象名变更

| 旧名称 | 新名称 |
|--------|--------|
| `window.cloudcliDesktop` | `window.rdcliDesktop` |
| `window.cloudcliDesktopNotifications` | `window.rdcliDesktopNotifications` |

> **⚠️ 影响**: 前端 `src/hooks/useWebPush.ts` 和 `src/components/settings/view/Settings.tsx` 中都有 `(window as any).cloudcliDesktopNotifications` 检查。`launcher/launcher.js` 中也有 `window.cloudcliDesktop`。必须与 `preload.cjs` 中的 `exposeInMainWorld` 名称一致。

### 2.5 localStorage Key 变更

| 旧名称 | 新名称 |
|--------|--------|
| `CLOUDCLI_GITHUB_STARS` | `RDCLI_GITHUB_STARS` |
| `CLOUDCLI_HIDE_GITHUB_STAR` | `RDCLI_HIDE_GITHUB_STAR` |

> **影响**: 升级后旧 key 会被忽略，GitHub star 数据重新获取一次，影响很小。

### 2.6 npm 包名变更

| 旧名 | 新名 |
|------|------|
| `@cloudcli-ai/cloudcli` | `@rdcli-ai/rdcli` |

> **⚠️ 影响**: CI/CD 中的 `npm install -g @cloudcli-ai/cloudcli`、Dockerfile、i18n 中的升级命令全部需要更新。如果 npm 上还没有发布 `@rdcli-ai/rdcli` 包，这些命令都会失败。

### 2.7 Docker image 名变更

| 旧名 | 新名 |
|------|------|
| `docker.io/cloudcliai/sandbox:claude-code` | `docker.io/rdcliai/sandbox:claude-code` |
| `docker.io/cloudcliai/sandbox:codex` | `docker.io/rdcliai/sandbox:codex` |

> **⚠️ 影响**: 如果 Docker Hub 上没有对应的新 repo，`sandbox` 命令会失败。

### 2.8 CLI 命令名变更

| 旧名 | 新名 |
|------|------|
| `cloudcli` | `rdcli` |

> **影响**: `package.json` 中 `bin` 的 key 改了。用户安装后执行的命令从 `cloudcli` 变为 `rdcli`。文档中的示例命令全部需要更新。

### 2.9 桌面端协议变更

| 旧值 | 新值 |
|------|------|
| `cloudcli://auth/callback` | `rdcli://auth/callback` |

> **⚠️ 影响**: OAuth 回调 URL 注册到操作系统的协议变了。云端控制面的 OAuth 配置需要同步更新 callback URL，否则桌面端登录流程全部中断。

### 2.10 桌面端 App ID 变更

| 旧值 | 新值 |
|------|------|
| `ai.cloudcli.desktop` | `ai.rdcli.desktop` |

> **影响**: Windows 系统上 AppUserModelId 变了，任务栏分组和通知归属会变化。

### 2.11 域名变更

| 旧值 | 新值 |
|------|------|
| `cloudcli.ai` | `rdcli.ai` |
| `*.cloudcli.ai` (cookie 匹配) | `*.rdcli.ai` |

> **警告**: 如果 `rdcli.ai` 域名尚未注册/配置/部署，所有云端功能（环境列表、SSH、桌面通知）全部不可用。

---

## 三、排查指南

### 如果编译/打包失败

1. 检查 `package.json` 中 `bin.rdcli` 指向的 `dist-server/server/modules/cli/cli.js` 是否存在
2. 检查 `npm install` 时是否引用了未发布的 `@rdcli-ai/rdcli`
3. 检查 `electron-builder` 配置中的 `appId`、`protocols` 是否合法

### 如果桌面端启动失败

1. 检查 `electron/main.js` 中的 `APP_NAME` 和 `preload.cjs` 中的 `exposeInMainWorld` 名称
2. 检查 IPC channel 在 `main.js` 和 `preload.cjs` 中是否完全一致
3. 检查 `rdcli.ai` 是否可访问（如果使用云端功能）
4. 旧的 `~/.cloudcli/` 目录不会被自动读取，考虑是否需要迁移

### 如果 CLI 命令不工作

1. 确认 `package.json` 的 `bin` 字段 key 是 `rdcli`
2. `npm install -g` 后检查 `which rdcli`
3. 帮助文本已更新为 `rdcli [command]`

### 如果 i18n 不显示升级命令

1. 检查所有语言的 `common.json` 中 `npmUpgradeCommand` 是否为 `npm install -g @rdcli-ai/rdcli@latest`

### 如果 Docker sandbox 不工作

1. 检查 `docker.io/rdcliai/sandbox:claude-code` 这个镜像是否存在
2. 检查 `docker/shared/` 下的 shell 脚本是否已正确重命名（`start-rdcli.sh`, `install-rdcli.sh`）

---

## 四、未改动的引用（保留原样）

以下引用**故意没有改动**，因为它们指向外部资源或底层依赖：

| 引用 | 原因 |
|------|------|
| `github.com/siteboon/claudecodeui` | GitHub 仓库尚未迁移 |
| `@anthropic-ai/claude-agent-sdk` | npm 依赖，非本项目控制 |
| `@anthropic-ai/sdk` | 同上 |
| `trendshift.io/repositories/15586` (siteboon/claudecodeui) | 外部 badge |
| `raw.githubusercontent.com/siteboon/claudecodeui/main/public/` | README 中的截图外链 |
| `Claude` / `Claude Code` / `claude` 作为 AI 助手名称 | 保留原品牌 |
| `claude-ui-v2` (PWA CACHE_NAME) | 内部缓存标识，不影响用户感知 |
| `~/.claude/` 目录 | Claude Code 自身的配置目录，不可改动 |

---

## 五、回滚方法

如果需要回滚此次改名，执行反向的 sed 替换：

```bash
find . -type f \( -name "*.ts" -o -name "*.tsx" -o -name "*.js" -o -name "*.jsx" \
  -o -name "*.json" -o -name "*.md" -o -name "*.html" -o -name "*.css" \
  -o -name "*.yml" -o -name "*.yaml" -o -name "*.sh" -o -name "*.cjs" \) \
  ! -path "*/node_modules/*" ! -path "*/.git/*" ! -path "*/dist-server/*" \
  ! -name "package-lock.json" -print0 | xargs -0 sed -i \
  -e 's/rdCLI/CloudCLI/g' -e 's/RDCLI/CLOUDCLI/g' -e 's/rdcli/cloudcli/g'
```

然后将 docker shell 脚本重命名回去：
```bash
mv docker/shared/start-rdcli.sh docker/shared/start-cloudcli.sh
mv docker/shared/install-rdcli.sh docker/shared/install-cloudcli.sh
```
