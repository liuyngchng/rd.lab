# 1. ubuntu18 修改mysql数据库密码
```
sudo vim /etc/mysql/my.cnf
```
添加如下内容：
```
[mysqld]
skip-grant-tables=1
```
重启mysql服务
```
sudo service mysql restart
```
登录mysql
```
mysql
```
切换到mysql数据库,并修改root用户密码
```
use mysql;
update user set plugin='mysql_native_password' where user='root';
update user set authentication_string=password('123456') where user='root';
flush privileges;
exit;
```
修改配置并重启mysql服务
```
sudo sudo vim /etc/mysql/my.cnf把skip-grant-tables删除掉
sudo service mysql restart
```
# 2. binlog

## 2.1 登录到MySQL查看binlog

只查看第一个binlog文件的内容
	
show binlog events;  

查看指定binlog文件的内容  

`show binlog events in 'mysql-bin.000002';`

查看当前正在写入的binlog文件  

`show master status\G;`

获取binlog文件列表  

`show binary logs;`

## 2.2 config mysqld in ubuntu

修改 my.cnf  
查看my.cnf的位置 `file /etc/mysql/my.cnf`  
```
cd /etc/mysql/mysql.conf.d
vim mysqld.cnf

```
去掉以下三项的注释

```
server-id       	= 1
log_bin         	= /var/log/mysql/mysql-bin.log
expire_logs_days    = 10
max_binlog_size   	= 100M
```
执行

```
systemctl restart mysql 
mysql -uroot -p
show variables like '%log_bin%';
```
看到 log_bin | ON

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

## 2.3 browse binlog

sudo mysqlbinlog  -d dbname --base64-output=decode-rows  /var/log/mysql/mysql-bin.000001

## 3. general query log

修改 my.cnf  
查看my.cnf的位置 `file /etc/mysql/my.cnf`  
```
cd /etc/mysql/mysql.conf.d
vim mysqld.cnf

```
去掉以下两项的注释

```
general_log_file        = /var/log/mysql/mysql.log
general_log             = 1
```

查看日志

```
tail -f /var/log/mysql/mysql.log
```
