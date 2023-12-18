/**
 * HTTP URI pattern match
 * @author whoami@hotmail.com
 * @since 2022-06-01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "prsr.h"
#include "http.h"
#include "rgx.h"
#include "md5.h"
#include "const.h"
#include "utl.h"

extern int _SRV_PORT_;
extern char _SRV_IP_[_CFG_LEN_];
extern pthread_key_t tdt;

/**
 * get random
 * t, target data.
 */
char *const getrdm(char *const t);

char *const doget(char uri[50], char *const buf) {
	sprintf(buf, _CMN_RESP_FMT_, uri, gettime());
	return buf;
}

char *const douri1(char body[1024], char *const buf) {
	printf("[%s][%s-%d]%s matched\n", gettime(),
		filename(__FILE__), __LINE__,_URI1_);
	strcpy(buf, "{\"data\":\"uri1 matched\"}");
	return buf;
}

char* const douri2(char body[1024], char *const buf) {
	printf("[%s][%s-%d]%s matched\n", gettime(),
		filename(__FILE__), __LINE__,_URI2_);
	strcpy(buf, "{\"data\":\"uri2 matched\"}");
	return buf;
}

char* const douri3(char body[1024], char *const buf) {
	printf("[%s][%s-%d]%s matched\n", gettime(), filename(__FILE__), __LINE__,
		_URI3_);
	strcpy(buf, "{\"data\":\"uri3 matched\"}");
	return buf;
}

char* const douri4(char body[1024], char *const buf) {
	printf("[%s][%s-%d]%s matched\n", gettime(), filename(__FILE__), __LINE__,
		_URI4_);
	strcpy(buf, "{\"data\":\"uri4 matched\"}");
	return buf;
}

char* const douri5(char body[1024], char *const buf) {
	printf("[%s][%s-%d]%s matched\n", gettime(), filename(__FILE__), __LINE__,
		_URI5_);
	strcpy(buf, "{\"data\":\"uri5 matched\"}");
	return buf;
}

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
        gettime(), filename(__FILE__), __LINE__,
		*(unsigned long *)pthread_getspecific(tdt),method,
		_SRV_IP_, _SRV_PORT_, uri, body
    );
    char buf[1500] = {0};				// response body
    if (strcasecmp(method, "GET")==0){
		doget(uri, buf);
    } else {
        if (strcmp(uri, _URI1_)==0) {
			douri1(body, buf);
        } else if (strcmp(uri, _URI2_)==0) {
			douri2(body, buf);
        } else if (strcmp(uri, _URI3_)==0) {
			douri3(body, buf);
        } else if (strcmp(uri, _URI4_)==0) {
			douri4(body, buf);
        } else if(strcmp(uri, _URI5_)==0) {
			douri5(body, buf);
        } else {
        	sprintf(buf, _CMN_RESP_FMT_,uri ,gettime());
        }
    }
    sprintf(resp, _HTTP_MSG_FMT_, strlen(buf), buf);
    return strlen(resp);
}

char *const getrdm(char *const t) {
	char r[4]={0};
	srand(time(NULL));
	for (int i = 0; i < sizeof(r); i++) {r[i]=rand()%255;}
	return getchararrayhex(r, sizeof(r), t);
}
