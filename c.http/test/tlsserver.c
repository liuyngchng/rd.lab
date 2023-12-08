/**
 * openssl genrsa 2048 > ca.key
 * openssl req -new -x509 -nodes -days 1000 -key ca.key -subj /CN=tlsCA\ CA/OU=dev\ group/O=richard\ SIA/DC=tls/DC=com > ca.crt
 * gcc tlsserver.c -lssl -lcrypto
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SERVER_CERT "ca.crt"
#define SERVER_KEY "ca.key"

extern void ERR_print_errors_fp(FILE *fp);

int main() {
    SSL_CTX *ctx;
    SSL *ssl;
    BIO *bio, *accept_bio;
    // 初始化OpenSSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    // 创建SSL上下文
    ctx = SSL_CTX_new(SSLv23_server_method());
    // 加载服务器证书和私钥
    if (SSL_CTX_use_certificate_file(ctx, SERVER_CERT, SSL_FILETYPE_PEM) <= 0) {
        fprintf(stderr, "Error loading server certificate\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, SERVER_KEY, SSL_FILETYPE_PEM) <= 0 ) {
		fprintf(stderr,"Error loading server private key\n");
		ERR_print_errors_fp(stderr);
		exit(1);
	}
	// 创建TCP socket，并绑定到指定端口号
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8888);
	bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	listen(sockfd, 5);
	while (1)
	{
	    printf("Waiting for incoming connection...\n");
	    // 接受客户端连接
	    accept_bio = BIO_new_accept("8888");
	    BIO_do_accept(accept_bio);
	    bio = BIO_pop(accept_bio);
	    // 创建SSL对象并绑定到BIO
	    ssl = SSL_new(ctx);
	    SSL_set_bio(ssl, bio, bio);
	    if (SSL_accept(ssl) <= 0)
		{
			fprintf(stderr,"Error accepting SSL connection\n");
			ERR_print_errors_fp(stderr);
			BIO_free_all(bio);
			continue;
		}
		printf("Connection accepted. Start reading...\n");
		char buf[1024];
        memset(buf, 0, sizeof(buf));
        // 从客户端接收数据
        int len = SSL_read(ssl, buf, sizeof(buf));
        if (len > 0)
        {
            printf("Received data from client: %s\n", buf);
            // 向客户端发送响应
            char response[] = "Hello from server!";
            SSL_write(ssl, response, strlen(response));
        }
        // 关闭SSL连接
        SSL_shutdown(ssl);
        SSL_free(ssl);
        BIO_free_all(bio);
    }
    // 清理资源
    close(sockfd);
    SSL_CTX_free(ctx);
	return 0;
}
