#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXBUF 1024
#define _PORT_ 8899

int main(){
    SSL_CTX *ctx;
    int sockfd, cfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[MAXBUF];
    int n;
    SSL *ssl;

    SSL_library_init();
    ctx = SSL_CTX_new(TLSv1_2_server_method());
    if (SSL_CTX_use_certificate_file(ctx, "ca.pem" , SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        printf("err ca.pem\n");
        exit(1);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "ca.pem", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        printf("err ca.key\n");
        exit(1);
    }
    if (!SSL_CTX_check_private_key(ctx)) {
        ERR_print_errors_fp(stderr);
        printf("err check key\n");
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
    serv_addr.sin_port = htons(_PORT_);
    int i=bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (i < 0) {
        printf("bind failed, port %d", _PORT_);
        exit(1);
    }
    if (listen(sockfd, 10) < 0) {
    	printf("listen failed\n");
        exit(1);
    }
    printf("listening port %d\n", _PORT_);
    clilen = sizeof(cli_addr);
    cfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (cfd < 0) {
        printf("accept failed\n");
        exit(1);
    }
    printf("connected\n");
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, cfd);
    if (SSL_accept(ssl) == -1) {
        ERR_print_errors_fp(stderr);
        printf("ssl accept failed\n");
    } else {
    	printf("TLS connection established.\n");
        //snd buf
    	char *msg="hello, TLS world";
        SSL_write(ssl, msg, strlen(msg));
        printf("send msg %s\n", msg);
        //rcv buf
        bzero(buffer, MAXBUF);
        SSL_read(ssl, buffer, MAXBUF);
        printf("received msg: %s\n", buffer);
    }
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(cfd);
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}
