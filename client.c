/**
   g++ -o main main.cc
   runtime need to ldconfig let so file can be load.
   ./_tcp_client 10.0.0.1 9999 > /dev/null	2>&1 &
   ifstat > tp.dat &
   vim tp.dat
   :g/^\s*$/d	   // delete blank line
   sudo iftop -i lo
  **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#define _BUF_SIZE_ 8096

#define IPSTR "127.0.0.1"
#define PORT 80
#define BUFSIZE 10240

long int get_time();

int con(char *ip, int port)
{
	printf("con to %s:%d\n", ip, port);
	char buf_init[]="hello,this is a test";
	char buf[_BUF_SIZE_];
	for(int i=0;i<sizeof(buf);i++) {
		buf[i]=buf_init[i%strlen(buf_init)];
	}
   // memset(buf,0,sizeof(buf));
	struct sockaddr_in srv_sock;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("error,%d, %s\n", errno, strerror(errno));
	}
	bzero(&srv_sock, sizeof(srv_sock));
	srv_sock.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &srv_sock.sin_addr);
	srv_sock.sin_port = htons(port);
	srv_sock.sin_addr.s_addr = inet_addr(ip);
	int ret = connect(sockfd, (struct sockaddr*)& srv_sock, sizeof(srv_sock));
	if (ret < 0) {
		printf("connect to %s:%d\n", ip, port);
		printf("error, errno is %d, %s\n",errno, strerror(errno));
		return 1;
	}
	printf("connected to %s:%d\n", ip, port);
	//buf[strlen(buf)-1]='\0';
	//write(sockfd, buf, strlen(buf));
	int ss = send(sockfd, buf, strlen(buf), 0);
	printf("close connection.\n");
	close(sockfd);
	printf("exit\n");
	return 0;
}

int get_data()
{
	int sockfd, h;
	struct sockaddr_in servaddr;
	char str1[4096], str2[4096], buf[BUFSIZE], *str;
	socklen_t len;
	fd_set	 t_set1;
	struct timeval	tv;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("create socket error!\n");
		exit(0);
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

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
		printf("inet_pton error!\n");
		exit(0);
	}

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		printf("connect error!\n");
		exit(0);
	}
	printf("connect success\n");

	//send data
	memset(str2, 0, 4096);
	strcat(str2, "a=1&b=2");
	str=(char *)malloc(128);
	len = strlen(str2);
	sprintf(str, "%d", len);

	memset(str1, 0, 4096);
	strcat(str1, "GET / HTTP/1.1\r\n");
	strcat(str1, "Accept: */*\r\n");
	strcat(str1, "Accept-Language: zh-cn\r\n");
	strcat(str1, "User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n");
	strcat(str1, "Host: 127.0.0.1:80\r\n");
	strcat(str1, "Content-Type: application/json;charset=UTF-8\r\n");
	//strcat(str1, "Content-Length: ");
	//strcat(str1, str);
	//strcat(str1, "\r\n");

	//strcat(str1, str2);
	strcat(str1, "Connection: keep-alive\r\n\r\n");
	printf("%s\n",str1);
	for (int c = 0; c < 100; c++)
	{
		printf("====start request====, turn = %d\n", c);
		long int t = get_time();
		printf("send length = %d\n", strlen(str1));
		printf("%s", str1);
		int ss = send(sockfd,str1,strlen(str1), 0);
		if (ss < 0) {
            printf("snd fail, err_code = %d，err_msg = '%s'\n",errno, strerror(errno));
			exit(0);
		} else {
			printf("snd %d byte\n", ss);
		}
		memset(buf, 0, sizeof(buf));
		printf("rcving\n");
		int rs = recv(sockfd, buf, sizeof(buf), 0);
		//int rs = read(sockfd, buf, sizeof(buf));
		if (rs==0) {
			close(sockfd);
			printf("read faild！\n");
			return -1;
		}
		printf("rcvd=%s\n", buf);
		printf("====end request====, turn = %d\n", c);
		printf("%ldus elapses in turn %d\n", get_time() - t, c);
//		usleep(1000);
		//break;
	}
	close(sockfd);
	return 0;
}


long int get_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long int timestamp = tv.tv_sec * 1000000 + tv.tv_usec;
	return timestamp;
}

int main()
{
	printf("start\n");
	char ip[] = "127.0.0.1";
	int port = 8082;
//     connect(ip, port);
	long int t = get_time();
	printf("%ld\n", get_time());
	get_data();
	printf("%ldus elapsed in all request\n", get_time() - t);
	return 0;
}
