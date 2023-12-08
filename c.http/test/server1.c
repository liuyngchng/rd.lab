/**
 * https://www.oomake.com/question/4087737
 * openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout ca.pem -out ca.pem
 * gcc -o _server1 server1.c -lssl -lcrypto
 */
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#define FAIL -1
#define PORT "8899"
SSL_CTX* InitServerCTX(void){
    SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = TLS_server_method();
    ctx = SSL_CTX_new(method);
    if ( ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile){
    if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0){
        ERR_print_errors_fp(stderr);
        abort();
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    if (!SSL_CTX_check_private_key(ctx)) {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}
void ShowCerts(SSL* ssl) {
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    } else {
        printf("No certificates.\n");
    }
}
void Servlet(SSL* ssl) {
    char buf[1024];
    int sd, bytes;
    if (SSL_accept(ssl) == FAIL) {
        ERR_print_errors_fp(stderr);
    } else {
        ShowCerts(ssl);
        bytes = SSL_read(ssl, buf, sizeof(buf));
        if (bytes > 0) {
            buf[bytes] = 0;
            printf("received msg: \"%s\"\n", buf);
            char *msg="server ack message";
            SSL_write(ssl, msg, strlen(msg));
        } else {
            ERR_print_errors_fp(stderr);
        }
    }
    sd = SSL_get_fd(ssl);
    SSL_free(ssl);
    close(sd);
}
int main(int count, char *strings[]) {
    SSL_CTX *ctx;
    BIO *acc, *client;
    SSL_library_init();
    ctx = InitServerCTX();
    LoadCertificates(ctx, "ca.crt", "ca.key");
    acc = BIO_new_accept(PORT);
    if (!acc) {
        printf("Error creating server socket");
    }
    while(1) {
        if (BIO_do_accept(acc) <= 0) {
            printf("Error binding server socket");
            continue;
        }
        SSL *ssl;
        client = BIO_pop(acc);
        if (!(ssl = SSL_new(ctx))) {
            printf("Error creating SSL context");
            continue;
        }
        SSL_set_bio(ssl, client, client);
        // Here should be created threads
        Servlet(ssl);
    }
    printf("server quit, maybe something go wrong.");
    SSL_CTX_free(ctx);
}
