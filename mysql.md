# ubuntu18 修改mysql数据库密码
```sh
sudo vim /etc/mysql/my.cnf
```
添加如下内容：
```sh
[mysqld]
skip-grant-tables=1
```
重启mysql服务
```sh
sudo service mysql restart
```
登录mysql
```sh
mysql
```
切换到mysql数据库,并修改root用户密码
```sh
use mysql;
update user set plugin='mysql_native_password' where user='root';
update user set authentication_string=password('123456') where user='root';
flush privileges;
exit;
```
修改配置并重启mysql服务
```sh
sudo sudo vim /etc/mysql/my.cnf把skip-grant-tables删除掉
sudo service mysql restart
```
# binlog

##   登录到MySQL查看binlog

只查看第一个binlog文件的内容
	
show binlog events;  

查看指定binlog文件的内容  

`show binlog events in 'mysql-bin.000002';`

查看当前正在写入的binlog文件  

`show master status\G;`

获取binlog文件列表  

`show binary logs;`

##   config mysqld in ubuntu

修改 my.cnf  
查看my.cnf的位置 `file /etc/mysql/my.cnf`  
```sh
cd /etc/mysql/mysql.conf.d
vim mysqld.cnf

```
去掉以下三项的注释

```sh
server-id       	= 1
log_bin         	= /var/log/mysql/mysql-bin.log
expire_logs_days    = 10
max_binlog_size   	= 100M
```
执行

```sh
systemctl restart mysql 
mysql -uroot -p
show variables like '%log_bin%';
```
看到 log_bin | ON

```sh
+---------------------------------+--------------------------------+
| Variable_name                   | Value                          |
+---------------------------------+--------------------------------+
| log_bin                         | ON                             |
| log_bin_basename                | /var/log/mysql/mysql-bin       |
| log_bin_index                   | /var/log/mysql/mysql-bin.index |
| log_bin_trust_function_creators | OFF                            |
| log_bin_use_v1_row_events       | OFF                            |
| sql_log_bin                     | ON                             |
+---------------------------------+--------------------------------+
```



##   browse binlog

sudo mysqlbinlog  -d dbname --base64-output=decode-rows  /var/log/mysql/mysql-bin.000001

# general query log

修改 my.cnf  
查看my.cnf的位置 `file /etc/mysql/my.cnf`  
```sh
cd /etc/mysql/mysql.conf.d
vim mysqld.cnf

```
去掉以下两项的注释

```sh
general_log_file        = /var/log/mysql/mysql.log
general_log             = 1
```

查看日志

```sh
tail -f /var/log/mysql/mysql.log
```
 # mysqldump 数据迁移
 from MySQL8 to mysql5.7

数据导出 带表结构和库结构

```sh
mysqldump --databases ry -uroot -p > ./source.sql
```

数据修改
```sh
sed -i "s/utf8mb4_0900_ai_ci/utf8_general_ci/g" ./source.sql
sed -i "s/utf8mb4/utf8/g" ./source.sql
```

数据导入

```sh
mysql -h11.11.54.33 -P13307 -uroot -p ry <./source.sql
```

# 修改MySQL8 密码

查看临时密码

```sh
grep 'password' /var/log/mysqld.log
```

修改mysql密码

 ```sh
mysql -uroot -p 				//输入临时密码
alter USER 'root'@'localhost' IDENTIFIED BY 'mypassword';
 ```

授权远程连接

```sh
show databases;
use mysql;
select host, user, authentication_string, plugin from user;

update user set host = "%" where user='root';
select host, user, authentication_string, plugin from user;
flush privileges;
```

远程 telnet IP port， 若不通，则可能是防火墙原因，执行

```sh
systemctl status firewalld
systemctl stop firewall
```

远程连接若出现 Authenticationplugin‘caching_sha2_password’ cannot be loaded:XXXXX， 则


```sh
mysql -u root -p
use mysql;
alter USER 'root'@'%' IDENTIFIED WITH mysql_native_password BY 'mypassword';
flush privileges;
```

```sh
备注：
mysql8 之前的版本中加密规则是mysql_native_password，而在mysql8之后,加密规则是caching_sha2_password。
可以把mysql用户登录密码加密规则还原成mysql_native_password.。
```

# docker setup mysql8

## docker pull

```sh
docker pull mysql:8.0.28
docker images

```

##  config start up

初始化mysql密码，打包配置文件

```sh
docker run --name mysql -p 3306:3306 -e MYSQL_ROOT_PASSWORD=fGB#K9rtXFELC8y^3%GN -d mysql:8.0.28
docker ps
docker exec -it mysql /bin/bash
cd /etc/
tar -czf mysql.tar.gz mysql
exit
```

在宿主机上拷贝配置文件

```sh
su root
mkdir -p /data/mysql/log
mkdir -p /data/mysql/data
docker cp mysql:/etc/mysql.tar.gz /data/mysql
cd /data/mysql
mv mysql conf
docker stop mysql
docker rm mysql
```

##  normal start up

```sh
docker run -dit \
--name mysql \
-p 3306:3306 \
-e MYSQL_ROOT_PASSWORD=fGB#K9rtXFELC8y^3%GN \
-e LANG=C.UTF-8 \
-v /data/mysql/conf:/etc/mysql \
-v /data/mysql/data:/var/lib/mysql \
mysql:8.0.28
```

如果看到

```sh
docker: Error response from daemon: driver failed programming external connectivity on endpoint mysql (0482470b97dad25e247e8396b943a466338dada8d5356de7636e7301f8aba8b4):  (iptables failed: iptables --wait -t filter -A DOCKER ! -i docker0 -o docker0 -p tcp -d 172.17.0.2 --dport 3306 -j ACCEPT: iptables: No chain/target/match by that name
```

则运行

```sh
# centos
systemctl stop docker.service 
iptables -t nat -F
ifconfig docker0 down
brctl delbr docker0
systemctl start docker.service

```





##  创建用户

```sh
CREATE USER 'foo'@'%' IDENTIFIED WITH mysql_native_password BY 'fGB#K9rtXFELC8y^3%GN';
grant all privileges on mysql.* to 'foo'@'%';
flush privileges;
```

##  禁止root用户远程登录

```sh
drop user 'root'@'%';
```

