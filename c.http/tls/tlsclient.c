/**
 * gcc -o _tlsclient tlsclient.c -lssl -lcrypto -Wall
 */
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#define FAIL -1
#define SERVER "127.0.0.1"
#define PORT "8899"

//extern SSL_METHOD *TLS_client_method();

SSL_CTX* initssl(void){
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    // depend on openssl version
//    method=TLSv1_2_client_method();	// for OpenSSL 1.0.2g
    method = TLS_client_method();	// for OpenSSL 1.1.1f
    ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
void showcert(SSL* ssl){
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("issuer: %s\n", line);
        free(line);
        X509_free(cert);
    } else {
        printf("no certificates.\n");
    }
}
int main(int argc, char *argv[]) {
    SSL_CTX *ctx;
    SSL *ssl;
    BIO *conn;
    char buf[1024]={0};
    int bytes;
    SSL_library_init();
    ctx = initssl();
    conn = BIO_new_connect(SERVER ":" PORT);
    if (!conn) {
        printf("error creating connection BIO\n");
        return -1;
    }
    if (BIO_do_connect(conn) <= 0) {
        printf("error connecting to remote machine");
        return -1;
    }
    ssl = SSL_new(ctx);
    SSL_set_bio(ssl, conn, conn);
    if (SSL_connect(ssl) <= 0){
        printf("error connecting SSL object");
        return -1;
    }
    printf("connected!\n");
	showcert(ssl);
	char *msg = "hello, ssl world";
	SSL_write(ssl, msg, strlen(msg));
	bytes = SSL_read(ssl, buf, sizeof(buf));
	printf("received msg: %s\n", buf);
	SSL_free(ssl);
    SSL_CTX_free(ctx);
    return 0;
}
