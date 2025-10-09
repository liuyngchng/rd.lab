# 1. Dameng DB
## 1.1 setup dmdb in GUI mode

```sh
cd dameng_x86
./DMInstall.bin
cd /home/rd/Desktop/DMDBMS
```
add path for cmd

```sh
sudo vim /etc/profile
PATH=/home/rd/dmdbms/bin:$PATH
export PATH
source /etc/profile
```
start DmServiceDMSERVER, to run

```sh
DmServiceDMSERVER start
```
test you dm server
```sql
disql SYSDBA/SYSDBA@localhost:5236
create table t(a int,b int,c int);
insert into t values(1,2,3);
insert into t values(11,22,33);
commit;
quit
```

## 1.2 setup dmdb in terminal (no GUI)
install db server
```sh
cd dameng_x86
./DMInstall.bin -i
cd /opt/dmdbms/bin
```
init server instance
```sh
./dminit
input system dir: /opt/dmdbms/data			// 数据文件存放路径
input db name: dm5252						//实例/数据库名
input port num: 5252						//服务端口
input page size(4,8,16,32): 8				//数据库页大小
input extent size(16,32): 32				//扩展大小
input sec priv mode(0,1): 0					//安全特权模式
input time zone(-12:59,+14:00): +8			//时区 ，选东8区
input case sensitive? ([Y]es,[N]o): Y		//标识符是否区分大小写
which charset to use? (0[GB18030],1[UTF-8],2[EUC-KR]): 1	//字符集
length in char? ([Y]es,[N]o): Y				//字符长度, N for DBXXFW.dmp
enable database encrypt? ([Y]es,[N]o): N	//是否启用数据库加密
input slice size(512,4096): 512
page check mode? (0/1/2): 0					//是否开启页页检查模式
input elog path:							//日志路径
auto_overwrite mode? (0/1/2): 0				//自动覆盖模式
```
注册服务
```sh
cd /opt/dmdbms/script/root
dm_service_installer.sh -t dmserver -i /opt/dmdbms/test/dm.ini -p TEST
cd /opt/dmdbms
chmod -R 777 test
cd /opt/dmdbms/bin
./DmServiceTEST start	#启动
./DmServiceTEST stop	#停止
./DmServiceTEST status	#查看服务状态

```
start port listening, 若初始化的数据库名称为test，则启动时执行
```sh
cd /opt/dmdbms/bin
nohup ./dmserver ../data/test/dm.ini > dm.log 2>&1 &
```
delete server instance named 'test' 
```sh
cd dmdbms/script/root
./dm_service_uninstaller.sh -n test
cd /dmdbms/data
rm -fr test
ps -ef | grep dmserver
kill -9 xxxx
```
docker

```sh
docker run -dit -v /home/rd/work/dky:/dky --network host --name test 209c24a7f1c1
```
create user
```sh
create user DBXXFW identified by DBXXFW123456;
grant DBA to DBXXFW;

```
import data
```sh
./dimp SYSDBA/SYSDBA IGNORE=N ROWS=Y FULL=Y file="/dky/xxfw/DBXXFW.dmp"
```
## 1.3 setup unixODBC

```sh
tar -zxf unixODBC-2.3.9.tar.gz
cd unixODBC-2.3.9/
./configure
make
sudo make install
```
add so to lib path
```sh
sudo vim /etc/profile
LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
sudo ldconfig
```
then, run
```sh
odbcinst -j
```
you can see
```sh
unixODBC 2.3.9
DRIVERS............: /usr/local/etc/odbcinst.ini
SYSTEM DATA SOURCES: /usr/local/etc/odbc.ini
FILE DATA SOURCES..: /usr/local/etc/ODBCDataSources
USER DATA SOURCES..: /home/rd/.odbc.ini
SQLULEN Size.......: 8
SQLLEN Size........: 8
SQLSETPOSIROW Size.: 8
```
## 1.4 config odbc

sudo vim /user/local/etc/odbcinst.ini,
content as following
```sh
[DM7 ODBC DRIVER]
Description = ODBC DRIVER FOR DM7
Driver		= /home/rd/dmdbms/bin/libdodbc.so
Setup		= /lib/libdmOdbcSetup.so
```
sudo vim /usr/local/etc/odbc.ini,
content as following

```sh
[dm]
Description = DM ODBC DSN
Driver		= DM7 ODBC DRIVER
SERVER		= localhost
UID			= SYSDBA
PWD			= SYSDBA
TCP_PORT 	= 5236
```
## 1.5 config LD_LIBRARY_PATH
add dm libdodbc.so to LD_LIBRARY_PATH
sudo vim /etc/profile
```sh
LD_LIBRARY_PATH=/home/rd/dmdbms/bin:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
```
and run
```sh
sudo ldconfig
```
now test your odbc config, run
 ```sh
 isql -v dm
 ```
you can see
```sh
+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
```
if you see
`Can't open lib '/home/rd/dmdbms/bin/libdodbc.so' : file not found`,
but the so file really exit, that is because of LD_LIBRARY_PATH config
for dmdb lib and odbc lib is not valid or haven't be activated.
check and fix the problem,you can run

```sh
ldd theSoCalledMissedSoFile.so
```
to check what's wrong.

#  2. DM DB case sensitive  
##  2.1 表名、字段名

（1）在设置为大小写敏感的库中进行查询的时候，可能经常会遇到无效的表名或列名的问题，  下面针对这种情况进行说明。  

（2）如果使用DM管理工具图形界面建表的话建议表名和字段名都使用大写，因为使用图形界面建表相当于使用语句建表的时候加了双引号的，会固定住大小写。 

（3）如果写成小写，那么就是小写了，在查询的时候也需要加双引号，否则就有可能报无效的表名或列名的问题，比较麻烦；

（4）如果使用脚本建表的话，如果表名和字段名没有加双引号的话都会被系统自动转换成大写  

如：create table test(a int)；系统会自动把表名test，字段名a转换成大写，处理方式与Oracle数据库一致  
所以针对大小写敏感的库，在使用建表脚本或者通过查询脚本进行查询的时候建议统一不要写双引号，  
让系统统一自动都转化为大写；

（5） 以上几点主要针对大小写敏感的库而言，大小写不敏感的库则不存在上述问题。  

（6）基于以上几点，在初始化数据库的过程中就可以对字符串比较大小写敏感这个参数做出合理的选择了。
##  2.3 SQL

```sql
select * from v$version;

-- 查看所有表空间（schema）---
select name from v$tablespace;

-- 查看所有表空间物理文件 --
select * from v$datafile;

-- 查看表定义 --
CALL SP_TABLEDEF('schema_name', 'table_name');

-- 执行sql脚本 --
SQL> start /a/b/c.sql

-- disql 执行SQL脚本 (./test.sql为文件路径)--
disql SYSDBA/SYSDBA@localhost \` ./test.sql 

-- 编辑SQL脚本 --
SQL> edit /a/b/c.sql


-- 查看表空间物理文件的名称及大小 --
select t.name tablespace_name,t.id file_id,d.path file_name,d.total_size*sf_get_page_size()/1024/1024||'m' total_space from v$tablespace t, v$datafile d where t.id=d.group_id;

-- 查看数据库实例信息 --
select name inst_name from v$instance;

-- 确定高负载的 SQL(慢查询) --
select * from v$long_exec_sqls;


select owner, table_name, tablespace_name from dba_tables where owner = 'sysdba';

-- 查询当前用户下所有的表名--
select NAME from sysobjects where "SUBTYPE$"='UTAB'AND SCHID=(SELECT ID FROM sysobjects WHERE NAME=USER AND TYPE$='SCH');
-- 有开源的客户端工具可以连DM ?--
-- 基于JDBC的工具就可以，比如：SQuirrel SQL、DbVisualizer --
--  create user --
create user USNAME IDENTIFIED BY PSWORD;
-- grant role to user --
grant DBA to username;
-- delete user and default schema --
DROP USER username cascade;

-- 创建schema(注意：必须要有第二行的斜杠) --
create schema SCHEMA_NAME authorization SCHEMA_OWNER; 
/

-- 删除 schema --
drop schema SCHEMA_NAME

-- 查询所有的表以及其拥有者 --
select distinct table_name, owner from all_tables; 

-- 查询所有的schema --
select distinct object_name  from all_objects where object_type = 'SCH';

-- 切换schema --
set schema schema_name;		//前提条件是schema_name 的owner 属于当前用户，否则无法切换；
-- 查看schema编码 --  

-- 0 表示 GB18030，1 表示 UTF-8，2 表示 EUC-KR  --

select SF_GET_UNICODE_FLAG();  
select  UNICODE ();  
 
```



字符集在安装初始化库的时候指定，设定后不可更改  



# 3. DM8

## 3.1 server

docker

```
docker pull docker.1ms.run/danilaworker/damengdb:8.1.2
```

Official damengdb DM8 documentation about docker installation:

https://eco.dameng.com/document/dm/zh-cn/start/dm-install-docker.html

run

```sh
# 通过环境变量设置密码
docker run -d -p 5236:5236 \
  --name dm8 \
  --privileged=true \
  -e PAGE_SIZE=16 \
  -e LD_LIBRARY_PATH=/opt/dmdbms/bin \
  -e INSTANCE_NAME=dm8 \
  -e SYSDBA_PWD=SYSDBA001 \
  -v /data/dm8:/opt/dmdbms/data \
  docker.1ms.run/danilaworker/damengdb:8.1.2
```

## 3.2 dmPython 客户端

安装依赖

```sh
# dmPython 底层需要依赖 dm客户端的 so文件。
pip install dmPython
```

复制 so文件到指定目录

```sh
# 复制的bin目录下的so文件到宿主机目录 /data/dm8_client
mkdir -p /data/dm8_client
docker cp dm8:/opt/dmdbms/bin /data/dm8_client
cd /data/dm8_client/
mv bin/* ./
rm -fr bin

# 编辑 ~/.bashrc
echo 'export DM_HOME=/data/dm8_client' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/data/dm8_client:$LD_LIBRARY_PATH' >> ~/.bashrc

# 立即生效
source ~/.bashrc

# 验证环境变量
echo $DM_HOME
echo $LD_LIBRARY_PATH
```

