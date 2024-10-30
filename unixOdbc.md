# odbc

unixodbc-dev , ODBC libraries for Unix

```sh
sudo apt-get install unixodbc*
```

odbcinst,  Helper program for accessing ODBC configuration file

```sh
sudo apt-get install odbcinst
```

安装相应数据库的ODBC的驱动

```sh
# ODBC driver for PostgreSQL
sudo apt install odbc-postgresql
```

# 查看配置

打印配置信息

```ini
odbcinst -j
-------------------------------------------
unixODBC 2.3.9
DRIVERS............: /etc/odbcinst.ini
SYSTEM DATA SOURCES: /etc/odbc.ini
FILE DATA SOURCES..: /etc/ODBCDataSources
USER DATA SOURCES..: /home/rd/.odbc.ini
SQLULEN Size.......: 8
SQLLEN Size........: 8
SQLSETPOSIROW Size.: 8
```

查询已经安装的驱动程序

```sh
odbcinst -q -d
-----------------------------------------------------
[PostgreSQL ANSI]
[PostgreSQL Unicode]
# 具体的驱动程序配置
cat  /etc/odbcinst.ini
------------------------------------------------------
[PostgreSQL ANSI]
Description=PostgreSQL ODBC driver (ANSI version)
Driver=psqlodbca.so
Setup=libodbcpsqlS.so
Debug=0
CommLog=1
UsageCount=1

[PostgreSQL Unicode]
Description=PostgreSQL ODBC driver (Unicode version)
Driver=psqlodbcw.so
Setup=libodbcpsqlS.so
Debug=0
CommLog=1
UsageCount=1
```

# 配置odbc驱动

配置 ODBC 调用某个指定数据源数据库的驱动程序

```ini
cat /etc/odbc.ini 
-------------------------------------------------------
[pg]
Description = PostgreSQL
# 此处的名称指向了 文件 /etc/odbcinst.ini 的某个具体so文件
Driver = PostgreSQL Unicode
#Driver = PostgreSQL
Trace = No
TraceFile = /tmp/pgodbc.log
Database = defaultdb
Servername = 11.53.58.76 
UserName = db_user_name
Password = db_password
Port = db_connect_port
```

# isql

用于通过已配置的 `unixOdbc` 数据源，连接相应的数据库服务器。

```sh
# 如果某个字段有特殊字符，可以在字段外部加单引号
isql -v <数据源名称> <用户名> <密码> -b -d <数据库名称> -t <表名>
isql -v 'pg' 'my_db_user_name' 'my_db_password'
+---------------------------------------+
| Connected!                            |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
+---------------------------------------+
```



# code

```c
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

int main() {
	SQLHENV hEnv = NULL;
    SQLHDBC hDbc = NULL;
    SQLHSTMT hStmt = NULL;
    SQLRETURN retcode;
    SQLLEN id;
    SQLCHAR menuID[32];
    SQLCHAR menuName[128];

    // 分配环境句柄
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    // 设置环境属性
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    // 分配连接句柄
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    // 连接数据库
    SQLRETURN ret;
    // 输入正确的用户名和密码
    // pg 对应 /etc/odbc.ini 中对应的数据源名称
    ret = SQLConnect(hDbc, (SQLCHAR*)"pg", SQL_NTS, (SQLCHAR*)"db_user_name", SQL_NTS, (SQLCHAR*)"db_password", SQL_NTS);
    if (ret != SQL_SUCCESS) {
        printf("fail to connect the database, error code %d\n\n", ret);
        return -1;
    }
    else {
        printf("connect to the database\n\n");
    }
    SQLHSTMT hStmt3 = NULL;
    SQLCHAR* create3 = (SQLCHAR*)"CREATE TABLE if not exists b(a varchar); ";
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt3);
    ret = SQLExecDirect(hStmt3, create3, SQL_NTS);
    if (ret != SQL_SUCCESS) {
        printf("execute failed, %s\n", create3);
        return -1;
    }
    else {
        printf("execute success, %s\n", create3);
    }

    SQLHSTMT hStmt4 = NULL;
    SQLCHAR* query4 = (SQLCHAR*)"INSERT INTO b VALUES ('中文读取'); ";
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt4);
    ret = SQLExecDirect(hStmt4, query4, SQL_NTS);
    if (ret != SQL_SUCCESS) {
        printf("execute failed, %s\n", query4);
        return -1;
    } else {
        printf("execute success, %s\n", query4);
    }
    SQLHSTMT hStmt9 = NULL;
    SQLCHAR* query9 = (SQLCHAR*)"select * from b;";
    // 分配语句句柄
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt9);
    // 执行查询
    ret = SQLExecDirect(hStmt9, query9, SQL_NTS);
	if (ret != SQL_SUCCESS) {
	   printf("execute failed, %s\n", query9);
	   return -1;
	} else {
	   printf("execute success, %s\n", query9);
	   ret = SQLBindCol(hStmt9, 1, SQL_C_CHAR, &menuID, sizeof(menuID), NULL);

	   // 检索数据
	   int row = 0;
	   while ((ret = SQLFetch(hStmt9)) == SQL_SUCCESS) {
		   printf("row %d, column a: %s\n", row++, menuID);
	   }

	   if (ret == SQL_NO_DATA) {
		   printf("no more data\n");
	   } else {
		   printf("failed to fetch data\n");
	   }
   }

   // 释放句柄
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt4);
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt9);

	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

   return 0;
}
```

