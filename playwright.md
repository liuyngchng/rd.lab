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

