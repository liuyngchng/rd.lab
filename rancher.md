# setup

## rancher

https://blog.csdn.net/qq_42268472/article/details/126798628



https://forums.rancher.com/t/rancher-release-v1-6-30/17359

```sh
docker search rancher
# 实际版本为 1.6.30
docker pull rancher/server
docker pull rancher/agent:v1.2.11

```

## mysql 5.7

```sh
docker pull mysql:5.7
docker run --name mysql5.7 -p 3307:3306 -e MYSQL_ROOT_PASSWORD='du1124WXczZ!l1#a' -d mysql:5.7
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
-e MYSQL_ROOT_PASSWORD='du1124WXczZ!l1#a' \
-e LANG=C.UTF-8 \
-v /data/mysql57/conf:/etc/mysql \
-v /data/mysql57/data:/var/lib/mysql \
mysql:5.7

```



# run

```sh
docker run -d --restart=unless-stopped --name rancher -p 8084:8080 rancher/server --db-host 11.10.36.1 --db-port 3307 --db-user root --db-pass 'du1124WXczZ!l1#a' --db-name cattle
docker run -d --restart=unless-stopped --name rancher -p 8084:8080 rancher:1.6.30 --db-host 11.10.36.1 --db-port 3307 --db-user root --db-pass 'du1124WXczZ!l1#a' --db-name cattle
```

# config

系统管理-系统设置-主机注册地址，去掉https，或者配置证书

```sh
mkdir -p  /rancher/privatekey       
mkdir -p /rancher/certs/
openssl genrsa -out /rancher/privatekey/rancher.key 2048  
openssl req -new -key /rancher/privatekey/rancher.key -out /rancher/certs/rancher.csr   #需要输入证书信息,随便填就行
openssl x509 -req -days 1825 -in /rancher/certs/rancher.csr -signkey /rancher/privatekey/rancher.key -out /rancher/certs/rancher.crt  
```

在系统界面上添加证书

# rancher-agent注册host

