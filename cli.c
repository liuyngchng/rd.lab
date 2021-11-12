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
#include <math.h>
#include <ctype.h>
#define _BUF_SIZE_ 8096

long int get_time();
int get_data(int sockfd, char* data);
void split_stream_head(char *s, char **ms, int size);
void split_stream_body(char *s, char **ms, int size);
int num_s(char *hex_str);
void set_sockopt(int sockfd);

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
//	srv_sock.sin_addr.s_addr = inet_addr(ip);

	int ret = connect(sockfd, (struct sockaddr*)& srv_sock, sizeof(srv_sock));
	if (ret < 0) {
		printf("connect to %s:%d\n", ip, port);
		printf("error, errno is %d, %s\n",errno, strerror(errno));
		return 1;
	}
	printf("connected to %s:%d\n", ip, port);
	return sockfd;
}


int get_data(int sockfd, char* data)
{
	char hdr[4096], param[4096], buf[_BUF_SIZE_], *bdh, *end;
	memset(hdr, 0, sizeof(buf));
	memset(param, 0, 4096);
	memset(buf, 0, sizeof(buf));
	strcat(param, "?a=1&b=2");
	memset(hdr, 0, 4096);
	strcat(hdr, "GET /data");
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
    if (ss < 0)
    {
        printf("snd fail, err_code = %d，err_msg = '%s'\n",errno, strerror(errno));
        exit(0);
    } else {
        printf("snd %d bytes\n", ss);
    }
    int rs_sum = 0;
    int rs = recv(sockfd, buf, sizeof(buf)-1, 0);
    if (rs == 0)
    {
        close(sockfd);
        printf("read failed！\n");
        exit(-1);
    }
    if (strstr(buf, "Content-Length") != NULL)
    {
        printf("Content-Length contained in data\n");
    }
    else if (strstr(buf, "Transfer-Encoding: chunked") != NULL)
    {
        printf("chunk_data\n");
//        printf("%s\n", buf);
        memcpy(data + rs_sum, buf, rs);
        rs_sum += rs;
        bdh = strstr(buf, "\r\n\r\n");
        end = strstr(bdh + 4, "\r\n\r\n");
        while(end == NULL)
        {
            memset(buf, 0, sizeof(buf));
            rs = recv(sockfd, buf, sizeof(buf)-1, 0);
            memcpy(data + rs_sum, buf, rs);
            rs_sum += rs;
            end = strstr(buf, "\r\n\r\n");
        }
    }
    else
    {
        //todo
        printf("%ld\tnothing done.\n", get_time());
    }
	return strlen(data);
}

void set_sockopt(int sockfd)
{
    unsigned int opt_val = 0;
	unsigned int opt_len = sizeof(int);
    // snd buf
	getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&opt_val, &opt_len);
	printf("snd_buf len=%d\n", opt_val);
	opt_val = 10;
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&opt_val, opt_len);
	opt_val = 0;
	getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&opt_val, &opt_len);
	printf("snd_buf len=%d\n", opt_val);
	// rcv buf
	getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&opt_val, &opt_len);
	printf("rcv_buf len=%d\n", opt_val);
	opt_val = 10;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&opt_val, opt_len);
	opt_val = 0;
	getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&opt_val, &opt_len);
	printf("rcv_buf len=%d\n", opt_val);
}

char* get_chunk_end(char * s)
{
	return strstr(s, "\r\n");
}

int num_s(char *hex_str)
{
	unsigned long int l = strlen(hex_str);
	int sum = 0;
	for (int j = 0; j < l; j++)
	{
		int n ;
		char c =hex_str[j];
		if isdigit(c)
			n = c - 48;
		else
			n = c - 55;
		sum += n * pow(16, l - 1 - j);
	}
	return sum;
}

/**
  @param s, stream
  @param ms, string array, ms[0], header, ms[1] length, ms[2] body
  @param size ,size of ms
 */
void split_stream_head(char *s, char **ms, int size)
{
    char *len_f = strstr(s, "Content-Length:");
    if (len_f != NULL)
    {
        //todo get conetent length from header.
    }
    char *sub_str = strstr(s, "\r\n\r\n");
    if (NULL == sub_str)
    {
        sub_str = s;
    }
    ms[0] = "ignored_header\0";
	char *str, *token;
    char *saveptr;
    int i;
    for (i = 0, str = sub_str;; i++,str = NULL)
    {
        token = strtok_r(str, "\r\n", &saveptr);
        if (token == NULL)
            break;
        if (i == 0)
        {
            ms[1] = token;  //length
            if (num_s(ms[1]) < 0)
            {
                printf("error_in_split\n[%s]\n", s);
            }
        }
        else if (i == 1)
        {
            ms[2] = token;  //body
        }
    }
}

void split_stream_body(char *s, char **ms, int size)
{
    ms[0] = "no_header\0";
	char *str, *token;
    char *saveptr;
    int i;
    for (i = 0, str = s;; i++,str = NULL)
    {
        token = strtok_r(str, "\r\n", &saveptr);
        if (token == NULL)
            break;
        if (i == 0)
        {
            ms[1] = token;  //length
            if (num_s(ms[1]) < 0)
            {
                printf("error_in_split_stream_body\n[%s]\n", s);
            }
        }
        else if (i == 1)
        {
            ms[2] = token;  //body
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

int main()
{
	printf("start\n");
	char *ip = "127.0.0.1";
	int port = 8082;
	long int t = get_time();
	int sockfd = con(ip, port);
    int size = 8092;
    char *data = (char *)malloc((size) * sizeof(char));
    for (int i = 0;i< 10000; i++)
    {
        memset(data, 0, size);
        printf("---req_start_turn=%d---\n", i);
        int size = get_data(sockfd, data);
        printf("%s\n", data);
        printf("---req__end__turn=%d/size=%d---\n", i, size);
        if(size < 0)
        {
            printf("size is zero\n");
            break;
        }


	}
	free(data);
	close(sockfd);
	printf("%ldus elapsed\n", get_time() - t);
	return 0;
}
