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
#include <math.h>
#include <ctype.h>

#define _BUF_SIZE_ 8096

long int get_time();
int get_data(int sockfd, char *data, char *path);
void get_content_len(char *s, char **ms, int size);
int num_s(char *hex_str);

int con(char *ip, int port)
{
	printf("start conn %s:%d\n", ip, port);
	struct sockaddr_in srv_sock;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("error,%d, %s\n", errno, strerror(errno));
	}
	bzero(&srv_sock, sizeof(srv_sock));
	srv_sock.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &srv_sock.sin_addr);
	srv_sock.sin_port = htons(port);

	int ret = connect(sockfd, (struct sockaddr*)& srv_sock, sizeof(srv_sock));
	if (ret < 0) {
		printf("connect to %s:%d\n", ip, port);
		printf("error, errno is %d, %s\n",errno, strerror(errno));
		return 1;
	}
	printf("connected to %s:%d\n", ip, port);
	return sockfd;
}
int get_data(int sockfd, char *data, char *path)
{
	char hdr[4096], param[4096], buf[_BUF_SIZE_], *bdh, *end, *bdy;
	memset(hdr, 0, sizeof(hdr));
	memset(param, 0, 4096);
	memset(buf, 0, sizeof(buf));
	strcat(param, "?a=1&b=2");
	memset(hdr, 0, 4096);
	strcat(hdr, "GET ");
	strcat(hdr, path);
	strcat(hdr, param);
	strcat(hdr, " HTTP/1.1\r\n");
	strcat(hdr, "Accept: */*\r\n");
	strcat(hdr, "Accept-Language: zh-cn\r\n");
	strcat(hdr, "User-Agent: rdAgent\r\n");
	strcat(hdr, "Host: 127.0.0.1:80\r\n");
	strcat(hdr, "Content-Type: application/json;charset=UTF-8\r\n");
	strcat(hdr, "Connection: keep-alive\r\n");
	strcat(hdr, "\r\n");
    int ss = send(sockfd, hdr, strlen(hdr), 0);
    if (ss < 0) {
        printf("snd fail, err_code = %d，err_msg = '%s'\n",errno, strerror(errno));
        exit(0);
    } else {
        printf("snd %d bytes\n", ss);
    }
    int rs_sum = 0;
    int rs = recv(sockfd, buf, sizeof(buf)-1, 0);
    if (rs == 0) {
        close(sockfd);
        printf("read failed！\n");
        exit(-1);
    }
    char *con_len = strstr(buf, "Content-Length");
    if (con_len != NULL) {
        memcpy(data + rs_sum, buf, rs);
        rs_sum += rs;
        bdh = strstr(buf, "\r\n\r\n");
        bdy = bdh + 4;
        int bdy_l = strlen(bdy);
        int size = 1;
        char **info = (char **)malloc((size + 1) * sizeof(char *));
        get_content_len(con_len, info, size);
        int flag_size = strlen("Content-Length: ");
        char *tmp_flg = info[0] + flag_size;
        const int con_len_i = atoi(tmp_flg);
        while(bdy_l < con_len_i) {
            memset(buf, 0, sizeof(buf));
            rs = recv(sockfd, buf, sizeof(buf)-1, 0);
            memcpy(data + rs_sum, buf, rs);
            rs_sum += rs;
            bdy_l +=rs;
        }
    }
	return strlen(data);
}
char* get_chunk_end(char * s)
{
	return strstr(s, "\r\n");
}
int num_s(char *hex_str)
{
	unsigned long int l = strlen(hex_str);
	int sum = 0;
	for (int j = 0; j < l; j++) {
		int n ;
		char c =hex_str[j];
		if (isdigit(c))
			n = c - 48;
		else
			n = c - 55;
		sum += n * pow(16, l - 1 - j);
	}
	return sum;
}
void get_content_len(char *s, char **ms, int size)
{
	char *str, *token;
    char *saveptr;
    int i;
    for (i = 0, str = s;; i++,str = NULL) {
        token = strtok_r(str, "\r\n", &saveptr);
        if (token == NULL)
            break;
        if (i == 0) {
            ms[0] = token;  //content_length
            break;
        }
    }
}

long int get_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long int timestamp = tv.tv_sec * 1000000 + tv.tv_usec;
	return timestamp;
}
int req(char *ip, int port, char *path, char *data)
{
	long int t = get_time();
	int sockfd = con(ip, port);
    int size = get_data(sockfd, data, path);
	return sockfd;
}

int main()
{
    char *ip = "127.0.0.1";
	int port = 8000;
    char *path = "/data";
    int size = 8092;
    char *data = (char *)malloc((size) * sizeof(char));
    memset(data, 0, size);
    int sockfd = req(ip, port, path, data);
    free(data);
	close(sockfd);
    return 0;
}
