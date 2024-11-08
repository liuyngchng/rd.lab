# setup

env ubuntu 22.04

```sql
sudo apt install postgresql
# 设置新用户的密码
sudo passwd postgres
# 查看服务状态
sudo systemctl status postgresql
# 启动服务
sudo systemctl start postgresql
#切换用户
sudo -u postgres -i
# 查看数据库版本
psql --version
psql (PostgreSQL) 14.13 (Ubuntu 14.13-0ubuntu0.22.04.1)
# 连接本地数据库
psql
# 查看数据库清单
postgres=# \l
                                  List of databases
   Name    |  Owner   | Encoding |   Collate   |    Ctype    |   Access privileges   
-----------+----------+----------+-------------+-------------+-----------------------
 postgres  | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 | 
 template0 | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 | =c/postgres          +
           |          |          |             |             | postgres=CTc/postgres
 template1 | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 | =c/postgres          +
           |          |          |             |             | postgres=CTc/postgres
(3 rows)
# 创建数据库
create database class with owner=postgres encoding='UTF-8';
# 删除数据库
drop database class;
```

