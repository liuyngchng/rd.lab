/**
 * TLS peer connection
 * @author whoami@hotmail.com
 * @since 2022-06-01
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

#include "utl.h"


SSL_CTX* initprssl(void){
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    // depend on openssl version
#ifdef __SSL1_0__
    method=TLSv1_2_client_method(); // for OpenSSL 1.0.2g
    printf("[%s][%s-%d][t-%ld]TLSv1_2_client_method\n", gettime(), filename(__FILE__), __LINE__, pthread_self());
#else
    method = TLS_client_method();	// for OpenSSL 1.1.1f
    printf("[%s][%s-%d][t-%ld]TLS_client_method\n", gettime(), filename(__FILE__), __LINE__, pthread_self());
#endif
    ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
    	printf("[%s][%s-%d]ctx null err\n", gettime(), filename(__FILE__), __LINE__);
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
char *const tlsreq(const char *ip, const int port, const char *msg, char *const resp, int n) {
    SSL_CTX *ctx;
    SSL *ssl;
    BIO *conn;
    int bytes;
    SSL_library_init();
    ctx = initprssl();
    char srv_prt[128]={0};
    sprintf(srv_prt, "%s:%d", ip, port);
    printf("[%s][%s-%d]tls connecting %s\n", gettime(), filename(__FILE__), __LINE__, srv_prt);
    conn = BIO_new_connect(srv_prt);
    if (!conn) {
        printf("[%s][%s-%d]error BIO new connect\n", gettime(), filename(__FILE__), __LINE__);
        return resp;
    }
    if (BIO_do_connect(conn) <= 0) {
        printf("[%s][%s-%d]error BIO do connect\n", gettime(), filename(__FILE__), __LINE__);
        return resp;
    }
    ssl = SSL_new(ctx);
    SSL_set_bio(ssl, conn, conn);
    if (SSL_connect(ssl) <= 0){
        printf("[%s][%s-%d]error SSL connect\n", gettime(), filename(__FILE__), __LINE__);
        return resp;
    }
    printf("[%s][%s-%d][t-%ld]connected to %s\n", gettime(), filename(__FILE__), __LINE__,
    	pthread_self(), srv_prt);
//	char *msg = "hello, ssl world";
	SSL_write(ssl, msg, strlen(msg));
	bytes = SSL_read(ssl, resp, n);
	printf("[%s][%s-%d]received msg: %s\n", gettime(), filename(__FILE__), __LINE__, resp);
	SSL_free(ssl);
    SSL_CTX_free(ctx);
    return resp;
}
