#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "util.h"
#include "prsr.h"
#include "http.h"
#include "rgx.h"
#include "md5.h"
#include "const.h"

/**
 * decode data
 * reqbd, HTTP request body;
 * resp, HTTP response body;
 * n, 最多返回 n 个字符至 resp;
 * id, 数据标识ID;
 * m, 最多返回 m 个字符至 id
 **/
char *const decdt(const char *reqbd, char *const resp,
		int n, char *const id, int m);

/**
 * encode data
 * req, HTTP request body;
 * resp, HTTP response body;
 * n, 最多返回 n 个字符至 resp;
 * id, 数据标识ID;
 * m, 最多返回 m 个字符至 id
 **/
char *const coddt(const char *reqbd, char *const resp,
		int n, char *const id, int m);

/**
 * 编码数据
 * s,  source data
 * t,  taregt data ,output
 * n,  sizeof t
 * id, data id
 * m,  sizeof id
 * code, encode code
 */
char *const codtxt(const char *s, char *const t, int n,
		char *const id, int m, const char *code);

/**
 * get CHG pkt
 **/
char *const getpkt1(const char *s, char *const t);

/**
 * get VLV pkt
 **/
char *const getpkt2(const char *s, char *const t);

/**
 * get random
 * t, target data.
 */
char *const getrdm(char *const t);

int dispatch(const char *req, char *const resp) {
    char l0[100]={0};
    char method[5]={0};
    char uri[50]={0};
    char body[1024]={0};
    getln(req, l0, sizeof(l0), 0);
    getbody(req, body, sizeof(body));
    getmethod(l0, method, sizeof(method));
    geturi(l0, uri, sizeof(uri));
    printf(
    	_CRUL_FMT_,
        gettime(), filename(__FILE__), __LINE__, method,
		_SRV_IP_, _SRV_PORT_, uri, body
    );
    char *respbd;            // response body
    if (strcasecmp(method, "GET")==0){
        char tmp[512]={0}; 
        sprintf(tmp, _CMN_RESP_FMT_,uri ,gettime());
        respbd=tmp;
    } else {
        if (strcmp(uri, _URI1_)==0) {
            printf("[%s][%s-%d]%s matched\n", gettime(),
            	filename(__FILE__), __LINE__, _URI1_);
            char dt1[1024]={0};
            if(strncmp(body, "68", 2)==0) {
            	char dt[1024]={0};
				char id[32]={0};
				char json[1256];
				sprintf(json, _REQ_FMT_, body);
				decdt(json, dt, sizeof(dt), id, sizeof(id));
				printf("[%s][%s-%d]decdt,%s\n", gettime(),
					filename(__FILE__), __LINE__, dt);
				respbd= prsdt(dt, dt1, sizeof(dt1));
            } else {
				respbd= prsdt(body, dt1, sizeof(dt1));
            }
        } else if (strcmp(uri, _URI2_)==0) {
            printf("[%s][%s-%d]%s matched\n", gettime(),
            	filename(__FILE__), __LINE__, _URI2_);
            char dt[1024]={0};
            char id[32]={0};
            decdt(body, dt, sizeof(dt), id, sizeof(id));
            char dt1[1024] = {0};
            prsdt(dt, dt1, sizeof(dt1));
            printf("[%s][%s-%d]prs_dt,%s\n", gettime(),
            	filename(__FILE__), __LINE__, dt1);
			char rh[9]={0};
			getrdm(rh);
			char dt2[256]={0};
			sprintf(dt2, "%s%s%s", _STR3_, rh, _STR4_);
			printf("[%s][%s-%d]pln_dt,%s\n", gettime(),
				filename(__FILE__), __LINE__, dt2);
			codtxt(dt2, dt, sizeof(dt), id, sizeof(id), "37");
			char tmp[128]={0};
			char pld[1224]={0};
			char l[5]={0};
			getshorthex(strlen(dt)+8, l);
			sprintf(pld,"68%s030002%s8E1F16", l, dt);
			sprintf(tmp,_OP_FMT_, "", pld);
            respbd=tmp;
        } else if (strcmp(uri, _URI3_)==0) {
            printf("[%s][%s-%d]%s matched\n", gettime(),
            	filename(__FILE__), __LINE__, _URI3_);
            char dt[1024]={0};
            char id[32]={0};
            decdt(body, dt, sizeof(dt), id, sizeof(id));
            char dt1[1024] = {0};
            prsdt(dt, dt1, sizeof(dt1));
            printf("[%s][%s-%d]prs_dt\n%s\n", gettime(),
            	filename(__FILE__), __LINE__, dt1);
            char tmp[128]={0};
            sprintf(tmp,_OP_FMT_, id, "");
            respbd=tmp;
        } else if (strcmp(uri, _URI4_)==0) {
            printf("[%s][%s-%d]%s matched\n", gettime(),
            	filename(__FILE__), __LINE__, _URI4_);
            char dt[512]={0};
            char id[32]={0};
            coddt(body, dt, sizeof(dt), id, sizeof(id));
            printf("[%s][%s-%d]codedt\n%s\n",gettime(),
            	filename(__FILE__), __LINE__, dt);
            char tmp[1024]={0};
            char pld[1024]={0};
            char l[5]={0};
            getshorthex(strlen(dt)+8, l);
            sprintf(pld,"68%s030002%s8E1F16", l, dt);
            sprintf(tmp,_OP_FMT_, id, pld);
            respbd=tmp;
        } else if(strcmp(uri, _URI5_)==0) {
        	printf("[%s][%s-%d]%s matched\n", gettime(),
				filename(__FILE__), __LINE__, _URI5_);
        	char dt[1024]={0};
			char id[32]={0};
			char json[1256];
			sprintf(json, _REQ_FMT_, body);
			decdt(json, dt, sizeof(dt), id, sizeof(id));
			printf("[%s][%s-%d]decdt,%s\n", gettime(),
				filename(__FILE__), __LINE__, dt);
			char tmp[128]={0};
			sprintf(tmp,_OP_FMT_, "", dt);
			respbd=tmp;
        } else {
        	char tmp[512]={0};
        	sprintf(tmp, _CMN_RESP_FMT_,uri ,gettime());
        	respbd=tmp;
        }
    }
    sprintf(resp,
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %ld\r\n\r\n"
        "%s",
        strlen(respbd),respbd
    );
    return strlen(resp);
}

char *const decdt(const char *reqbd, char *const resp,
	int n, char *const id, int m) {
    char data[1024]={0};
    getjsonv(reqbd, data, _PAYLOAD_, sizeof(data));
    getjsonv(reqbd, id, _MID_, m);
    if (strlen(id)==0) {
    	char tmp[9]={0};
    	strcpy(id, getrdm(tmp));
    }
    char bd[2048] ={0};
    unsigned long t=gettimestamp();
    sprintf(bd, _DEC_FMT_,data, id, t, id, t);
    char respbd[1024]={0};
    req(_CODEC_IP_, _CODEC_PORT_, _POST_, _DEC_PATH_,
    	bd, respbd, sizeof(respbd), 2);
    printf("[%s][%s-%d][respbd]%s\n",gettime(),
    	filename(__FILE__), __LINE__, respbd);
    getjsonv(respbd, resp, _DEC_DT_KY_, n);
    return resp;
};

char *const coddt(const char *reqbd, char *const resp,
	int n, char *const id, int m) {
    char data[1024]={0};
    getjsonv(reqbd, data, _PAYLOAD_, sizeof(data));
    getjsonv(reqbd, id, _MID_, m);
    char funcode[8]={0};
    char msg[1024]={0};
    if(strncmp(data, _CHG_, strlen(_CHG_))==0) {
        strcat(funcode, "38");
        getpkt1(data, msg);
    } else {
        strcat(funcode, "01");
        getpkt2(data, msg);
    }
    char bd[2048] ={0};
    unsigned long t=gettimestamp();
    sprintf(bd,_COD_FMT_,funcode, msg, id, t, id, t);
    char respbd[1024]={0};
    req(_CODEC_IP_, _CODEC_PORT_, _POST_, _COD_PATH_,
    	bd, respbd, sizeof(respbd), 2);
    printf("[%s][%s-%d]respbd\n%s\n",gettime(),
    	filename(__FILE__), __LINE__, respbd);
    char tmp[512]={0};
    getjsonv(respbd, resp, _COD_DT_KY_, n);
    printf("[%s][%s-%d]getjsonv\n%s\n",gettime(),
    	filename(__FILE__), __LINE__, resp);
    return resp;
};

char *const codtxt(const char *s, char *const t, int n,
	char *const id, int m, const char *code) {
    char bd[2048] ={0};
    unsigned long time=gettimestamp();
    sprintf(bd,_COD_FMT_, code, s, id, time, id, time);
    char respbd[1024]={0};
    req(_CODEC_IP_, _CODEC_PORT_, _POST_, _COD_PATH_,
    	bd, respbd, sizeof(respbd), 2);
    printf("[%s][%s-%d]respbd\n%s\n",gettime(),
    	filename(__FILE__), __LINE__, respbd);
    getjsonv(respbd, t, _COD_DT_KY_, n);
    printf("[%s][%s-%d]getjsonv\n%s\n",gettime(),
    	filename(__FILE__), __LINE__, t);
    return t;
};

char *const getpkt1(const char *s, char *const t) {
    char *time=gettime2();
    sprintf(t, "%s%s%s%s", _STR1_, time, time, _STR2_);
    return t;
}

char *const getrdm(char *const t) {
	char r[4]={0};
	srand(time(NULL));
	for (int i = 0; i < sizeof(r); i++) {r[i]=rand()%255;}
	getchararrayhex(r, sizeof(r), t);
}

char *const getpkt2(const char *s, char *const t) {
    char *tmp="01010001";
    if (strcmp(s, _V00_)==0) {
        sprintf(t, "%s00", tmp);
    } else if(strcmp(s, _V01_)==0) {
        sprintf(t, "%s01", tmp);
    } else if(strcmp(s, _V02_)==0) {
        sprintf(t, "%s02", tmp);
    } else if(strcmp(s, _V03_)==0) {
        sprintf(t, "%s03", tmp);
    } else if(strcmp(s, _V04_)==0) {
        sprintf(t, "%s04", tmp);
    } else {
    	printf("[%s][%s-%d]err_input_%s\n",gettime(),
    		filename(__FILE__), __LINE__, s);
    	pthread_exit(NULL);
    }
    return t;
}
