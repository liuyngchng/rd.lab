/**
 * 获取源代码并编译
 * git clone git@github.com:redis/hiredis.git
 * cd hiredis
 * make
 * 编译
 * gcc test.c -lhiredis -L ./hiredisdir -I ./hiredisdir
 * 运行
 * # 添加动态库加载路径
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${lib_hiredis_so_dir}
   # 使配置更改生效
   sudo ldconfig
   # 进入ELF 可执行文件的目录
   cd ${elf_dir}
   # 构建软链
   ln -s libhiredis.so libhiredis.so.1.2.1-dev
   # 查看是否缺少其他动态库
   ldd ./a.out
   # 执行可执行文件
   ./a.out
 */
#include <string.h>
#include <pthread.h>
#include "hiredis.h"
#include "utl.h"

/**
 * a redis ctx
 */
static redisContext *ctx;

/**
 * exec redis command
 */
static int exec(const char *cmd, redisReply *reply);

int rds_con(const char* ip, const int port, const char *psw) {
    ctx = redisConnect(ip, port);
    if (ctx == NULL || ctx->err) {
        if (ctx) {
            printf("[%s][%s-%d][t-%lu]err: %s\n", gettime(),
				filename(__FILE__), __LINE__, pthread_self(),
				ctx->errstr
			);
        } else {
            printf("[%s][%s-%d][t-%lu]alloc_rds_ctx_err\n", gettime(),
				filename(__FILE__), __LINE__, pthread_self()
			);
        }
        return -1;
    }
    if (psw == NULL) {
    	return 0;
    }
    redisReply *reply = redisCommand(ctx, "AUTH %s", psw);
    if (reply->type == REDIS_REPLY_ERROR) {
        printf("[%s][%s-%d][t-%lu]auth_failed_for_%s, cause: %s\n", gettime(),
			filename(__FILE__), __LINE__, pthread_self(),
			psw, reply->str
		);
        freeReplyObject(reply);
//        redisFree(ctx);	// cause segment fault
        return -1;
    }
    freeReplyObject(reply);
    printf("[%s][%s-%d][t-%lu]connect_and_auth_to %s:%d, %s\n", gettime(),
		filename(__FILE__), __LINE__, pthread_self(),
		ip, port, psw
	);
	return 0;

}
int rds_set(const char *key, const char *value) {
	char cmd[256] = {0};
	sprintf(cmd, "set %s %s", key, value);
	redisReply *reply ;
	//= (redisReply *)redisCommand(ctx, cmd);
	int result = exec(cmd, reply);
	freeReplyObject(reply);
	return result;
}

int rds_get(const char *key, char *value) {
	char cmd[256] = {0};
	sprintf(cmd, "get %s", key);
	redisReply *reply = (redisReply *)redisCommand(ctx, cmd);
	printf("[%s][%s-%d][t-%lu]rds_get_success, %s, %s\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self(),
		cmd, reply->str
	);
	strcpy(value, reply->str);
	freeReplyObject(reply);
	return 0;
}

static int exec(const char *cmd, redisReply *reply) {
	reply = (redisReply *)redisCommand(ctx, cmd);
	///TODO: check fail case
//	if (reply->type != REDIS_OK) {
//		printf("[%s][%s-%d][t-%lu]cmd_exec_failed, %s, cause: %s\n", gettime(),
//			filename(__FILE__), __LINE__, pthread_self(),
//			cmd, reply->str
//		);
//		freeReplyObject(reply);
////		redisFree(ctx);
//		return -1;
//	}
	printf("cmd_exec_success, %s, %s\n", cmd, reply->str);

//	redisFree(ctx);
	return 0;
}

