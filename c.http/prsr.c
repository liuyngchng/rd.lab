/**
 * data parser util
 * @author whoami@hotmail.com
 * @since 2022-06-01
 */
#include <stdio.h>
#include <string.h>
#include "prsr.h"

#include "utl.h"

char *const prsdt(const char *s, char *const t, const int n) {
    return prs_up_rpt_dt(s, t, n);
}

char *const prs_up_rpt_dt(const char *s, char *const t,
	const int n) {
    int i=0;
    char ctrl[3]={0};
    char seq[3]={0};
    strcat(t, "{\"cr\":\"");
	strncat(t, s + i, 2);
	i += 2;
    strcat(t, "\",\"ctrl\":\"");
    strncat(t, s+i, 2);

    strncat(ctrl, s+i, 2);
    i+=2;
    strcat(t, "\",\"l\":\"");
    strncat(t, s+i, 2);
    i+=2;
    strcat(t, "\",\"c\":\"");
    strncat(t, s+i, 4);
    i+=4;
    strcat(t, "\",\"r\":\"");
    strncat(t, s+i, 4);
    i+=4;
    strcat(t, "\",\"seq\":\"");
    strncat(t, s+i, 2);
    strncat(seq, s+i, 2);
    i+=2;
  //  printf("[seq]%s\n", seq);
    if(s[i-1]=='0') {
        strcat(t, "\",\"appId\":\"");
        strncat(t, s+i, 46);
        i+=46;
    }
    strcat(t, "\",\"dt\":\"");
    strcat(t, s+i);
    strcat(t, "\"}");
    //return dt;
    if (ctrl[1] == '2' && seq[1] =='1') {
        memset(t, 0, n);		//reset data for return
        strcat(t, "{\"ext\":{\"dt\":\"");
        strcat(t, s+i);
        strcat(t, "\"}}");
        return t;

    }
    printf("[%s][%s-%d][cmd]%s\n",gettime(), filename(__FILE__),
    	__LINE__,t);
    memset(t, 0, n);			// reset data for return
    return prs_dt_u(s+i, t, n);
}

char *const prs_dt_u(const char *s, char *const t, const int n) {
    int i=0;
    char id[5]={0};
    strcat(t, "{\"ctrl\":\"");
    strncat(t, s+i, 2);
    i+=2;
    strcat(t, "\",\"n\":\"");
    strncat(t, s+i, 2);
    i+=2;
    strcat(t, "\",\"id\":\"");
    strncat(t, s+i, 4);
    strncat(id, s+i, 4);
    i+=4;
    strcat(t, "\",\"dt\":\"");
    strcat(t, s+i);
    strcat(t, "\"}");
    printf("[%s][%s-%d][dt_u]%s, %s\n",gettime(), filename(__FILE__),
    	__LINE__, id,t);
    if(strncmp(id, "800", 3)!=0) {
    	return t;
    }
    //return t;
    memset(t, 0, n);			// reset data for return
    return prs_up_rpt_dt_obj(id, s+i, t, n);
}
    
char *const prs_up_rpt_dt_obj(const char *id, const char *s,
	char *const t, const int n) {
    int i=0;
    strcat(t, "{\"time\":\"");
    strncat(t, s+i, 12);
    i+=12;
    strcat(t, "\",\"sts\":\"");
    strncat(t, s+i, 8);
    i+=8;
    strcat(t, "\",\"iVStd\":\"");
    strncat(t, s+i, 16);
    i+=16;
    strcat(t, "\",\"iV\":\"");
    strncat(t, s+i, 16);
    i+=16;
    strcat(t, "\",\"fVStd\":\"");
    strncat(t, s+i, 16);
    i+=16;
    strcat(t, "\",\"fV\":\"");
    strncat(t, s+i, 16);
    i+=16;
    strcat(t, "\",\"flwRtStd\":\"");
    strncat(t, s+i, 8);
    i+=8;
    strcat(t, "\",\"flwRt\":\"");
    strncat(t, s+i, 8);
    i+=8;
    strcat(t, "\",\"t\":\"");
    strncat(t, s+i, 8);
    i+=8;
    strcat(t, "\",\"p\":\"");
    strncat(t, s+i, 8);
    i+=8;
    strcat(t, "\",\"engSum\":\"");
    strncat(t, s+i, 16);
    i+=16;
    strcat(t, "\",\"engDst\":\"");
    strncat(t, s+i, 8);
    i+=8;
    strcat(t, "\",\"acsV\":\"");
    strncat(t, s+i, 4);
    i+=4;
    strcat(t, "\",\"vlt\":\"");
    strncat(t, s+i, 4);
    i+=4;
    strcat(t, "\",\"pwr\":\"");
    strncat(t, s+i, 2);
    i+=2;
    strcat(t, "\",\"nb\":\"");
    strncat(t, s+i, 8);
    i+=8;
    strcat(t, "\",\"ecl\":\"");
    strncat(t, s+i, 4);
    i+=4;
    strcat(t, "\",\"cid\":\"");
    strncat(t, s+i, 12);
    i+=12;
    strcat(t, "\",\"rnf\":\"");
    strncat(t, s+i, 4);
    i+=4;
    //printf("[obj_id]%s\n", id);
    strcat(t, "\",\"ext\":");
    char extdt[512]={0};
    if (id[3] == '3') {
        strcat(t, prs_evt_ext(s+i, extdt, sizeof(extdt)));
    } else if (id[3] == '2') {
        strcat(t, prs_at_ext(s+i, extdt, sizeof(extdt)));
    }
    strcat(t, "}");
    return t;
} 
char *const prs_evt_ext(const char *s, char *const t, const int n) {
    int i=0;
    strcat(t, "{\"evt\":\"");
    strncat(t, s+i, 2);
    i+=2;
    strcat(t, "\",\"fvStdS1\":\"");
    strncat(t, s+i, 16);
    i+=16;
    strcat(t, "\",\"dt\":[");
    for(int j=0; j<3; j++) {
        char tmp[128]={0};
        if(j>0){
            strcat(tmp, ",");
        }
        strcat(tmp, "{\"date\":\"");
        strncat(tmp, s+i, 6);
        i+=6;
        strcat(tmp, "\",\"fVStd\":\"");
        strncat(tmp, s+i, 16);
        i+=16;
        strcat(tmp, "\",\"fV\":\"");
        strncat(tmp, s+i, 16);
        i+=16;
        strcat(tmp, "\"}");
        strcat(t, tmp);
    }
    strcat(t, "]}");
    return t;
}

char *const prs_at_ext(const char *s, char *const t, const int n) {
    int i=0;
    strcat(t, "{\"date\":\"");
    strncat(t, s+i, 6);
    i+=6;
    strcat(t, "\",\"dt\":\"");
    strcat(t, s+i);
    strcat(t, "\"}");
    return t;
 }
int dttest(int a){
    printf("hi, %d\n", a);
    return 0;
}
int dttest1(int argc, char* argv[]) {
//int main(int argc, char* argv[]) {
    if(NULL == argv[1]) {
        printf("pls input dt\n");
        return -1;
    }
    printf("prs_dt, %s\n", argv[1]);
    char buf[1024]={0};
    printf("[dt]%s\n", prs_up_rpt_dt(argv[1], buf, sizeof(buf)));
    return 0;
}
