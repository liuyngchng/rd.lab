#  1. 修改密码

##  1.1 MySQL 8

一般，在文件  /etc/mysql/debian.cnf 中有初始化安装中系统自动生成的用户名和密码。

在对ubuntu 22.04 上通过sudo apt-get install 安装的 MySQL 8.0.36 中测试通过

在 docker pull mysql:8.4.0 中测试通过

```sh
# ubuntu 22.04 LTS
sudo vi /etc/mysql/my.cnf
# ubuntu 24.02 LTS
sudo vi /etc/mysql/mysql.conf.d/mysqld.cnf
# 添加如下内容
[mysqld]
skip-grant-tables=1
# 保存后重启 mysql服务
sudo service mysql restart
# 连接服务
mysql
# 修改密码
use mysql;
# 首先执行 flush ***，不然会报  ERROR 1290 (HY000): The MySQL server is running with the --skip-grant-tables option so it cannot execute this statement
flush privileges;
update user set plugin='mysql_native_password' where user='root';
flush privileges;
ALTER USER 'root'@'localhost' IDENTIFIED BY 'P@$$W0rd';
FLUSH PRIVILEGES;
exit;
# 重新修改配置文件
sudo vi /etc/mysql/my.cnf
# 删除如下内容
[mysqld]
skip-grant-tables=1
# 保存后重启 mysql服务
sudo service mysql restart
# 连接mysql server
mysql -h localhost -u root -p
# 输入新密码登录,over
```



## 1.2 mysql5.7

OS = ubuntu18

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
##  1.3 修改MySQL8 临时密码

此方法在从dev.mysql.org上自行下载安装的 mysql 8.0.28 deb 中实验通过 

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



# 2. MySQL 增量备份

##   2.1 查看binlog

只查看第一个binlog文件的内容

```sh
mysql -h xxx -u xxx -p;
show binlog events;  
```

查看指定binlog文件的内容  

````sh
mysql -h xxx -u xxx -p;
show binlog events in 'mysql-bin.000002';
````

查看当前正在写入的binlog文件  

```sh
show master status\G;
```

获取binlog文件列表  

```sh
show binary logs;
```

##   2.2 config binlog

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



##   2.3 browse binlog file

```sh
sudo mysqlbinlog  -d dbname --base64-output=decode-rows  /var/log/mysql/mysql-bin.000001
```

## 2.4 清空 binlog

```sh
mysql>RESET MASTER;
# 可以看到文件已经清空
ls -al /MYSQL_DIR/mysql/mysqllog/binlog
```

##  2.5 生成新的bin log file

```sh
# 可以手动刷新日志，生成一个新的 binlog 文件
flush logs
```

##  2.6 增量备份

```shell
# 备份全备份数据库
mysqldump -u 用户名 -p 数据库名 > database_full_backup.sql
 
# 备份自上次全备份以来的binlog日志（start-position 为上次全备份时binlog的位置， mysql-bin.000001 为binlog文件名称 ）
# binlog 位置 可以通过  'show binlog events in 'binlog.000024'' 中的 End_log_pos 字段查看， 
# binlog 也可以通过直接查看 binlog 文件中的 end_log_pos 字段查看 
mysqlbinlog --start-position=123 mysql-bin.000001 > database_binlog_backup.sql
 
# 应用binlog日志到备份的数据库中
mysql -u 用户名 -p 数据库名 < database_binlog_backup.sql
```



# 3. general query log

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
 # 4. mysqldump 数据迁移
 from MySQL8 to mysql5.7

数据导出 带表结构和库结构

```sh
mysqldump --databases my_db -uroot -p > ./source.sql
mysqldump -h host -P port -u usrname  -p password --databases dbname --dump-date > backup_file.sql

mysqldump -h 192.168.1.1 -P 3306 -u whoami -p'!@#$%^&^%$#!l1#a' --databases my_db --dump-date > db_bck.sql
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

# 5.docker setup MySQL 8.0.28

## 5.1 docker pull

```sh
docker pull mysql:8.0.28
docker images

```

##  5.2 config start up

初始化mysql密码，打包配置文件

```sh
docker run --name mysql --privileged=true -p 3306:3306 -e MYSQL_ROOT_PASSWORD='yourMySqlPassword' -d mysql:8.0.28 
docker ps
docker exec -it mysql /bin/bash
cd /etc/
# /etc/mysql 是容器里mysql的配置文件夹
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
tar -zxf mysql.tar.gz
mv mysql conf
# 设置时区
vi /data/mysql/conf/my.cnf
[mysqld]
default-time-zone = '+08:00'
# 停止当前 mysql 服务
docker stop mysql
docker rm mysql
```

##  5.3 normal start up

这么做，是为了将MySQL中存储的数据放在宿主机上，而不是放在容器里

```sh
# 启动容器，将 yourMySqlPassword 替换为自己的密码
docker run -dit \
--name mysql \
--privileged=true \
-p 33003:3306 \
-e MYSQL_ROOT_PASSWORD='yourMySqlPassword' \
-e LANG=C.UTF-8 \
-v /data/mysql/conf:/etc/mysql \
-v /data/mysql/data:/var/lib/mysql \
mysql:8.0.28
```

接下来连接数据库

```sh
# 进入容器
docker exec  -it mysql bash
# 连接数据库
mysql -uroot -p
```



如果想在宿主机上像连接本机数据库一样使用，还需要进行一些配置，

```sh
# 宿主机上需要安装MySQL client， 能够执行mysql命令
mysql -hlocalhost -uxxxx -p
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

##  5.4 创建用户

兼容老系统，使用 mysql_native_password

```sql
CREATE USER IF NOT EXISTS 'foo'@'%' IDENTIFIED WITH mysql_native_password BY 'fGB#sfsfswe*&%$3^3%GN';
grant all privileges on mysql.* to 'foo'@'%';
flush privileges;
```

MySQL8以上，使用新的密码策略

```sql
CREATE USER IF NOT EXISTS 'newuser'@'%' IDENTIFIED BY 'user_password';
GRANT ALL PRIVILEGES ON *.* TO 'newuser'@'%';
FLUSH PRIVILEGES;
```

出现错误 

```sh
ERROR 1396 (HY000): Operation CREATE USER failed for 'newuser'@'%
```

一般来说是该用户已经存在，可以通过以下语句查看

```sql
select host, user from mysql.user;
```

若需要删除，可执行

```sql
DROP USER 'foo'@'%';
```



##  5.5 禁止root用户远程登录

```sql
drop user 'root'@'%';
```

## 5.6 查看时区

```sql
SELECT @@global.time_zone;
```

# 6. docker setup MySQL 8.4.0

## 6.1 docker pull

```sh
docker pull mysql:8.4.0
docker images
```

##  6.2 config start up

初始化mysql密码，打包配置文件

```sh
# --privileged 参数可避免如下错误
# ls: cannot access '/docker-entrypoint-initdb.d/': Operation not permitted
docker run --name mysql84 --privileged -p 3306:3306 -e MYSQL_ROOT_PASSWORD='yourMySqlPassword' -d mysql:8.4.0

docker ps
docker exec -it mysql /bin/bash
cd /etc/
# /etc/mysql 是容器里mysql的配置文件夹
tar -czf mysql.tar.gz mysql
# 文件 /etc/my.cnf 和文件夹 /etc/my.cnf.d 是 mysql 8.4.0 的配置文件夹
tar -czf my.cnf.d.tag.gz my.cnf.d
exit
```

在宿主机上拷贝配置文件

```sh
su root
mkdir -p /data/mysql84/log
mkdir -p /data/mysql84/data
mkdir -p /data/mysql84/mysqld
docker cp mysql84:/etc/mysql.tar.gz /data/mysql84
docker cp mysql84:/etc/my.cnf.d.tar.gz /data/mysql84
docker cp mysql84:/etc/my.cnf /data/mysql84
cd /data/mysql84
tar -zxf mysql.tar.gz
tar -zxf my.cnf.d.tar.gz
mv mysql conf
# 设置时区
vi /data/mysql84/my.cnf
[mysqld]
default-time-zone = '+08:00'
# 停止当前 mysql 服务
docker stop mysql84
docker rm mysql84

docker run -dit \
	--name mysql84 \
	--privileged \
	-p 3307:3306 \
	-e MYSQL_ROOT_PASSWORD=yourMySqlPassword \
	-e LANG=C.UTF-8 \
	-v /data/mysql84/my.cnf:/etc/my.cnf \
	-v /data/mysql84/my.cnf.d:/etc/my.cnf.d \
	-v /data/mysql84/conf:/etc/mysql \
	-v /data/mysql84/data:/var/lib/mysql \
	-v /data/mysql84/mysqld:/var/run/mysqld \
	mysql:8.4.0
```

##  6.3 normal start up

这么做，是为了将MySQL中存储的数据放在宿主机上，而不是放在容器里

```sh
# 启动容器，将 yourMySqlPassword 替换为自己的密码
# --privileged 参数可避免如下错误
# ls: cannot access '/docker-entrypoint-initdb.d/': Operation not permitted
docker run -dit \
	--name mysql84 \
	--privileged \
	-p 3307:3306 \
	-e MYSQL_ROOT_PASSWORD=yourMySqlPassword \
	-e LANG=C.UTF-8 \
	-v /data/mysql84/my.cnf:/etc/my.cnf \
	-v /data/mysql84/my.cnf.d:/etc/my.cnf.d \
	-v /data/mysql84/conf:/etc/mysql \
	-v /data/mysql84/data:/var/lib/mysql \
	-v /data/mysql84/mysqld:/var/run/mysqld \
	mysql:8.4.0
```

接下来连接数据库

```sh
# 进入容器
docker exec  -it mysql84 bash
# 连接数据库
mysql -uroot -p
# 输入密码登录成功
```

如果想在宿主机上像连接本机数据库一样使用，还需要进行一些配置，

```sh
# 宿主机上需要安装MySQL client， 能够执行mysql命令
mysql -hlocalhost -uxxxx -p
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

##  6.4 创建用户

```sql
CREATE USER 'foo'@'%' IDENTIFIED WITH mysql_native_password BY 'fGB#sfsfswe*&%$3^3%GN';
grant all privileges on mysql.* to 'foo'@'%';
flush privileges;
```

##  6.5 禁止root用户远程登录

```sql
drop user 'root'@'%';
```

## 6.6 查看时区

```sql
SELECT @@global.time_zone;
```

# 7. shell 中执行sql语句

```sh
mysql -h 11.10.36.1 -u foo -p'fooxfdsf#$%' -s -e 'select count(1) from db.my_tb'
```

# 8. 源码编译安装

源码编译安装mysql5.7, 从网址 https://downloads.mysql.com/archives/community/ 下载mysql源码  mysql-5.7.9_source.tar.gz。目标平台 RHEL7.4

```sh
Product Version:5.7.9
Operating System:Source Code
OS Version:All Operating Systems(Generic)(Architechture Independent)
```

下载boost 库,  MySQL 源代码依赖 boost库。

```sh
wget  http://www.sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz
mkdir -p /usr/local/boost/
cp boost_1_59_0.tar.gz /usr/local/boost/
cd /usr/local/boost/
tar -zxf boost_1_59_0.tar.gz
```

安装编译工具

```sh
yum -y install gcc gcc-c++ ncurses ncurses-devel bison cmake
```

开始`cmake`编译

````sh
cd /data/app/mysql-5.7.9
cmake ./ -DWITH_BOOST=/usr/local/boost/boost_1_59_0
````

执行完成之后，再make

```sh
make -j 3 && make install
```

安装完的应用程序在目录 `/usr/local/mysql` 下。
