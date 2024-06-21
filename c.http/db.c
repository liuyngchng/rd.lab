/**
 * gcc db.c util.c -lmysqlclient -I /home/rd/software/mysql-8.0.28-linux-glibc2.12-x86_64/include  -L/home/rd/software/mysql-8.0.28-linux-glibc2.12-x86_64/lib
 * develop manual
 * https://dev.mysql.com/doc/c-api/8.2/en/c-api-introduction.html
 * # 添加动态库加载路径
 * export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${lib_mysqlclient_so_dir}
 * # 使配置更改生效
 * sudo ldconfig
 * # 进入ELF 可执行文件的目录
 * # 查看是否缺少其他动态库
 * ldd ./a.out
 * # 执行可执行文件
 * ./a.out
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "const.h"
#include "utl.h"
#include "mysql.h"
#include <errno.h>

#define DB_MAX_ROW 10

static MYSQL *my;
int db_con(const char *hst, const char *usr, const char *pwd,
	const char *db, const int prt) {
	fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_cli_ver, %s\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
		mysql_get_client_info()
	);
	my = mysql_init(NULL);
	if (0 == mysql_real_connect(my, hst, usr, pwd, db, prt, NULL, 0)) {
		fprintf(stderr, "[%s][%7s-%3d][t-%4ld]db_con_fail, %s, %s, %s, %s, %s, %d\n",
			gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
			mysql_error(my), hst, usr, pwd, db, prt
		);
		return -1;
	}
	fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_con_suc, srv_ver, %s\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
		mysql_get_server_info(my)
	);
	return 0;
}

int db_select(const char *sql, char *const dt) {
	fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exec_stt, %s\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
		sql
	);
    mysql_set_character_set(my, "utf8");
    if (0 != mysql_query(my, sql)) {
        fprintf(stderr, "[%s][%7s-%3d][t-%4ld]db_exec_fail, %s, %s\n",
			gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
			mysql_error(my), sql
		);
        return -2;
    }
    fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exec_suc\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_
	);
    MYSQL_RES *res = mysql_store_result(my);
    int rows = mysql_num_rows(res);
    int cols = mysql_num_fields(res);
//    fprintf(stdout, "[%s][%7s-%3d][t-%4ld]row %d, col %d\n",
//		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
//		rows, cols
//	);		// for dbg
    MYSQL_FIELD *fields = mysql_fetch_field(res);
//    for (int i = 0; i < cols; i++) {
//        fprintf(stdout, "%s\t", fields[i].name);	// for dbg
//    }
//    fprintf(stdout, "\n");
    for (int i = 0; i < rows; i++) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if(i != 0) {
			strcat(dt, "\n");
		}
        for (int j = 0; j < cols; j++) {
        	if(j !=0) {
        		strcat(dt, "\t");
        	}
//            fprintf(stdout, "%s\t",row[j]);	//for dbg
            strcat(dt, row[j]);
        }
        if (i > DB_MAX_ROW) {
        	fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exceed_db_max_row, dt_truncate_return\n",
				gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_
			);
            break;
        }
    }
    mysql_free_result(res);
    mysql_close(my);
    fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exec_fin\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_
	);
    return 0;
}

int db_insert(const char *sql) {
    mysql_set_character_set(my, "utf8");
    fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exec_query\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_
	);
    int result = mysql_query(my, sql);
    if (0 != result) {
        fprintf(stderr, "[%s][%7s-%3d][t-%4ld]db_exec_fail_err, %s, %s\n",
			gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
			mysql_error(my), sql
		);
        return 2;
    }
    mysql_close(my);
    fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exec_fin\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_
	);
    return 0;
}

int db_update(const char *sql) {
    mysql_set_character_set(my, "utf8");
    fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exec_query\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_
	);
    int result = mysql_query(my, sql);
    if (0 != result) {
        fprintf(stderr, "[%s][%7s-%3d][t-%4ld]db_exec_fail, %s, %s\n",
			gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
			mysql_error(my), sql
		);
        return 2;
    }
    mysql_close(my);
    fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exec_fin\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_
	);
    return 0;
}

int db_delete(const char *sql) {
    mysql_set_character_set(my, "utf8");
    fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exec_query\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_
	);
    int result = mysql_query(my, sql);
    if (0 != result) {
        fprintf(stderr, "[%s][%7s-%3d][t-%4ld]db_exec_fail, %s, %s\n",
			gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
			mysql_error(my), sql
		);
        return 2;
    }
    mysql_close(my);
    fprintf(stdout, "[%s][%7s-%3d][t-%4ld]db_exec_fin\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_
	);
    return 0;
}
