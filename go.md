## 开发环境搭建

### 1. 安装 Go

```bash
# Ubuntu/Debian
sudo apt install golang-go

# 或从官网下载最新版
wget https://go.dev/dl/go1.23.0.linux-amd64.tar.gz
sudo tar -C /usr/local -xzf go1.23.0.linux-amd64.tar.gz
echo 'export PATH=$PATH:/usr/local/go/bin' >> ~/.bashrc
source ~/.bashrc

# 验证
go version
```

### 2. 配置 Go 代理（中国大陆用户）

由于国内访问 GitHub 可能受限，需要设置 Go 代理：

```bash
go env -w GOPROXY=https://goproxy.cn,direct
```

验证：

```bash
go env GOPROXY
# 输出: https://goproxy.cn,direct
```

> `goproxy.cn` 会缓存 GitHub 上的 Go 包，避免直接访问 GitHub 超时。

### 3. 解决 GitHub 无法访问的问题

如果 `goproxy.cn` 也没有缓存某些包（如 Milvus SDK），可以手动下载到本地后使用 `replace` 指令：

```bash
# 克隆需要的项目到本地
cd ~/workspace
git clone https://github.com/gin-gonic/gin.git
git clone https://github.com/milvus-io/milvus-sdk-go.git

# 在 go.mod 中添加本地路径映射
cd go_to_chat
go mod edit -replace github.com/gin-gonic/gin=/home/rd/workspace/gin
go mod edit -replace github.com/milvus-io/milvus-sdk-go/v2=/home/rd/workspace/milvus-sdk-go
```

> 如果你能直接访问 GitHub，不需要上面的步骤，`go mod tidy` 会自动下载。

### 4. 下载依赖

```bash
cd your_project
go env -w GOPROXY=https://goproxy.cn,direct   # 国内用户先设置代理
go mod tidy
```
