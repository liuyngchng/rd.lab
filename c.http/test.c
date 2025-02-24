#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cfg.h"
#include "peer.h"
#include "http.h"
#include "rgx.h"
#include "md5.h"
#include "const.h"
#include "utl.h"
#include "hiredis.h"
#include "hmac_md5.h"





void test() {
    // char *c1 = "hello";
    // char *c2="hilloworld";
    // int c=strncmp(c1, c2, 5);
    // printf("%d\n", c);
    // char t[64]={0};
    // printf("%s\n", gettime());
    // char *s = "GET / HTTP/1.1";
    // char *s = "key1=value1&key2=value2&key3=value3";
    // char t[20] = {0};
    // readline(s, t, sizeof(t), 2);
    // getmethod(s, t, 10);
    // geturi(s, t, 10);
    // printf("t, %s\n", t);
    //  startsrv();
    // char resp[8096]={0};
   
    // char *s = "{\"key1\":\"value1\", \"key2\":\"value2\", \"key3\":\"value3\"}";
    // char *jsonk = "key3";
    // char jsonv[256] = {0};
    // getjsonv(s, jsonv, jsonk, sizeof(jsonv));
    // printf("jsonv:%s\n", jsonv);
}

void testrgx() {
	char *txt="{\"code\":200,\"message\":\"[HAHAHHAHAHA] HEIHEIHEI!\",\"data\":{\"22345\":[\"123456\"]}}";
	char tmp[512]={0};
	char *pattern="\\[\".*\"])";
	match(txt, pattern, tmp, sizeof(tmp));
	printf("match, %s\n", tmp);
	getjsonv(txt, tmp, "22345", sizeof(tmp));
	printf("getjsonv, %s\n", tmp);
}

void testmd5() {
	char *s="12345678";
	unsigned char t[128]={0};
	md5sum(s, t);
	printf("%s, md5sum, %s\n", s, t);
	return;
}

int testgetcharhex(){
	char s=0x3c;
	char t[5]={0};
	getcharhex(s, t);
	printf("str=%s\n", t);
	return 0;
}

int testgetshorthex(){
	short i =(short)0xf3E2;
	char t[5]={0};
	printf("short_str=%s\n", getshorthex(i, t));
	return 0;
}

int testgetchararrayhex(){
	char s[4]={0x3c, (char)0xff, (char)0xea, 0x28};
	char t[9]={0};
	getchararrayhex(s,sizeof(s), t);
	printf("str=%s\n", t);
	return 0;
}

int testreadcfg() {
    char * filename = "./cfg.ini";
    printf("read file, %s\n", filename);
    getcfg(filename);
    return 0;
}

int testreadf() {
	char *filename = "./cfg.ini";
	char buf[1024]={0};
	printf("read cfg %s\n", readf(filename, buf, sizeof(buf)));
	return 0;
}

void testredis() {
	char *ip = "127.0.0.1";
	int port = 6379;
	redisContext *c = redisConnect(ip, port); // @suppress("Type cannot be resolved")
	if (c == NULL || c->err) { // @suppress("Field cannot be resolved")
	    if (c) {
	        printf("Error: %s\n", c->errstr);
	        return;
	    } else {
	        printf("Can't allocate redis context\n");
	    }
	}
	printf("connected to %s:%d\n", ip, port);
	redisReply *reply; // @suppress("Type cannot be resolved")
	char *cmd = "AUTH v3Jrh&3Z7^2JDGGN75^X";
	reply = (redisReply *)redisCommand(c, cmd);
	printf("auth result %s\n", reply->str);
	freeReplyObject(reply);

	cmd = "get mid_201982302000001105_did";
	reply = (redisReply *)redisCommand(c, cmd);
	printf("cmd result %s\n", reply->str);
	freeReplyObject(reply);

	redisFree(c);
}

/**
 * key =         0x0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b
  key_len =     16 bytes
  data =        "Hi There"
  data_len =    8  bytes
  digest =      0x9294727a3638bb1c13f48ef8158bfc9d
 *
 */
void test_hmac_md5() {
	printf("test hmac_md5\n");
	unsigned char key[17] = {0};
	for(int i=0; i< 16; i++) {
		key[i]=0x0b;
	}
	char hex_key[33]={0};
	printf("key=%s\n", char2hex(key, 16, hex_key));
	int key_len = 16;
	char *data = "Hi There";
	printf("data=%s\n", data);
	int data_len = 8;
	// 0x9294727a3638bb1c13f48ef8158bfc9d
	unsigned char digest[17] = {0};
	hmac_md5((unsigned char *)data, data_len, key, key_len, digest);
	printf("digest=");
	for(int i= 0; i< 16; i++) {
		printf("%02x", (int)digest[i]);
	}
	printf("\n");

}

void test_hex_char() {
	unsigned char a=0XF3;
	printf("a=%02X\n", a);
	char *b="32.888";
	printf("b=%lf\n", strtod(b, 0));
	char *c="fe";
	printf("c=%02X\n", (unsigned int)strtol(c, 0, 16));
}


int main(int argc, char* argv[]) {
	test_hmac_md5();
    return 0;
}

