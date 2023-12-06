#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "util.h"
#include "pttn.h"
#include "const.h"
#include "cfg.h"

extern int _SRV_PORT_;
extern char _SRV_IP_[_CFG_LEN_];

/**
 * 从文件句柄中接收数据
 **/
void* rcvdata(void *);

int startsrv() {
    struct sockaddr_in srvaddr;
    int sfd;
    int sockopt = 1;
    int res;
    /*创建一个套接字*/
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
		"[%s][%s-%d][t-%ld]listening %s:%d\n",gettime(),
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
        printf("[%s][%s-%d][t-%ld]%s connected\n", gettime(),
        	filename(__FILE__), __LINE__, pthread_self(), ip);
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
 * rcv data sent from peer
 **/
void *rcvdata(void* sockfd) {
    char buf[1024] = {0};
    int cfd = *(int*)sockfd;
    int size = read(cfd, buf, sizeof(buf));
    printf("[%s][%s-%d][t-%ld]rcv %d bytes\n%s\n",gettime(),
    	filename(__FILE__),__LINE__, pthread_self(), size, buf);
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
    printf("[%s][%s-%d][t-%ld]server starting\n", gettime(), filename(__FILE__), __LINE__, pthread_self());
    startsrv();
    return 0;
}
