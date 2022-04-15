#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/epoll.h>

#define _MAX_EVENTS_ 	10
#define _PORT_ 			8088
#define _BACKLOG_ 		10
#define _BUF_SIZE_ 		8096

/**
 * get the microseconds of current time
 */
long int get_time()
{
   	struct timeval tv;
   	gettimeofday(&tv, NULL);
   	long int timestamp = tv.tv_sec * 1000000 + tv.tv_usec;
	return timestamp;
}

/**
 * set sockfd no blocking
 */
void setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock, F_GETFL);
	if (opts < 0)
	{
		perror("fcntl(sock, F_GETFL)");
		exit(1);
	}
	opts |= O_NONBLOCK;
	if (fcntl(sock, F_SETFL, opts) < 0)
	{
		perror("fcntl(sock, F_SETFL)");
		exit(1);
	}
}

/**
 * receive data from sockfd
 */
void* rcvdata(void* sockfd)
{
    int cfd = *(int *)sockfd;
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
        printf("rcv %lu bytes\n",strlen(buf));
        memset(buf, 0, sizeof(buf));
        char buf_init[] = "rsp_frm_srv\n";		// \n make stream be flushed.
        for(int i = 0;i < strlen(buf_init); i++) {
            buf[i] = buf_init[i%strlen(buf_init)];
        }
        printf("start snd: %s\n", buf);
        int ss = send(cfd, buf, strlen(buf)+1, 0);
        //int ss = write(cfd, buf, strlen(buf)+1);
        printf("snd %d bytes, con closed\n", ss);
		//fflush(NULL);   // nothing to do 
		//close(cfd);
		break;
    }   
}

/** 
 * set up listening socket, 'listen_sock',
 *	(socket(), bind(), listen())
 */
int listen_sock(int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
	    printf("socket error,%d, %s\n", errno, strerror(errno));
	}
	struct sockaddr_in server_sock;
    bzero(&server_sock, sizeof(server_sock));
    server_sock.sin_family = AF_INET;
    server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    server_sock.sin_port = htons(port);
	int on = 1;
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (bind(sockfd, (struct sockaddr*)&server_sock, 
        sizeof(struct sockaddr_in)) < 0) {
        printf("bind error, %d, %s\n",errno,strerror(errno));
        close(sockfd);
        return 1;
    }   
    if (listen(sockfd, _BACKLOG_) < 0) {
        printf("listen error, %d, %s\n", errno, strerror(errno));
        close(sockfd);
        return 2;
    }
    printf("listening:%d\n", port);
	return sockfd;
}

/**
 * epoll sockfd
 */
int epoll_sock(int sockfd)
{
    struct epoll_event ev, events[_MAX_EVENTS_];
    int conn_sock, nfds, epollfd;
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
 		perror("epoll_create1");
 		exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
 		perror("epoll_ctl: sockfd");
 		exit(EXIT_FAILURE);
    }

	struct sockaddr_in local;
	socklen_t addrlen = sizeof(local);
    for (;;) {
 		nfds = epoll_wait(epollfd, events, _MAX_EVENTS_, -1);
 		if (nfds == -1) {
     		perror("epoll_wait");
     		exit(EXIT_FAILURE);
 		}

 		for (int n = 0; n < nfds; ++n) {
    		if (events[n].data.fd == sockfd) {
  				conn_sock = accept(sockfd, (struct sockaddr *) &local, &addrlen);
  				if (conn_sock == -1) {
      				perror("accept");
      				exit(EXIT_FAILURE);
  				}
  				setnonblocking(conn_sock);
  				ev.events = EPOLLIN | EPOLLET;
  				ev.data.fd = conn_sock;
  				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
					&ev) == -1) {
      				perror("epoll_ctl: conn_sock");
      				exit(EXIT_FAILURE);
  				}
     		} else {
  				//do_use_fd(events[n].data.fd);
				int fd = events[n].data.fd;
				//printf("something happened %d\n", fd);
				printf("con from %s:%d\n",inet_ntoa(local.sin_addr),ntohs(local.sin_port));
        		//rcvdata(&fd);
        		pthread_t t;
        		pthread_create(&t, NULL, &rcvdata, &fd);
        		pthread_detach(t);
     		}
 		}
    }
}

int main()
{
	int sockfd = listen_sock(_PORT_);
	epoll_sock(sockfd);	
}
