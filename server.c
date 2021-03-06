/*********************************************************
  
   g++ -g -o _server server.cc  -lpthread
   ./_tcp_server > /dev/null 2>&1 &   

   sudo tcpdump -i lo -c20000 tcp port 8080 -w ./test.pcap
   
   * runtime need to ldconfig let so file can be load.
  > File Name:	tcp_server.c
  > Author:		richard
  > Mail:		liuyngchng@hotmail.com
  > Created Time: Tue 02 Mar 2020 05:15:29 PM CST
 *********************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "time.h"
#include <signal.h>
#include <typeinfo>
#include <pthread.h>
#define _BACKLOG_ 10
#define _BUF_SIZE_ 8096

const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);
char* get_time();
void catch_child(int signo);
void* rcvdata(void *);
bool check_debug_mode(int argc, char* argv[]);
int ret;
bool debug;
int main(int argc, char* argv[])
{
	if (argc < 2) {
		perror("pls input listening port\n");
		return 1;
	}
	debug = check_debug_mode(argc, argv);
	int port = atoi(argv[1]);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("%s, socket error, %d, %s\n", get_time(), errno, strerror(errno));
	}

	unsigned int opt_val = 0;
	unsigned int opt_len = sizeof(int);
	// snd buf
    getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&opt_val, &opt_len);
    printf("snd_buf len=%d\n", opt_val);
    opt_val = 10; 
    int set_result = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&opt_val, opt_len);
    opt_val = 0;
    getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&opt_val, &opt_len);
    printf("snd_buf len=%d\n", opt_val);
    // rcv buf
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&opt_val, &opt_len);
    printf("rcv_buf len=%d\n", opt_val);
    opt_val = 10; 
    set_result = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&opt_val, opt_len);
    opt_val = 0;
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&opt_val, &opt_len);
    printf("rcv_buf len=%d\n", opt_val);

	struct sockaddr_in server_sock;
	bzero(&server_sock, sizeof(server_sock));
	server_sock.sin_family = AF_INET;
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sock.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr*)&server_sock, 
		sizeof(sockaddr_in)) < 0) {
		printf("bind error, %d, %s\n",errno, strerror(errno));
		close(sockfd);
		return 1;
	}
	if (listen(sockfd, _BACKLOG_) < 0) {
		printf("listen error, %d, %s\n", errno, strerror(errno));
		close(sockfd);
		return 2;
	}
	printf("listening:%d\n", port);
	sockaddr_in csock;
	socklen_t len = sizeof(csock);
	int cfd;
	while (1) {
	 a: cfd = accept(sockfd, (sockaddr*)&csock, &len);
		if (cfd < 0) {
			if((errno == ECONNABORTED) || errno == EINTR) {
				goto a;
			} else {
				printf("accept  error, %d, %s\n", errno, strerror(errno));
				return -1;
			}
		}
		//char buf_ip[INET_ADDRSTRLEN];
		//memset(buf_ip, 0, sizeof(buf_ip));
		//inet_ntop(AF_INET, &csock.sin_addr, buf_ip, sizeof(buf_ip));
		//cout << "con from " << buf_ip << ":" << ntohs(csock.sin_port) << endl;
		//cout << len << ":" << typeid(csock).name() << ":" << csock.sin_addr.s_addr << endl;
		printf("con from %s: %d\n", inet_ntoa(csock.sin_addr), ntohs(csock.sin_port));
		//rcvdata(&cfd);
		pthread_t t;
		pthread_create(&t, NULL, &rcvdata, &cfd);
		pthread_detach(t);
	}
	return 0;
}

void* rcvdata(void* sockfd)
{
	int cfd = *(int*)sockfd;
	char buf[_BUF_SIZE_];
	while (1) {
		memset(buf, 0, sizeof(buf));
		//read(cfd, buf, sizeof(buf));
		int rs = recv(cfd, buf, sizeof(buf), 0);
		printf("rcvd %d bytes,\n%s\n", rs, buf);
		if (rs < sizeof(buf)) {
			printf("rcv finished\n");
			//break;
		}
		if (debug) {
			printf("rcv %d\n", strlen(buf));
		}
		memset(buf, 0, sizeof(buf));
		char buf_init[] = "rsp_frm_srv";
    	for(int i = 0;i < strlen(buf_init); i++) {
        	buf[i] = buf_init[i%strlen(buf_init)];
    	}
		//buf[strlen(buf)] = '\0';
		printf("start snd: %s\n", buf);
		//int ss = send(cfd, buf, strlen(buf)+1, 0);
		int ss = write(cfd, buf, strlen(buf)+1);
		printf("snd %d bytes\n", ss);
		//fflush(stdout);
		//return NULL;
    }
}

void catch_child(int signo) 
{
	printf("child task finished\n");	  
}

bool check_debug_mode(int argc, char* argv[])
{
    bool debug =false;
    if (argc == 3) {
        for(int i=0; i< argc; i++) {
            if(strncasecmp(argv[i], "-d", 2)==0) {
                debug = true;
                printf("debug mode\n");
            }
        }
    }
    return debug;
}

char* get_time()
{
    struct tm *ptr; 
    time_t lt;
    lt =time(NULL);
    ptr = localtime(&lt);
    char* t = asctime(ptr);
    t[strlen(t)-1]=0;
    return t;
}
