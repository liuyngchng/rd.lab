# setup

## setup

通过docker 镜像安装 oracle

```sh
docker pull registry.cn-hangzhou.aliyuncs.com/qida/oracle-xe-11g
# 映射 oracle的默认端口 1521
docker run -d -p 1521:1521 --name oracle11 registry.cn-hangzhou.aliyuncs.com/qida/oracle-xe-11g
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

## config

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
conn sys/oracle as sysdba
alter user system identified by system;    	# 修改system用户账号的密码
alter user sys identified by system;		# 修改sys用户账号的密码
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

## test

```sql
create table stu(name varchar(10),code varchar(10),subject varchar(10), score number(16,2));
```

# Ubuntu下安装sqlplus客户端

## 安装alien

alien可以实现ubuntu deb 包和 centos 的 rpm 包两者的互换

```sh
# 转换RPM为DEB
sudo alien test.rpm
# 换DEB至RPM
sudo alien -r test deb
```

安装 alien 包

```sh
sudo apt-get install alien
```

  ## 下载并安装sqlplus相关软件

```sh
alien -i oracle-instantclient*-basic*.rpm
alien -i oracle-instantclient*-sqlplus*.rpm
alien -i oracle-instantclient*-devel*.rpm
```

   ## 安装libaio.so

```sh
sudo apt-get install libaio1
```

## 配置

把下面的语句配置在oracle.conf中

```sh
sudo sensible-editor /etc/ld.so.conf.d/oracle.conf
# 以下语句根据实际环境二选一配置在oracle.conf中
/usr/lib/oracle/<your version>/client/lib/   	# 32位
/usr/lib/oracle/<your version>/client64/lib/	# 64位
```

## 更新动态库配置

执行如下语句

```sh
sudo ldconfig
```

## 连接数据库
```sh
sqlplus system/oracle@//ip:1521/sid
sqlplus64 system/oracle@//ip:1521/sid
```

# Sequence

## 查看用户序列

```sql
SELECT SEQUENCE_NAME,MIN_VALUE,MAX_VALUE,INCREMENT_BY,LAST_NUMBER FROM USER_SEQUENCES;
```

修改序列

```sql
ALTER SEQUENCE seq_name INCREMENT BY 1 MAXVALUE 10000 CYCLE     -- 到10000后从头开始 NOCACHE ;
```

## create sequence

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

# 解决 sequence 问题的思路

## 主键 number 统一设置 12 位

Number(12), 10^12 = 1000亿，远传表抄表、计费、充值记录等每天产生约 400 万条记录，一月 1.2 亿， 1年 12 亿条。

Number(12) 短期内的值域够用。

## sequence 最大值

远传表 sequence 最大值 设置为 10 ^12。

## 涉及到的 sequence
# record num

```sql
SELECT 
   table_name, count_rows(table_name) nrows 
FROM 
    user_tables 
ORDER BY 
    nrows DESC;
```
