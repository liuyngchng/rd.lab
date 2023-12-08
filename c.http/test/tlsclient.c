#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8888

extern void ERR_print_errors_fp(FILE *fp);

int main() {
    SSL_CTX *ctx;
    SSL *ssl;
    BIO *bio;
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDRESS, &(serv_addr.sin_addr));
	connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	// 创建BIO对象，并与socket连接
	bio = BIO_new_socket(sockfd, BIO_NOCLOSE);
	// 创建SSL对象并绑定到BIO
	ssl = SSL_new(ctx);
	SSL_set_bio(ssl, bio, bio);
	if (SSL_connect(ssl) <= 0) {
	    fprintf(stderr,"Error connecting to SSL server\n");
	    ERR_print_errors_fp(stderr);
	    SSL_free(ssl);
	    exit(1);
	}
	printf("Connected to server. Start sending data...\n");
	char request[] = "Hello from client!";
    SSL_write(ssl, request, strlen(request));
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    // 从服务器接收响应
    int len = SSL_read(ssl, buf,sizeof(buf));
    if (len > 0) {
        printf("Received response from server: %s\n", buf);
    }
    // 关闭SSL连接
    SSL_shutdown(ssl);
    SSL_free(ssl);
	// 清理资源
	close(sockfd);
	SSL_CTX_free(ctx);
	return 0;
}
