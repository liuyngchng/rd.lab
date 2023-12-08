#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "util.h"

char _SRV_IP_[_CFG_LEN_]={0};
int _SRV_PORT_=8083;
char _CODEC_IP_[_CFG_LEN_]={0};
int _CODEC_PORT_=18080;
char _DEC_PATH_[_CFG_LEN_]={0};
char _COD_PATH_[_CFG_LEN_]={0};

int getcfg(const char *fname) {
    FILE *fp;
    fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("[%s][%s-%d]err_fail to open cfg file %s\n",
        		gettime(),filename(__FILE__), __LINE__, fname);
        return -1;
    }
    char buf[_CFG_LEN_]={0};
    while (fgets(buf, _CFG_LEN_, fp) != NULL) {
        int len = strlen(buf);
        int i=0;
        while (buf[i] != '=') {
            i++;
        }
        char key[_CFG_LEN_]={0};
        char value[_CFG_LEN_]={0};
        int k=0, kl=0, j=0;
        for (j = 0; j < i; j++) {
            key[k++] = buf[j];
        }
        kl=k;
        int v=0, vl=0;
        for (j = i+1; j < len; j++) {
        	if(buf[j]=='\n') break;
            value[v++] = buf[j];
        }
        vl=v;
        if (strcmp(key, "SRV_IP")==0) {
        	strcpy(_SRV_IP_, value);
        } else if(strcmp(key, "SRV_PORT")==0){
        	_SRV_PORT_=atoi(value);
        } else if(strcmp(key, "CODEC_IP")==0){
        	strcpy(_CODEC_IP_, value);
        } else if(strcmp(key, "CODEC_PORT")==0){
        	_CODEC_PORT_=atoi(value);
        } else if(strcmp(key, "DEC_PATH")==0){
        	strcpy(_DEC_PATH_, value);
        } else if(strcmp(key, "COD_PATH")==0){
        	strcpy(_COD_PATH_, value);
        }
    }
    fclose (fp);
    printf("_SRV_IP_=%s, _SRV_PORT_=%d\n", _SRV_IP_, _SRV_PORT_);
    return 0;
}

void initcfg() {
	getcfg("./cfg.ini");
}

