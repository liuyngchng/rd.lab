#include <string.h>
#include <pthread.h>
#include "hiredis.h"
#include "utl.h"
#include "const.h"

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
            fprintf(stderr, "[%s][%7s-%3d][t-%4ld]rds_connect_err: %s\n", gettime(),
				filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
				ctx->errstr
			);
        } else {
            fprintf(stdout, "[%s][%7s-%3d][t-%4ld]alloc_rds_ctx_err\n", gettime(),
				filename(__FILE__), __LINE__, pthread_self()%_TIDL_
			);
        }
        return -1;
    }
    if (psw == NULL) {
    	return 0;
    }
    redisReply *reply = redisCommand(ctx, "AUTH %s", psw);
    if (reply->type == REDIS_REPLY_ERROR) {
        fprintf(stderr, "[%s][%7s-%3d][t-%4ld]err_auth_failed_for_%s, cause: %s\n", gettime(),
			filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
			psw, reply->str
		);
        freeReplyObject(reply);
//        redisFree(ctx);	// cause segment fault
        return -1;
    }
    freeReplyObject(reply);
	return 0;

}
int rds_st(const char *key, const char *value) {
	char cmd[256] = {0};
	sprintf(cmd, "set %s %s", key, value);
	redisReply *reply ;
	//= (redisReply *)redisCommand(ctx, cmd);
	int result = exec(cmd, reply);
	freeReplyObject(reply);
	return result;
}

int rds_gt(const char *key, char *value) {
	char cmd[256] = {0};
	sprintf(cmd, "get %s", key);
	redisReply *reply = (redisReply *)redisCommand(ctx, cmd);
//	fprintf(stdout, "[%s][%7s-%3d][t-%4ld]rds_get_success, %s, %s\n",
//		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
//		cmd, reply->str
//	);
	if (NULL == reply->str) {
		fprintf(stderr, "[%s][%7s-%3d][t-%4ld]rds_get_key_value_NULL, %s\n",
			gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
			key
		);
	} else {
		strcpy(value, reply->str);
	}
	freeReplyObject(reply);
	return strlen(value);
}

static int exec(const char *cmd, redisReply *reply) {
	reply = (redisReply *)redisCommand(ctx, cmd);
	///TODO: check fail case
//	if (reply->type != REDIS_OK) {
//		fprintf(stdout, "[%s][%7s-%3d][t-%4ld]cmd_exec_failed, %s, cause: %s\n", gettime(),
//			filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
//			cmd, reply->str
//		);
//		freeReplyObject(reply);
////		redisFree(ctx);
//		return -1;
//	}
	fprintf(stdout, "cmd_exec_success, %s, %s\n", cmd, reply->str);

//	redisFree(ctx);
	return 0;
}

