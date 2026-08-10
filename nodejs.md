
用 Node.js 的 vite 把 React/Vue/纯 JS 应用，打包成一个自包含的 `.html` 文件。
双击即可在浏览器打开，无需 Node、无需服务器、无需网络。适合做**工具型小应用**分享给别人。

---

## 1. 原理

```
TypeScript / JSX 源码
   ↓  vite（Node.js 构建工具）
   JS 打包 + CSS 内联 + 字体 base64 内联
   ↓
单个 index.html（自包含）
```

- **打包过程**需要 Node.js（vite 是 Node 工具链）
- **产物运行**不需要 Node —— 就是个静态文件，浏览器直接执行里面的 `<script>`

> 类比：编译器（需要装）编出 .exe，之后运行 .exe 不需要编译器。

---

## 2. 最简步骤

### 2.1 已有 Vite + React 项目

```bash
cd your-project
# 安装单文件打包插件
npm install -D vite-plugin-singlefile
```

### 2.2 在 `package.json` 里加一条打包命令

```json
{
  "scripts": {
    "build:single": "tsc -b && npx vite build && node scripts/inline-fonts.mjs"
  }
}
```

> - `tsc -b`：TypeScript 类型检查（可省）
> - `npx vite build`：vite 打包（关键）
> - `node scripts/inline-assets.mjs`：资源内联兜底（见第 3 节，仅当还有资源没内联时才需要）

### 2.3 修改 `vite.config.ts`

```ts
import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import { viteSingleFile } from 'vite-plugin-singlefile';

export default defineConfig(() => ({
  plugins: [
    react(),
    viteSingleFile(),          // ← 关键：把所有 JS/CSS 内联进 index.html
  ],
  build: {
    assetsInlineLimit: 100000000,  // 资源内联上限调大（防止字体走独立文件）
  },
  base: './',                     // 相对路径，保证 file:// 下资源可解析
}));
```

### 2.4 打包

```bash
# 方式一：直接用 npm 脚本（推荐）
npm run build:single

# 方式二：命令行一条龙
npx vite build && node scripts/inline-assets.mjs
```

产物：`dist/index.html` —— 直接双击浏览器打开。

---

## 2.5 常用命令速查

| 命令 | 作用 |
|------|------|
| `npm install -D vite-plugin-singlefile` | 安装单文件插件 |
| **`npm run build:single`** | 完整打包单文件（类型检查 + vite + 字体内联） |
| `npx vite build` | 只打包（不含字体内联脚本） |
| `npm run dev` | 开发调试（起本地 server，改代码热更新） |
| `npm run preview` | 预览构建产物（模拟生产环境） |

---

## 3. 保证 JS / CSS / 资源全部内联

页面关联的内容分两类：

- **JS、CSS**：不用操心。`viteSingleFile()` 会自动把打包好的 JS 和 CSS 全部内联进 `index.html`。
- **资源（字体、图片、favicon）**：坑全在这。vite 默认只把小于 `assetsInlineLimit` 的资源内联（转 base64），超出的会复制成独立文件 → `file://` 下 404（图标变方块、图片空白）。

保证资源也全内联，两种通用套路：

### 方案 A：资源放 `src/`，走 `import`（推荐）

CSS、字体、图片都放进 `src/` 由 vite 处理（别放 `public/`、别用 `<link>` 外链），再把 `assetsInlineLimit` 调大（见 2.3）。vite 会把 CSS 里的 `url()` 和 import 的资源自动转 base64 内联：

```ts
import './assets/fontawesome.css'; // 里面引用的字体一并内联
```

### 方案 B：构建后脚本兜底

有资源没法走 import（比如 vite 就是不内联某些大文件），写个 Node 脚本在 `vite build` 后扫描 `dist/index.html`，把残留的外部 `url(...)` 用 dist 里对应文件 base64 替换：

```js
// scripts/inline-assets.mjs —— 正则按你的资源路径改
import fs from 'node:fs';
import path from 'node:path';

const dist = path.resolve('dist');
const htmlPath = path.join(dist, 'index.html');
let html = fs.readFileSync(htmlPath, 'utf8');

html = html.replace(/url\((?:\.\.\/)?webfonts\/([a-z0-9-]+\.woff2)\)/g, (m, name) => {
  const p = path.join(dist, 'webfonts', name);
  if (fs.existsSync(p)) {
    return `url(data:font/woff2;base64,${fs.readFileSync(p).toString('base64')})`;
  }
  return m;
});

fs.writeFileSync(htmlPath, html);
```

```bash
npx vite build && node scripts/inline-assets.mjs
```

> **字体是重灾区**：`.woff2` 偏大，vite 默认不内联，漏掉就静默变方块（不报错）。是否真自包含，验证方法见第 4 节。

---

## 4. 验证产物是否真正自包含

```bash
# 1. 不应有外部 .js / .css / 字体链接
grep -oE '(src|href)="[^"#]*\.(js|css|woff2)"' dist/index.html
# 2. 字体已内联（应该有 data:font）
grep -c "data:font/woff2" dist/index.html
# 3. script 无外链
grep -o '<script[^>]*>' dist/index.html
```

三项都符合 = 单文件可用。

---

## 5. 注意事项

| 事项 | 说明 |
|------|------|
| **体积** | 单文件通常几百 KB ~ 几 MB（字体是大头）。1.4MB / gzip ~145KB 算正常 |
| **后端 API** | 不能调跨域后端。有 API 需求要么用 `file://` 访问不了 → 需起 server，要么把数据做成纯本地（localStorage / 导入导出文件） |
| **多标签页存储** | `sessionStorage`/`localStorage` 在 file:// 下可用 |
| **favicon / 图片** | 同样需要内联（data URL）或 `assetsInlineLimit` 调大 |
| **dev 代理** | `vite.config` 里的 proxy 只在 dev server 生效，单文件版无关 |

---

## 6. 真实案例

本仓库的 `f/`（ReactFlow 工作流设计工具）就是这样打包的：

- `f/vite.config.ts` — 见上文方案 A + B 组合
- `f/scripts/inline-assets.mjs` — 兜底内联 4 个 woff2
- `npm run build:single` — 产出 `f/dist/index.html`
- 最终单文件 `workflow-editor.html`（~1.4MB），浏览器双击即用，导出 JSON/MD/PNG 全走本地 Blob 下载，无需后端

---

## 7. 其他打包成"独立可运行"的选项

| 方案 | 形态 | 适用 |
|------|------|------|
| **单文件 HTML**（本文） | 一个 .html | 轻量工具、分享给他人、双击即用 |
| **Tauri** | 桌面 app（.exe/.app） | 需要系统能力（文件系统、系统托盘） |
| **Electron** | 桌面 app（体积大） | 复杂桌面应用 |
| **PWA** | 网页 + 可安装 | 需要 server 托管 |

---

*关键词：vite singlefile、单文件打包、自包含 html、file:// 打开前端、字体内联*

## 8. 镜像及代理



```sh
# 临时使用镜像源
npm install --registry=https://registry.npmmirror.com
# 临时使用代理
npm config set proxy http://127.0.0.1:8080
npm config set https-proxy http://127.0.0.1:8080
```

其他可以使用的镜像源

| 镜像源名称           | 地址 (Registry URL)                         |
| :------------------- | :------------------------------------------ |
| **淘宝 (npmmirror)** | `https://registry.npmmirror.com`            |
| **腾讯云**           | `https://mirrors.cloud.tencent.com/npm/`    |
| **华为云**           | `https://mirrors.huaweicloud.com/npm/`      |
| **阿里云**           | `https://npm.aliyun.com`                    |
| **清华大学**         | `https://mirrors.tuna.tsinghua.edu.cn/npm/` |

