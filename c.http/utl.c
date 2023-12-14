/**
 * common util method
 * @author whoami@hotmail.com
 * @since 2022-06-01
 */
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "utl.h"

char *const getmethod(const char *s, char *const t, const int n) {
    int i = 0;
    for (; i < (int)strlen(s); i++){
        if(i>= n) {
            break;
        }
        if (s[i] == '\n' || s[i] == '\r' || s[i] == ' ') {
                break;
        } else {
            t[i]=s[i];
        }
    }
    t[i]='\0';
    return t;
}

char *const geturi(const char *s, char *const t, const int n) {
    int i=0;    // index for s
    int j=0;    // count for space
    int k=0;    // index for t
    for (; i < (int)strlen(s); i++){
        if (j==1) {
            if (s[i]==' ') {
                break;
            } else {
                if (k < n) {
                    t[k++]=s[i];
                } else {
                    break;
                }
            }
        } else if (s[i]==' ') {
            j++;
        }
    }
    t[k]='\0';
    return t;
}

char *const getln(const char *s, char *const t, const int n, const int l) {
    int i=0;    // s index
    int j=0;    // l index
    int k=0;    // t index
    for (int i=0;i<strlen(s);i++) {
        if (j == l) {
            if (s[i] == '\n' || s[i] == '\r') {
                break;
            } else {
                if (k < n) {
                    t[k++]=s[i];
                } else {
                    break;
                }
            }
        } else if (s[i] == '\n' || s[i] == '\r') {
            j++;
        }
    }
    t[k]='\0';
    return t;
}

char *const getbody(const char *s, char *const t, int n) {
    int i = 0;
    int j = 0;
    int k = 0;      // start flag
    for (; i < strlen(s); i++){
        if(i<3){
            continue;
        }
        if(j>=n){
            break;
        }
        if (k){
            t[j++] = s[i];
            continue;
        }
        if (s[i-3] == '\r' && s[i-2] == '\n' && s[i-1] == '\r' && s[i] == '\n') {
            k=1;
        }
        continue;
    }
    t[j]='\0';
    return t;
}

char *const gettimestr(char *const buf, int n){
    time_t lt;
    lt =time(NULL);
    printf("current second,%ld\n", lt);
    struct tm *ptr; 
    ptr = localtime(&lt);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ptr);
    return buf;
}

char *gettime1(){
    time_t lt;
    lt =time(NULL);
    printf("current second,%ld\n", lt);
    struct tm *ptr; 
    ptr = localtime(&lt);
    static char buf[32]={0};
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ptr);
    return buf;
}

unsigned long gettimestamp() {
    struct tm *tm_t;
    struct timeval timeeval;
    time_t lt;
    lt =time(NULL);
//    printf("second,\t%ld\n", lt);
    gettimeofday(&timeeval,NULL);
    unsigned long timestamp=lt*1000+timeeval.tv_usec/1000;
//    printf("timestamp, \t%ld\n",timestamp);
    return timestamp;
}

char *gettime() { 
    struct tm *tm_t;
    struct timeval time;
    gettimeofday(&time,NULL);
    tm_t = localtime(&time.tv_sec);
    static char str_time[32]={0};
    sprintf(str_time,
        "%04d-%02d-%02d %02d:%02d:%02d %03ld",
        tm_t->tm_year+1900,
        tm_t->tm_mon+1, 
        tm_t->tm_mday,
        tm_t->tm_hour, 
        tm_t->tm_min, 
        tm_t->tm_sec,
        time.tv_usec/1000
    );
    return str_time;
}

char *gettime2() { 
    struct tm *tm_t;
    struct timeval time;
    gettimeofday(&time,NULL);
    tm_t = localtime(&time.tv_sec);
    static char str_time[32]={0};
    sprintf(str_time,
        "%02d%02d%02d%02d%02d%02d",
        (tm_t->tm_year+1900)%100,
        tm_t->tm_mon+1, 
        tm_t->tm_mday,
        tm_t->tm_hour, 
        tm_t->tm_min, 
        tm_t->tm_sec
    );
    return str_time;
}

char *const getjsonv(const char *s, char *const t, const char *jsonk, int n) {
    int i=0;    // source index
    int j=0;    // target start index
    int k=0;    // key index  
    int f=0;    // target end flag
    for (; i < strlen(s); i++){
        if(k==0 && strncmp(&s[i], jsonk, strlen(jsonk)) !=0) {
            continue;
        }
        if (k==0){
            if (i<strlen(s)-strlen(jsonk)){
                i+=strlen(jsonk);
                k++;
            } else{
                printf("err_json_str, %s\n", &s[i]);
                break;
            }
            
        } else {
            if (k<3 && (s[i]==':' || s[i] =='"')) {
                k++;
                continue;
            }
            if (j<n && s[i] != '"') {
            	if(s[i] == '[') {
            		continue;
            	}
                t[j++] = s[i]; 
                continue;
            } else{
                break;
            }
        }
    }
    t[i]='\0';
}

char *const getcharhex(const char s, char *const t) {
	char *m="ABCDEF";
	short i,j, k;
	k=s&0XF0;
	i=k>>4;
	j=s&0X0F;
	if (i>9) {
		t[0]=0x37+i;
	} else {
		t[0] =0x30+i;
	}
	if (j>9) {
		t[1]=0x37+j;
	} else {
		t[1]=0x30+j;
	}
	return t;
}

char *const getshorthex(const unsigned short s, char *const t){
	int i=s&0xFF00;
	int j=s&0xFF;
	char a=i>>8;
	char b=j;
	getcharhex(a, t);
	getcharhex(b, t+2);
	return t;
}

char *const getchararrayhex(const char *s, int n, char *const t){
	for (int i=0; i< n; i++) {
		getcharhex(s[i], (char *)(t+2*i));
	}

	return t;
}

char *const readf(const char *fname, char *const t, int n) {
    FILE *fp;
    fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("fail to open file %s\n", fname);
        return t;
    }
    char buf[1024]={0};
    int m=0;
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        m += strlen(buf);
        if (m>n) break;
        strcat(t, buf);
    }
    fclose (fp);
    return t;
}

