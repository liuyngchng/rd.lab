/**
 * TCP socket util
 * @author whoami@hotmail.com
 * @since 2022-06-01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "pttn.h"
#include "const.h"
#include "cfg.h"
#include "utl.h"

extern int _SRV_PORT_;
extern char _SRV_IP_[_CFG_LEN_];


pthread_key_t tdt;

void destructor(void* data) {
    printf("[%s][%s-%d]free thread-specific data\n", gettime(),
		filename(__FILE__),__LINE__);
    free(data);
}

/**
 * 从文件句柄中接收数据
 **/
void* rcvdata(void *);

/**
 * start a TCP server
 */
int startsrv() {
    struct sockaddr_in srvaddr;
    int sfd;
    int sockopt = 1;
    int res;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        printf("[%s][%s-%d]create socket error\n", gettime(),
        	filename(__FILE__),__LINE__);
        return -1;
    }
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(_SRV_PORT_);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(int));
    res = bind(sfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr));
    if (res < 0) {
        printf(
            "[%s][%s-%d]bind error, port %d, cause %d,%s\n",gettime(), 
            filename(__FILE__),__LINE__, _SRV_PORT_, errno, strerror(errno)
        );
        close(sfd);
        return -1;
    }
    listen(sfd, _BACKLOG_);
    printf(
		"[%s][%s-%d][t-%ld]listening %s:%d, srv_started\n",gettime(),
		filename(__FILE__),__LINE__,pthread_self(), _SRV_IP_, _SRV_PORT_
	);
    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int cfd;
again:  cfd = accept(sfd, (struct sockaddr *)&cliaddr, &len);
        if(cfd < 0) {
            printf(
                "[%s][%s-%d]accept error, cause %d,%s\n", 
                gettime(), filename(__FILE__),__LINE__, errno, strerror(errno)
            );
            if((errno == ECONNABORTED) || errno == EINTR) {
				goto again;
			} else {
				close(sfd);
                printf("[%s][%s-%d]server closed\n", gettime(),
                	filename(__FILE__),__LINE__);
                return -1;
			}
        }
        char *ip = inet_ntoa(cliaddr.sin_addr);
        int port = ntohs(cliaddr.sin_port);
        printf("[%s][%s-%d][t-%ld]con_from_%s:%d\n", gettime(),
        	filename(__FILE__), __LINE__, pthread_self(), ip, port);
        pthread_t t;
		pthread_create(&t, NULL, &rcvdata, &cfd);
		pthread_detach(t);
    }
    close(sfd);
    printf("[%s][%s-%d]server closed\n", gettime(),
    	filename(__FILE__),__LINE__);
    return 0;
}

/**
 * rcv data sent from peer represented by sockfd
 **/
void *rcvdata(void* sockfd) {
    char buf[1024] = {0};
    int cfd = *(int*)sockfd;
    int size = read(cfd, buf, sizeof(buf));
    unsigned long *tidp = NULL;
    tidp = (unsigned long *)malloc(sizeof(unsigned long));
    *tidp = gettimestamp();
	int tmp = pthread_setspecific(tdt,tidp);
//	unsigned long * pthread_test=pthread_getspecific(tdt);
    printf("[%s][%s-%d][t-%ld]rcv %d bytes, tdt %lu, pthread_getspecific %lu\n%s\n",gettime(),
    	filename(__FILE__),__LINE__, pthread_self(), size,
		*tidp, *(unsigned long *)pthread_getspecific(tdt), buf);
    char resp[4096] = {0};
    dispatch(buf,resp);
    write(cfd, resp, strlen(resp));
    printf("[%s][%s-%d][t-%ld]return msg\n%s\n", gettime(),
    	filename(__FILE__), __LINE__, pthread_self(),resp);
    close(cfd);
    printf("[%s][%s-%d][t-%ld]connection closed\n", gettime(),
    	filename(__FILE__), __LINE__, pthread_self());
    return NULL;
}

int main(int argc, char* argv[]) {
	initcfg();
	pthread_key_create(&tdt, destructor);
    printf("[%s][%s-%d][t-%ld]server starting\n", gettime(), filename(__FILE__), __LINE__, pthread_self());
    startsrv();
    return 0;
}
