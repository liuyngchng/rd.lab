# 1.setup

## 1.1 setup

通过 `docker` 镜像安装 `oracle`, 版本为 Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production

```sh
docker pull registry.cn-hangzhou.aliyuncs.com/qida/oracle-xe-11g
# 映射 oracle的默认端口 1521
docker run -d -p 1521:1521 --name oracle11 registry.cn-hangzhou.aliyuncs.com/qida/oracle-xe-11g
docker run -d -p 1521:1521 --name oracle11 oracle11:1.0.0
docker exec -it oracle11 bash
sqlplus /nolog
# 使用sysdba角色登录sqlplus
conn sys/oracle as sysdba
# 查看当前实例下的数据库
select name from v$database;
# 查看有多少个数据表
select count(*) from user_tables;
# 退出sqlplus
exit
```

## 1.2 config

```sh
# 查看环境变量
echo $ORACLE_HOME
/u01/app/oracle/product/11.2.0/xe
# 创建软连接
ln -s $ORACLE_HOME/bin/sqlplus /usr/bin
# 可见软链已经创建成功
file /usr/bin/sqlplus
# 切换用户,带环境变量
su - oracle
# 登录sqlplus， 并修改sys、system用户密码
sqlplus /nolog
conn sys/oracle as sysdba					# usr:sys, pwd:oracle
alter user system identified by system;    	# 修改system用户账号的密码, usr:system, pwd:system
alter user sys identified by system;		# 修改sys用户账号的密码, usr:sys, pwd:system
create user test identified by test; 		# 创建内部管理员账号 test， 设置密码为 test
grant connect,resource,dba to test; 		# 将 dba 权限授权给内部管理员账号 test
ALTER PROFILE DEFAULT LIMIT PASSWORD_LIFE_TIME UNLIMITED; # 修改密码规则策略为密码永不过期
alter system set processes=1000 scope=spfile; # 修改数据库最大连接数据
# 修改以上信息后，需要重新启动数据库
conn /as sysdba
shutdown immediate; 						# 关闭数据库
startup;									# 启动数据库
exit;										# 退出 SQLPUS
```

注意，如果执行修改密码的时候出现： `database not open`，提示数据库没有打开，则执行

```sh
alter database open;
```

若提示 `xxxx: database not mounted`，则执行

```sh
alter database mount;
alter database open;
```

此时，数据库的默认用户名的密码已修改，并创建了测试用户 test，可以通过客户端工具进行连接了。

## 1.3 test

```sql
create table stu(name varchar(10),code varchar(10),subject varchar(10), score number(16,2));

create table up_rpt_dt(mid varchar(64),time varchar(64),dt CLOB);
```

# 2. Ubuntu下安装 sqlplus 客户端

## 2.1 安装alien

由于`oracle` 只提供了基于Red Hat Linux 版本的安装包， 安装包格式为`rpm` （(Red Hat Package Manager)）格式，在 Ubuntu 下安装需要进行转换。 

`alien`可以实现`Ubuntu` `deb` 包和 `CentOS` 的 `rpm` 包两者之间的互换

```sh
# 将 RPM 包转换为 DEB 包
sudo alien test.rpm
# 将 DEB 包转换为 RPM 包
sudo alien -r test deb
```

安装 `alien` 包

```sh
sudo apt-get install alien
```

  ## 2.2 下载并安装sqlplus相关包

在 oralce 官网下载 instant client， https://www.oracle.com/database/technologies/instant-client/downloads.html, 此处以oracle11.2为例，下载以下 rpm 包。

```sh
oracle-instantclient11.2-basic-11.2.0.4.0-1.x86_64.rpm
oracle-instantclient11.2-devel-11.2.0.4.0-1.x86_64.rpm
oracle-instantclient11.2-jdbc-11.2.0.4.0-1.x86_64.rpm
oracle-instantclient11.2-sqlplus-11.2.0.4.0-1.x86_64.rpm
```

安装 rpm 包

```sh
sudo alien -i oracle-instantclient*-basic*.rpm
sudo alien -i oracle-instantclient*-sqlplus*.rpm
sudo alien -i oracle-instantclient*-devel*.rpm
```

   ## 2.3 安装libaio.so

```sh
sudo apt-get install libaio1
```

## 2.4 配置动态库的路径

把下面的语句配置在oracle.conf中

```sh
sudo sensible-editor /etc/ld.so.conf.d/oracle.conf
# 以下语句根据实际环境二选一配置在oracle.conf中
/usr/lib/oracle/<your version>/client/lib/   	# 32位 OS
/usr/lib/oracle/<your version>/client64/lib/	# 64位 OS
```

以 oracle 11.2 为例，目录如下所示

```sh
/usr/lib/oracle/11.2/client64/lib/
```

## 2.5 更新动态库配置

执行如下语句

```sh
sudo ldconfig
```

## 2.6 连接数据库

* 命令格式如下：

```sh
sqlplus username/password@//ip:1521/sid
sqlplus64 username/password@//ip:1521/sid
```

* 查询 SID，可以通过以下三种方法中的一种。

（1）或者通过系统管理员登录通过SQL 语句查看 sid

```sql
select instance_name from V$instance; 
```

（2）在oracle 的 docker 容器中查询 sid 

```sh
# 进入docker 容器
docker exec -it oracle11 bash
# 查看进程
ps -ef | grep oracle -i
oracle 27 1  0 Oct25 ?  00:00:00 /u01/app/oracle/product/11.2.0/xe/bin/tnslsnr LISTENER -inherit
# 查看环境变量
echo $ORACLE_SID
XE
```

（3）执行语句

```sh
cat /etc/oratab
XE:/u01/app/oracle/product/11.2.0/xe:N
```

"XE" 即为 sid。

* 运行命令

```sh
# 指定数据库服务器计算机的主机名或ip地址的普通用户连接
sqlplus64 test/test@//localhost:1521/XE
```

# 3. SQLPlus及常用 SQL

## 3.1 登录

常用的命令如下所示

```sh
# 先登录sqlplus， 再连接数据库
sqlplus /nolog 						# 步骤1： 登录到sqlplus，还未登录到数据库，若要登录到数据库，执行步骤2
conn username/password				# 步骤2：注意，conn /as sysdba 不写用户名和密码的连接属于操作系统认证
# 普通用户连接到默认数据库（schema与用户名同名）
sqlplus username/password 
# 仅限sys和system用户（具有数据库管理权限的用户），连接到默认数据库
sqlplus username/password as sysdba 
# 指定net服务名的普通用户连接
sqlplus username/password@net_service_name  
#  指定数据库服务器计算机的主机名或ip地址的普通用户连接
sqlplus username/password@//host:port/sid
# 退出
quit;
```

## 3.2 查看版本

```sql
SELECT * FROM v$version;
```

## 3.3 查看 SID

```sql
select instance_name from V$instance; 
```

## 3.4 查看表信息

* 查看表结构

```sql
# 简单查看字段名称及类型
desc table_name;

# 查看创建表的SQL语句
# 设置显示long,lob等型字段的长度,默认为80，设置为一个较大的值
set long 5000000;
# 查询，table_name 需要大写
SELECT DBMS_METADATA.GET_DDL('TABLE','table_name') FROM DUAL;

SELECT DBMS_METADATA.GET_DDL('TABLE','UTL_RECOMP_COMPILED') FROM DUAL;
```

* 查看表清单

```sql
# 查看所有的表:
SELECT * FROM tab;
SELECT * FROM dba_tables;
SELECT * FROM dba_objects;
SELECT * FROM cat;
# 查看用户建立的表 :
SELECT table_name from user_tables;  	# 当前用户的表
SELECT table_name from all_tables;  	# 所有用户的表
SELECT table_name from dba_tables;  	# 包括系统表
SELECT * from user_indexes; 			# 可以查询出所有的用户表索引
```

* 查看索引

```sql
# 查找表的所有索引(包括索引名，类型，构成列),表名为 table_name
SELECT t.*,i.index_type FROM user_ind_columns t,user_indexes i WHERE t.index_name = i.index_name AND t.table_name = i.table_name AND t.table_name = 'table_name';
# 查找表的主键(包括名称，构成列)
SELECT cu.* FROM user_cons_columns cu, user_constraints au WHERE cu.constraint_name = au.constraint_name AND au.constraint_type = 'P' AND au.table_name = 'table_name';
```

## 3.5 导入导出数据

* 导出文本数据
我们使用SPOOL方法，将数据库中的表导出为文本文件的时候会采用两种方法，如下述：

```sql
# 导出
set trimspool on
set linesize 120
set pagesize 2000
set newpage 1
set heading off
set term off
spool 导出路径+文件名
select col1||','||col2||','||col3||','||col4||'..' from tablename;
spool off
# 导入
sqlldr username/password@sid control=*.ctl
# *.ctl格式为：
load
infile"d://test.txt"
……
```

* 导入导出二进制数据

```sql
# 导出
exp system/123456 file= C:person.dmp full=y
# 导入
imp 管理员账号/密码 file=C:person.dmp fromuser=用户名
```

#  4. GUI Client

客户端图形化连接工具，可以使用 Oracle 官方提供的免费工具 `Oracle SQL Developer`， 可通过链接

https://www.oracle.com/database/sqldeveloper/ 下载， 支持 Windows 以及 linux。

# 5. JDBC

oracle JDBC 连接字符串格式如下所示：

```shell
# 使用 service_name
jdbc:oracle:thin:@//host:port/service_name 
# 使用 service id
jdbc:oracle:thin:@host:port:SID 
```



# 6. Sequence

## 6.1 查看用户序列

```sql
SELECT SEQUENCE_NAME,MIN_VALUE,MAX_VALUE,INCREMENT_BY,LAST_NUMBER FROM USER_SEQUENCES;
```

修改序列

```sql
ALTER SEQUENCE seq_name INCREMENT BY 1 MAXVALUE 10000 CYCLE     -- 到10000后从头开始 NOCACHE ;
```

## 6.2 create sequence

```sql
CREATE SEQUENCE 序列名

　　[INCREMENT BY n]					// 步长，如果省略，则默认为1，如果出现负值，则代表Oracle序列的值是按照此步长递减的

　　[START WITH n]						// 初始值(即产生的第一个值)，默认为1
　　[{MAXVALUE/ MINVALUE n| NOMAXVALUE}]   // 最大值。选项NOMAXVALUE是默认选项，代表没有最大值定义，这时对于递增Oracle序列，系统能够产生的最大值是10的27次方;对于递减序列，最大值是-1 
　　				// 最小值。选项NOMAXVALUE是默认选项，代表没有最小值定义，这时对于递减序列，系统能够产生的最小值是?10的26次方;对于递增序列，最小值是1
　　[{CYCLE|NOCYCLE}]   		// CYCLE代表循环，NOCYCLE代表不循环。如果循环，则当递增序列达到最大值时，循环到最小值;对于递减序列达到最小值时，循环到最大值。如果不循环，达到限制值后，继续产生新值就会发生错误
　　[{CACHE n| NOCACHE}];   // 存放序列的内存块的大小，默认为20。NOCACHE表示不对序列进行内存缓冲。对序列进行内存缓冲，可以改善序列的性能
```

example

```sql
 create sequence seq_name increment by 1 start with 1;
```

使用

```sql
insert into t1 values(seq_name.nextval, 1, 1);
```

# 7. 解决 sequence 问题的思路

## 7.1 主键 number 统一设置 12 位

Number(12), 10^12 = 1000亿，远传表抄表、计费、充值记录等每天产生约 400 万条记录，一月 1.2 亿， 1年 12 亿条。

Number(12) 短期内的值域够用。

## 7.2 sequence 最大值

远传表 sequence 最大值 设置为 10 ^12。

## 7.3 涉及到的 sequence
# 8. record num

```sql
SELECT 
   table_name, count_rows(table_name) nrows 
FROM 
    user_tables 
ORDER BY 
    nrows DESC;
```
