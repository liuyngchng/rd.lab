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
char* get_data(int sockfd, char* data);
void split(char *s, char **ms, int size);
int num_s(char *hex_str);
void set_sockopt(int sockfd);

int con(char *ip, int port)
{
	printf("con to %s:%d\n", ip, port);
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


char* get_data(int sockfd, char* data)
{
	char str1[4096], str2[4096], buf[_BUF_SIZE_], *str;
    socklen_t len;
	//send data
	memset(str2, 0, 4096);
	strcat(str2, "?a=1&b=2");
	str=(char *)malloc(128);
	len = strlen(str2);
	sprintf(str, "%d", len);
	memset(str1, 0, 4096);
	strcat(str1, "GET /data");
	strcat(str1, str2);
	strcat(str1, " HTTP/1.1\r\n");
	strcat(str1, "Accept: */*\r\n");
	strcat(str1, "Accept-Language: zh-cn\r\n");
	strcat(str1, "User-Agent: rdAgent\r\n");
	strcat(str1, "Host: 127.0.0.1:80\r\n");
	strcat(str1, "Content-Type: application/json;charset=UTF-8\r\n");
	strcat(str1, "Connection: keep-alive\r\n");
	strcat(str1, "\r\n");
    int ss = send(sockfd,str1,strlen(str1), 0);
    if (ss < 0)
    {
        printf("snd fail, err_code = %d，err_msg = '%s'\n",errno, strerror(errno));
        exit(0);
    } else {
        printf("snd %d byte\n", ss);
    }
    memset(buf, 0, sizeof(buf));
    printf("rcving\n");
    int rs;
    rs = recv(sockfd, buf, sizeof(buf), 0);
    //int rs = read(sockfd, buf, sizeof(buf));
    if (rs==0)
    {
        close(sockfd);
        printf("read faild！\n");
        return NULL;
    }
    int size = 3;
    char **info = (char **)malloc((size +1) * sizeof(char *));
    split(buf, info, size);
    int c_l = num_s(info[1]);           // chunk length
    printf("c_l=%d\n", c_l);
    char *body_head = info[2];
    char ctt[c_l + 1];
    memset(ctt, 0, sizeof(ctt));
    ctt[c_l] = '\0';
    int offset = 0;
    int bd_h_l = strlen(body_head);
    memcpy(ctt + offset, body_head, bd_h_l);
    free(info);
    offset += bd_h_l;
    while(offset < c_l)
    {
        memset(buf, 0, sizeof(buf));
        rs = recv(sockfd, buf, sizeof(buf), 0);
        char *chunk_end = strstr(buf, "\r\n");
        char *real_buf;
        if (chunk_end == NULL)
            real_buf = buf;
        else
        {
            char *token, *new_l, *saveptr;
            int i;
            for (i = 0, str = buf;; i++, str = NULL)
            {
                token = strtok_r(str, "\r\n", &saveptr);
                if (token == NULL)
                    break;
                if (i == 0)
                    real_buf = token;
                else if (i == 1)
                {
                    new_l = token;      //new chunk length
                    printf("new chunk length=%s\n", new_l);
                }

            }
            rs = strlen(real_buf);
        }
        memcpy(ctt + offset, real_buf, rs);
        offset += rs;
    }
	return ctt;
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
void split(char *s, char **ms, int size)
{
    char *len_f = strstr(s, "Content-Length:");
    if (len_f != NULL)
    {
        //todo get conetent length from header.
    }
    char *sub_str = strstr(s, "\r\n\r\n");
    ms[0] = "ignored_header";
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
            printf("chunk_length=%s\n", ms[1]);
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
	char ip[] = "127.0.0.1";
	int port = 8082;
//     connect(ip, port);
	long int t = get_time();
	printf("%ld\n", get_time());
	int sockfd = con(ip, port);
//	int sockfd = conn();
	char *response = get_data(sockfd);
	printf("%s\n", response);
	printf("%ldus elapsed\n", get_time() - t);
	return 0;
}
