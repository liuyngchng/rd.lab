## 1. document server

### 2.1 docker

```sh
# 拉取镜像
docker pull docker.1ms.run/onlyoffice/documentserver:latest

# 运行
docker run -i -t -d -p 8080:80 \
  -e JWT_ENABLED=true \
  -e JWT_SECRET=your_jwt_secret \
  onlyoffice/documentserver
  
# 停止已有的容器
docker stop onlyoffice-document-server
  
# 2. 运行容器, 注意，使用了 host模式， 8080的端口映射就失效了。
docker run -i -t -d -p 8080:80 \
  --network=host \
  --restart=always \
  -e JWT_ENABLED=true \
  -e JWT_SECRET=your_jwt_secret_here \
  -e JWT_HEADER=Authorization \
  -v /app/onlyoffice/DocumentServer/logs:/var/log/onlyoffice \
  -v /app/onlyoffice/DocumentServer/data:/var/www/onlyoffice/Data \
  -v /app/onlyoffice/DocumentServer/lib:/var/lib/onlyoffice \
  -v /app/onlyoffice/DocumentServer/db:/var/lib/postgresql \
  --name onlyoffice-document-server \
  onlyoffice/documentserver

# 验证访问
curl http://localhost:8080/welcome
```



### 2.2 primitive



```sh
# Ubuntu/Debian
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys CB2DE8E5
sudo echo "deb https://download.onlyoffice.com/repo/debian squeeze main" | sudo tee /etc/apt/sources.list.d/onlyoffice.list
sudo apt-get update
sudo apt-get install onlyoffice-documentserver

# 配置
sudo onlyoffice-documentserver configure
```



## 2. doc

API 的使用方法详见  https://api.onlyoffice.com/docs/docs-api/get-started/

### 2.1 前端

页面中嵌入 js  http://localhost:8080/web-apps/apps/api/documents/api.js



数据流

```sh
┌─────────────────┐        上传请求        ┌─────────────────┐
│                 │ ─────────────────────> │                 │
│   用户浏览器    │                        │   Flask 应用     │
│                 │ <───────────────────── │   (localhost:19000) │
│                 │      返回配置           │                 │
└─────────────────┘                        └─────────┬───────┘
         │                                            │
         │ 加载OnlyOffice编辑器                        │ 返回文档文件
         │                                            │
         ▼                                            ▼
┌─────────────────┐  请求文档文件     ┌─────────────────┐
│                 │ ────────────────> │   Flask 应用     │
│ OnlyOffice编辑器 │                  │   (提供下载)     │
│ (localhost:8080) │ <──────────────── │                 │
│                 │   文档文件流       └─────────────────┘
└─────────┬───────┘
         │
         │ 用户编辑文档
         │
         │ 保存文档时
         │
         ▼
┌─────────────────┐  回调请求 (POST)  ┌─────────────────┐
│                 │ ────────────────> │   Flask 应用     │
│ OnlyOffice编辑器 │                  │   (callbackUrl)  │
│                 │ <──────────────── │                 │
│                 │    响应结果        └─────────────────┘
└─────────────────┘
```

