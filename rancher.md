# rancher 1.x

##  pull

docker rancher，1.x 的 images 是 `rancher/server`。

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
docker run --name mysql5.7 -p 3307:3306 -e MYSQL_ROOT_PASSWORD='sfs' -d mysql:5.7
docker exec -it mysql /bin/bash
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
vi /data/mysql57/conf/my.cnf
[mysqld]
default-time-zone = '+08:00'

# 停止当前 mysql 服务
docker stop mysql5.7
docker rm mysql5.7

# 重启服务
docker run -dit \
--name mysql5.7 \
-p 3307:3306 \
-e MYSQL_ROOT_PASSWORD='12345' \
-e LANG=C.UTF-8 \
-v /data/mysql57/conf:/etc/mysql \
-v /data/mysql57/data:/var/lib/mysql \
mysql:5.7
```



##  run

```sh
docker run -d --restart=unless-stopped --name rancher -p 8084:8080 rancher/server --db-host 11.10.36.1 --db-port 3307 --db-user root --db-pass 'du1124WXczZ!l1#a' --db-name cattle
docker run -d --restart=unless-stopped --name rancher -p 8084:8080 rancher:1.6.30 --db-host 11.10.36.1 --db-port 3307 --db-user root --db-pass 'du1124WXczZ!l1#a' --db-name cattle
```

##  config

系统管理-系统设置-主机注册地址，去掉https，或者配置证书

```sh
mkdir -p  /rancher/privatekey       
mkdir -p /rancher/certs/
openssl genrsa -out /rancher/privatekey/rancher.key 2048  
openssl req -new -key /rancher/privatekey/rancher.key -out /rancher/certs/rancher.csr   #需要输入证书信息,随便填就行
openssl x509 -req -days 1825 -in /rancher/certs/rancher.csr -signkey /rancher/privatekey/rancher.key -out /rancher/certs/rancher.crt  
```

在系统界面上添加证书

##  注册 host

按照页面提示，通过 rancher-agent 注册 host 至 rancher server。

# rancher 2.x

##  pull

docker rancher，2.x 的 images 是 `rancher/rancher`。当前版本为 2.8.1

```sh
docker pull rancher/rancher
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

