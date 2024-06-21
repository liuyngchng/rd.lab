#include <signal.h>
#include <string.h>
#include <pthread.h>
#include "rdkafka.h"
#include "const.h"
#include "utl.h"

/**
  	producer
  	1.rd_kafka_conf_set设置全局配置
	2.rd_kafka_topic_conf_set设置topic配置
	3.rd_kafka_brokers_add设置broker地址，启动向broker发送消息的线程
	4.rd_kafka_new启动kafka主线程
	5.rd_kafka_topic_new建topic
	6.rd_kafka_produce使用本函数发送消息
	7.rd_kafka_poll调用回调函数
 */

static rd_kafka_t *rk;


int kfk_init(const char *broker) {
	rd_kafka_conf_t 		*conf;
	rd_kafka_conf_res_t 	res;
	rd_kafka_topic_conf_t 	*topic_conf;
	rd_kafka_resp_err_t 	err;
	char tmp[16];
	char errstr[512];
	conf = rd_kafka_conf_new();
	res = rd_kafka_conf_set(conf, "bootstrap.servers", broker, errstr, sizeof(errstr));
	if (res != RD_KAFKA_CONF_OK) {
		fprintf(stderr, "[%s][%7s-%3d][t-%4ld]kfk_err_set_broker,%s\n",
			gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
			errstr
		);
		return -1;
	}
	fprintf(stdout, "[%s][%7s-%3d][t-%4ld]kfk_st_bootstrap_servers, %s\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
		broker
	);
	//quick termination
	snprintf(tmp, sizeof(tmp), "%i", SIGIO);
	rd_kafka_conf_set(conf, "internal.termination.signal", tmp, NULL, 0);
	topic_conf = rd_kafka_topic_conf_new();
	rd_kafka_conf_set_default_topic_conf(conf, topic_conf);
	rd_kafka_type_t role = RD_KAFKA_PRODUCER;
	rk = rd_kafka_new(role, conf, errstr, sizeof(errstr));
	if(!rk){
		fprintf(stderr, "%%kfk_create_producer_err, %s\n", errstr);
		return -1;
	}
	fprintf(stdout, "[%s][%7s-%3d][t-%4ld]kfk_init_producer, %d\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
		role
	);
	// add more brokers
//	if (rd_kafka_brokers_add(rk, broker) == 0){
//	    fprintf(stdout, stderr, "%% No valid brokers specified\n");
//	    return -1;
//	}
	return 0;
}

int kfk_wrt(const char *topic, const char *key, const char *msg) {
	rd_kafka_topic_conf_t *topic_cfg = rd_kafka_topic_conf_new();
	rd_kafka_topic_t *rkt = rd_kafka_topic_new(rk, topic, topic_cfg);
	 if (!rkt) {
		fprintf(stderr, "[%s][%7s-%3d][t-%4ld]kfk_create_topic_err, %s\n",
			gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
			rd_kafka_err2str(rd_kafka_last_error())
		);
		rd_kafka_destroy(rk);
		return 1;
	}
	fprintf(stdout, "[%s][%7s-%3d][t-%4ld]kfk_set_topic, %s\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
		topic
	);
	int32_t partition = RD_KAFKA_PARTITION_UA;
	int msgflags = RD_KAFKA_MSG_F_COPY;

	int result = rd_kafka_produce(rkt, partition, msgflags,
		(void *)msg, strlen(msg), key, strlen(key), NULL
	);
	if (result != 0) {
		fprintf(stderr, "kfk_produce_topic_err %s: %s\n",
			rd_kafka_topic_name(rkt),
		    rd_kafka_err2str(rd_kafka_last_error())
		);
	}
	rd_kafka_resp_err_t resp = rd_kafka_flush(rk, 1000);
	if (resp != RD_KAFKA_RESP_ERR_NO_ERROR) {
		fprintf(stderr, "kfk_snd_msg_err, %d\n", resp);
		return resp;
	}
	fprintf(stdout, "[%s][%7s-%3d][t-%4ld]kfk_snd_msg_end, [topic]%s, [key]%s, [msg]%s\n",
		gettime(), filename(__FILE__), __LINE__, pthread_self()%_TIDL_,
		topic, key, msg
	);
	rd_kafka_topic_destroy(rkt);
	rd_kafka_destroy(rk);
	return 0;
}
