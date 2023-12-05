#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "peer.h"
#include "http.h"
#include "util.h"
#include "rgx.h"
#include "md5.h"
#include "const.h"

void httpreq(){
    char resp[8096] = {0};
    // myhttpreq(resp);
    char *ip = _CODEC_IP_;
    int port = _CODEC_PORT_;
    char *method="POST";
    char *path= _DEC_PATH_;
    char *bd = "{\"data\":\"6800EB03008188C1E000E1E55B9988776655443300110101000000000013FFFFFFFF03149A0FEC0100010001010000000000000000584AD45FB958C43F878129C981AA7587AC372EF3C21116C07E8EC7A55BB0BEE5BB50467015B1A81172D66EDD9B169E0C07311B501EB6EEBA5FB0A6D86BD1E15CEF058E5EBE6E57391861501FFA388EB61EB817A691A4DC92274579EAEAE681C9DC49F83CF6C7E5CD6631291A28D15DE8144EF11806CFC5C8D965282448E1F558A68852B243A1F657B2B68FC2E7DD721B8A1EE1254C36D8C7C3CAB9E08B2F7DBF8CA378C9477FC86D08A98B480D5E62464E189361000016\",\"transId\":\"T20211212091414-89F4BE38C6964238\",\"deviceId\":\"89F4BE38C6964238\",\"timestamp\":1635756025000}";
    // char reqeust[1024]={0};
    // req(resp);
    req(ip, port, method, path, bd, resp, sizeof(resp), 2);
    printf("[%s][%s-%d]resp body\n%s\n",gettime(), filename(__FILE__), __LINE__, resp);
}

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
    httpreq();
   
    // char *s = "{\"key1\":\"value1\", \"key2\":\"value2\", \"key3\":\"value3\"}";
    // char *jsonk = "key3";
    // char jsonv[256] = {0};
    // getjsonv(s, jsonv, jsonk, sizeof(jsonv));
    // printf("jsonv:%s\n", jsonv);
}

void testrgx() {
	char *txt="{\"code\":200,\"message\":\"[HAHAHHAHAHA] HEIHEIHEI!\",\"data\":{\"pcArrayMsgData\":[\"123456\"]}}";
	char tmp[512]={0};
	char *pattern="\\[\".*\"])";
	match(txt, pattern, tmp, sizeof(tmp));
	printf("match, %s\n", tmp);
	getjsonv(txt, tmp, "pcArrayMsgData", sizeof(tmp));
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


int main(int argc, char* argv[]) {
	testgetchararrayhex();
    return 0;
}

