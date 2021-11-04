#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
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

#define MAX_EVENTS 10
#define port 8088
#define _BACKLOG_ 10
#define _BUF_SIZE_ 8096

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
        char buf_init[] = "rsp_frm_srv\n";
        for(int i = 0;i < strlen(buf_init); i++) {
            buf[i] = buf_init[i%strlen(buf_init)];
        }
        printf("start snd: %s\n", buf);
        int ss = send(cfd, buf, strlen(buf)+1, 0);
        //int ss = write(cfd, buf, strlen(buf)+1);
        printf("snd %d bytes, con closed\n", ss);
		close(cfd);
		break;
    }   
}

int main()
{
    struct epoll_event ev, events[MAX_EVENTS];
    int listen_sock, conn_sock, nfds, epollfd, n;

    /* set up listening socket, 'listen_sock',
	(socket(), bind(), listen()) */
	
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0) {
	    printf("socket error,%d, %s\n", errno, strerror(errno));
	}
	struct sockaddr_in server_sock;
    bzero(&server_sock, sizeof(server_sock));
    server_sock.sin_family = AF_INET;
    server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    server_sock.sin_port = htons(port);
    if (bind(listen_sock, (struct sockaddr*)&server_sock, 
        sizeof(sockaddr_in)) < 0) {
        printf("bind error, %d, %s\n",errno,strerror(errno));
        close(listen_sock);
        return 1;
    }   
    if (listen(listen_sock, _BACKLOG_) < 0) {
        printf("listen error, %d, %s\n", errno, strerror(errno));
        close(listen_sock);
        return 2;
    }
    printf("listening:%d\n", port);

	struct sockaddr_in local;
	socklen_t addrlen = sizeof(local);

    /* epoll */

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
 		perror("epoll_create1");
 		exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
 		perror("epoll_ctl: listen_sock");
 		exit(EXIT_FAILURE);
    }

    for (;;) {
 		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
 		if (nfds == -1) {
     		perror("epoll_wait");
     		exit(EXIT_FAILURE);
 		}

 		for (n = 0; n < nfds; ++n) {
    		if (events[n].data.fd == listen_sock) {
  				conn_sock = accept(listen_sock,
    			(struct sockaddr *) &local, &addrlen);
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
				//todo
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

