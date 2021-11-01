
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

#define IPSTR "127.0.0.1"
#define PORT 8082
#define BUFSIZE 1024

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

int get_data()
{
    int sockfd, ret, i, h;
    struct sockaddr_in servaddr;
    char str1[4096], str2[4096], buf[BUFSIZE], *str;
    socklen_t len;
    fd_set   t_set1;
    struct timeval  tv;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
            printf("create socket error!\n");
            exit(0);
    };

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
            printf("inet pton error!\n");
            exit(0);
    };

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
    strcat(str1, "GET /data HTTP/1.1\n");
    strcat(str1, "Host: www.test.cn\n");
    strcat(str1, "Content-Type: application/x-www-form-urlencoded\n");
    strcat(str1, "Content-Length: ");
    strcat(str1, str);
    strcat(str1, "\n\n");

    strcat(str1, str2);
    strcat(str1, "\r\n\r\n");
    printf("%s\n",str1);

    ret = write(sockfd,str1,strlen(str1));
    if (ret < 0) {
        printf("snd fail, err_code = %d，err_msg = '%s'\n",errno, strerror(errno));
        exit(0);
    } else {
        printf("snd success %d byte！\n\n", ret);
    }
    memset(buf, 0, 4096);
    i=read(sockfd, buf, 4095);
    if (i==0){
        close(sockfd);
        printf("读取数据报文时发现远端关闭，该线程终止！\n");
        return -1;
    }
    close(sockfd);
    printf("====result====");
    printf("%s\n", buf);
    return 0;

//
//    FD_ZERO(&t_set1);
//    FD_SET(sockfd, &t_set1);
//    while(1) {
//        sleep(2);
//        tv.tv_sec= 0;
//        tv.tv_usec= 0;
//        h = 0;
//        printf("--------------->1");
//        h = select(sockfd +1, &t_set1, NULL, NULL, &tv);
//        printf("--------------->2 h = %d", h);
//        if (h == 0) continue;
//        if (h < 0) {
//            close(sockfd);
//            printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
//            return -1;
//        };
//        if (h > 0) {
//            memset(buf, 0, 4096);
//            i= read(sockfd, buf, 4095);
//            if (i==0){
//                close(sockfd);
//                printf("读取数据报文时发现远端关闭，该线程终止！\n");
//                return -1;
//            }
//
//            printf("%s\n", buf);
//            return 0;
//        }
//    }
//    close(sockfd);
//    return 0;
}


int main()
{
    cout << "Hello world!" << endl;
    char ip[] = "127.0.0.1";
    int port = 8082;
//    connect(ip, port);
    get_data();
    return 0;
}
