#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXBUF 1024

int main(){
    SSL_CTX *ctx;
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[MAXBUF];
    int n;
    SSL *ssl;

    SSL_library_init();
    ctx = SSL_CTX_new(TLSv1_2_server_method());
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem" , SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    if (!SSL_CTX_check_private_key(ctx)) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    //创建套接字并绑定端口
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("创建socket失败");
        exit(1);
    }
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8888);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("绑定失败");
        exit(1);
    }
    if (listen(sockfd, 10) < 0) {
        perror("监听失败");
        exit(1);
    }
    printf("等待客户端连接......\n");
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("连接失败");
        exit(1);
    }
    printf("连接已建立......\n");
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, newsockfd);
    if (SSL_accept(ssl) == -1) {
        ERR_print_errors_fp(stderr);
    } else {
    	printf("TLS连接已建立......\n");
        //发送
        SSL_write(ssl, "你好，TLS\n", strlen("你好，TLS\n"));
        printf("已发送数据: 你好，TLS\n");
        //接收
        bzero(buffer, MAXBUF);
        SSL_read(ssl, buffer, MAXBUF);
        printf("已收到数据: %s\n", buffer);
    }
    //关闭连接并释放资源
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(newsockfd);
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}
