# 官方网站

https://playwright.dev/docs/getting-started-mcp


# 环境配置

```sh
# 安装 mcp server
claude mcp add playwright npx @playwright/mcp@latest

# 安装浏览器插件
npx playwright install chromium

# 查看mcp 清单
claude
/mcp


# 删除 mcp server
claude mcp remove playwright
```



使用 `docker` 环境的 `mcp server`

```sh
# 拉取镜像
docker pull mcr.microsoft.com/playwright/mcp:latest

# 添加 mcp server 
claude mcp add playwright -- docker run -i --rm mcr.microsoft.com/playwright/mcp:latest

# docker 中已经有内置的无头的 chrome 浏览器了，无需再像宿主机环境，还需执行 npx playwright install chromium
```


# 代码生成器（codegen）

在浏览器上一顿操作，自动复刻生成代码。

```sh
# 打开页面并录制，实时生成代码
playwright codegen https://example.com

# 指定生成 Python 代码
playwright codegen --target python https://example.com

# 指定语言 + 保存到文件
playwright codegen --target python -o myscript.py https://example.com

# 模拟移动设备
playwright codegen --device="iPhone 13" https://example.com

# 录制时保存登录状态（录完不用再登录）
playwright codegen --save-storage=auth.json https://example.com
```

运行后同时打开两个窗口：

1. 浏览器窗口 —— 你在里面操作，点击、输入都会被录制
2. Playwright Inspector 窗口 —— 实时生成对应代码，点什么写什么

示例：登录操作生成的 Python 代码

```python
page.goto("https://example.com/login")
page.get_by_label("用户名").fill("admin")
page.get_by_label("密码").fill("123456")
page.get_by_role("button", name="登录").click()
```

## 注意事项

- 只记录操作序列，不记录「意图」，不自动加断言（录完需要自己加 `expect`）
- 动态内容（弹窗、日期选择等）可能需要手动调整选择器

