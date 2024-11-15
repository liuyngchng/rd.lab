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
```

# connect test

```sql
#切换用户 必须切换，否则可能连接失败
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
# 授权远程登录
psql -U postgres -d postgres -h 127.0.0.1 -p 5432
#授予psql数据库指定用户postgers用户 指定连接后的数据库是postgres 指定主机地址为127.0.0.1 指定端口为5432可以登入数据库
# 退出控制台
\q
```

# create user & grant privilege

```sql
# 创建用户名和密码
create user user_name with password 'user_password';
alter user user_name with password 'user_password';
# 创建数据库
create database db_name owner user_name;
grant all privileges on database db_name to user_name;
```

# connect

```sql
# 连接数据库，无需切换用户 postgres
psql -U user_name -h ip -d database_name
#命令行输入密码
PGPASSWORD=$your_password psql -U user_name -h ip -d database_name
PGPASSWORD='123!@#$%^&456' psql -U test -h localhost -d test_dt

\l                   #查看数据库
\c databanse_name    #切换数据库
\d                   #查看表
\d table_name        #查看表结构
\q                   #退出
\dx                  #查看已安装插件
# create table
CREATE TABLE test_table (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    age INT
);
```

# config

```sql
#数据库设为只读
alter system set default_transaction_read_only to on;
select pg_reload_conf();
select pg_terminate_backend(pid) from pg_stat_activity where usename not in ('replicator', 'aurora', 'alicloud_rds_admin') and usename not like 'pg%' and pid <> pg_backend_pid();
 
#（回滚）数据库关闭只读
alter system set default_transaction_read_only to off;
select pg_reload_conf();
select pg_terminate_backend(pid) from pg_stat_activity where usename not in ('replicator', 'aurora', 'alicloud_rds_admin') and usename not like 'pg%' and pid <> pg_backend_pid();
 
#检查当前是否只读
show default_transaction_read_only;
```

# client GUI

```sh
# Setup the repository
#

# Install the public key for the repository (if not done previously):
curl -fsS https://www.pgadmin.org/static/packages_pgadmin_org.pub | sudo gpg --dearmor -o /usr/share/keyrings/packages-pgadmin-org.gpg

# Create the repository configuration file:
sudo sh -c 'echo "deb [signed-by=/usr/share/keyrings/packages-pgadmin-org.gpg] https://ftp.postgresql.org/pub/pgadmin/pgadmin4/apt/$(lsb_release -cs) pgadmin4 main" > /etc/apt/sources.list.d/pgadmin4.list && apt update'

#
# Install pgAdmin
#

# Install for both desktop and web modes:
sudo apt install pgadmin4

# Install for desktop mode only:
sudo apt install pgadmin4-desktop

# Install for web mode only: 
sudo apt install pgadmin4-web 

# Configure the webserver, if you installed pgadmin4-web:
sudo /usr/pgadmin4/bin/setup-web.sh
```

最终会弹出

```shell
Apache successfully restarted. You can now start using pgAdmin 4 in web mode at http://127.0.0.1/pgadmin4
```

可以通过 http://127.0.0.1/pgadmin4 进行配置。
