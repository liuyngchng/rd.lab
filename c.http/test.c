#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cfg.h"
#include "peer.h"
#include "http.h"
#include "util.h"
#include "rgx.h"
#include "md5.h"
#include "const.h"





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
	printf("md5sum, %s\n", t);
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
	short i = 0xf3E2;
	char t[5]={0};
	printf("short_str=%s\n", getshorthex(i, t));
	return 0;
}

int testgetchararrayhex(){
	char s[4]={0x3c, 0xff, 0xea, 0x28};
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


int main(int argc, char* argv[]) {
	testreadcfg();
    return 0;
}

