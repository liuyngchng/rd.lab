# rancher 1.x

##  pull

docker rancher，1.x 的 images 是 `rancher/server`。rancher1.x 所支持的docker 版本请参考 https://rancher.com/docs/rancher/v1.6/zh/hosts/#docker%E7%89%88%E6%9C%AC%E9%80%82%E7%94%A8%E5%AF%B9%E6%AF%94， 本案例中安装的docker 版本如下所示

```sh
docker version
Client:
 Version:      17.03.2-ce
 API version:  1.27
 Go version:   go1.7.5
 Git commit:   f5ec1e2
 Built:        Tue Jun 27 01:35:00 2017
 OS/Arch:      linux/amd64

Server:
 Version:      17.03.2-ce
 API version:  1.27 (minimum version 1.12)
 Go version:   go1.7.5
 Git commit:   f5ec1e2
 Built:        Tue Jun 27 01:35:00 2017
 OS/Arch:      linux/amd64
 Experimental: false
```

获取镜像文件

```sh
docker search rancher
# 实际版本为 1.6.30
docker pull rancher/server
docker pull rancher/agent:v1.2.11

```

##  MySQL 5.7

rancher 1.x 适配的 MySQL 为5.7, MySQL 8 会报错。

```sh
docker pull mysql:5.7
docker run --name mysql5.7 -p 3307:3306 -e MYSQL_ROOT_PASSWORD='*****' -d mysql:5.7
docker exec -it mysql5.7 /bin/bash
cd /etc/
# /etc/mysql 是容器里mysql的配置文件夹
tar -czf mysql.tar.gz mysql
exit
mkdir -p /data/mysql57/log
mkdir -p /data/mysql57/data
docker cp mysql5.7:/etc/mysql.tar.gz /data/mysql57
cd /data/mysql57
tar -zxf mysql.tar.gz
mv mysql conf
# 设置时区
vi /data/mysql57/conf/conf.d/my.cnf
[mysqld]
default-time-zone = '+08:00'

# 停止当前 mysql 服务
docker stop mysql5.7
docker rm mysql5.7

# 重启服务
docker run -dit \
	--name mysql5.7 \
	-p 3307:3306 \
	-e MYSQL_ROOT_PASSWORD='*****' \
	-e LANG=C.UTF-8 \
	-v /data/mysql57/conf:/etc/mysql \
	-v /data/mysql57/data:/var/lib/mysql \
	mysql:5.7
```



##  run

```sh
docker run -d --restart=unless-stopped --name rancher -p 8084:8080 rancher/server --db-host 11.10.36.1 --db-port 3307 --db-user root --db-pass '**********' --db-name cattle

docker stop rancher
docker rm rancher
docker run -d --restart=unless-stopped \
  --name rancher -p 8084:8080 \
    rancher/server:v1.6.30 \
  --db-host 11.10.36.2 \
  --db-port 3307 \
  --db-user root \
  --db-pass '*****' \
  --db-name cattle\
```

通过以下地址浏览

```
https://11.10.36.2:8084
```

在页面右下角，修改语言为“简体中文”

##  config

### 修改主机注册地址

点击 系统管理-系统设置 ， 将https 修改为http， 这样通过rancher-agent 注册主机至rancher-server时，api使用 http，或者配置证书

```sh
mkdir -p  /rancher/privatekey       
mkdir -p /rancher/certs/
openssl genrsa -out /rancher/privatekey/rancher.key 2048  
openssl req -new -key /rancher/privatekey/rancher.key -out /rancher/certs/rancher.csr   #需要输入证书信息,随便填就行
openssl x509 -req -days 1825 -in /rancher/certs/rancher.csr -signkey /rancher/privatekey/rancher.key -out /rancher/certs/rancher.crt  
```

在系统界面上添加证书

### 设置登录用户

点击 系统管理-访问控制，点击“local”， 点击  “1. 设置管理员用户”，填写用户名和密码，点击 “启用本地验证”后， 在页面右上角点击注销用户，重新登录

### 创建环境

点击 环境-环境管理， 添加环境，例如 test

### 添加主机

注意，在添加主机前，先创建环境，这样添加的主机就在对应的环境中了，否则列入default 环境中

点击 基础架构-添加主机， 在“指定用于注册这台主机的公网IP” 添加注册主机的IP 地址

按照页面提示，拷贝脚本备用

```sh
# key a6HpKwjy3PQ5PAOnFIzQFBqnyUU 对应于不同的环境
docker run -e CATTLE_AGENT_IP="11.10.36.2" \
	--rm --privileged \
	-v /var/run/docker.sock:/var/run/docker.sock \
	-v /var/lib/rancher:/var/lib/rancher \
	rancher/agent:v1.2.11 \
	http://11.10.36.2:8084/v1/scripts/167EC71967993A3160E1:1703980800000:a6HpKwjy3PQ5PAOnFIzQFBqnyUU
```

## 启动agent

通过 rancher-agent 注册 host 至 rancher server，通过1.4节的脚本

```sh
# pull image
docker pull rancher/agent:v1.2.11
# run
docker run xxxx
```

此时，在页面中点击 基础架构-主机，可以看到容器列表

## 负载均衡

rancher 中，当启动的容器数量大于1时，系统自动提供负载均衡，负载均衡微服务默认为 `rancher/lb-service-haproxy:v0.9.14`。官方文档详见

https://docs.rancher.cn/docs/rancher1/infrastructure/cattle/adding-load-balancers/_index/

注意， 系统自动提供的负载均衡，对于服务的网络类型有要求，具体如下。

```wiki
Load balancers will only work for services that are using the managed network. If you select any other network choice for your target services, it will not work with the load balancer.
```



# rancher 2.x

##  pull

docker rancher，2.x 的 images 是 `rancher/rancher`。当前版本为 2.8.1, 2.x较为复杂，目前还没有玩明白。

```sh
docker pull rancher/rancher
docker pull rancher/rancher-agent
mkdir -p /data/rancher/rancher
mkdir -p /data/rancher/audit
```

##  run

```sh
docker run -d --privileged --restart=unless-stopped -p 8085:80 -p 8443:443 \
  	--name rancher-server -e CATTLE_DB_CATTLE_MYSQL_HOST=11.10.36.1 \
	-e CATTLE_DB_CATTLE_MYSQL_PORT=3306 \
  	-e CATTLE_DB_CATTLE_MYSQL_NAME=cattle \
  	-e CATTLE_DB_CATTLE_USERNAME=foo \
  	-e CATTLE_DB_CATTLE_PASSWORD='*******' \
  	-e 'https_proxy=' -e 'HTTPS_PROXY=' -e 'HTTP_PROXY=' \
	-e 'http_proxy='  -e 'ALL_PROXY='   -e 'all_proxy=' \
  	-v /data/rancher/rancher:/var/lib/rancher \
	-v /data/rancher/audit:/var/log/auditlog \
  	rancher/rancher
```

## login

```sh
https://localhost:8443
```

获取初始化密码

```sh
docker logs  rancher-server  2>&1 | grep "Bootstrap Password:"
```

找到密码后，

在web界面中输入初始化密码，获取新密码

在 https://localhost:8443 中输入 新用户名(admin)和新密码登录

