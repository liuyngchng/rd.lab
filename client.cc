/**
   g++ -o _tcp_client tcp_client.cc /home/rd/so/libmytime.so
   runtime need to ldconfig let so file can be load.
   ./_tcp_client 10.0.0.1 9999 > /dev/null	2>&1 &
   ifstat > tp.dat &
   vim tp.dat
   :g/^\s*$/d	   // delete blank line
   sudo iftop -i lo
  */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <iostream>
#define _BUF_SIZE_ 8096

using namespace std;

int connect(char *ip, int port)
{
	cout << "con to " << ip << ":" << port << endl;
	char buf_init[]="hello,this is a test";
	char buf[_BUF_SIZE_];
	for(int i=0;i<sizeof(buf);i++) {
		buf[i]=buf_init[i%strlen(buf_init)];
	}
   // memset(buf,0,sizeof(buf));
	struct sockaddr_in srv_sock;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "error," << errno << ", " << strerror(errno) << endl;
	}
	bzero(&srv_sock, sizeof(srv_sock));
	srv_sock.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &srv_sock.sin_addr);
	srv_sock.sin_port = htons(port);
	srv_sock.sin_addr.s_addr = inet_addr(ip);
	int ret = connect(sockfd, (struct sockaddr*)& srv_sock, sizeof(srv_sock));
	if (ret < 0) {
		cout << "connect to " << ip << ":" << port
			 << " error, errno is " << errno << ", errstring is "
			 << strerror(errno) << endl;
		return 1;
	}
	cout << "connected to " << ip << ":"<< port << endl;
	//buf[strlen(buf)-1]='\0';
	int count = 0;
	while (count < 10000000) {
		//write(sockfd, buf, strlen(buf));
		int ss = send(sockfd, buf, strlen(buf), 0);
		if (strncasecmp(buf,"quit", 4) == 0) {
			cout <<"quit !"<< endl;
			break;
		}
	}
	cout <<"close connection." << endl;
	close(sockfd);
	cout <<"exit" << endl;
	return 0;
}


int main()
{
    char ip[] = "127.0.0.1";
    int port = 8088;
    connect(ip, port);
}
