/**
 * openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout ca.pem -out ca.pem
 * openssl genrsa 2048 > ca.key
 * openssl req -new -x509 -nodes -days 1000 -key ca.key -subj /CN=tlsCA\ CA/OU=tlsdev\ group/O=richard\ SIA/DC=rd/DC=com > ca.crt
 * gcc -o _tlsserver tlsserver.c -lssl -lcrypto
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
#include <time.h>
#include <pthread.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#define FAIL -1
#define PORT "8899"
#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x

char *gettime() {
    struct tm *tm_t;
    struct timeval time;
    gettimeofday(&time,NULL);
    tm_t = localtime(&time.tv_sec);
    static char str_time[32]={0};
    sprintf(str_time,
        "%04d-%02d-%02d %02d:%02d:%02d %03ld",
        tm_t->tm_year+1900,
        tm_t->tm_mon+1,
        tm_t->tm_mday,
        tm_t->tm_hour,
        tm_t->tm_min,
        tm_t->tm_sec,
        time.tv_usec/1000
    );
    return str_time;
}

SSL_CTX* initssl(void){
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    // depend on openssl version
    method=TLSv1_2_server_method(); // for OpenSSL 1.0.2g
//    method = TLS_server_method();
    ctx = SSL_CTX_new(method);
    if ( ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
void loadcert(SSL_CTX* ctx, char* CertFile, char* KeyFile){
    if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0){
        ERR_print_errors_fp(stderr);
        abort();
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    if (!SSL_CTX_check_private_key(ctx)) {
        fprintf(stderr, "private key does not match the public certificate\n");
        abort();
    }
}
void showcert(SSL* ssl) {
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
void *acceptssl(void* arg) {
	SSL* ssl=(SSL *)arg;
    char buf[1024]={0};
    int sd, n;
    printf("[%s][%s-%d][t-%lu]SSL accept\n",
    	gettime(), filename(__FILE__), __LINE__, pthread_self());
    if (SSL_accept(ssl) == FAIL) {
        ERR_print_errors_fp(stderr);
    } else {
    	printf("[%s][%s-%d]show cert\n", gettime(), filename(__FILE__), __LINE__);
        showcert(ssl);
        printf("[%s][%s-%d]SSL read\n", gettime(), filename(__FILE__), __LINE__);
        n = SSL_read(ssl, buf, sizeof(buf));
        if (n > 0) {
            buf[n] = 0;
            printf("[%s][%s-%d]rcv %d bytes msg: \n++++\n%s\n++++\n",
            	gettime(), filename(__FILE__), __LINE__, n, buf);
            char resp[4096] = {0};

            char *msg="HTTP/1.1 200 OK\r\n"
            	"Content-Type: application/json\r\n\r\n"
            	"{\"status\":200}";
            strcpy(resp, msg);
//            dispatch(buf,resp);
            SSL_write(ssl, resp, strlen(resp));
            printf("[%s][%s-%d]snd msg: \n----\n%s\n----\n",
            	gettime(), filename(__FILE__), __LINE__, msg);

        } else {
        	printf("[%s][%s-%d]rcv %d bytes, err info\n",
        	    gettime(), filename(__FILE__), __LINE__, n);
            ERR_print_errors_fp(stderr);
        }
    }
    sd = SSL_get_fd(ssl);
    SSL_free(ssl);
    close(sd);
    return NULL;
}
int main(int argc, char *argv[]) {
    SSL_CTX *ctx;
    BIO *acc, *client;
    SSL_library_init();
    ctx = initssl();
    loadcert(ctx, "ca.crt", "ca.key");
    ERR_load_crypto_strings();
    printf("[%s][%s-%d]load cert finish.\n",
		gettime(),filename(__FILE__), __LINE__);
    acc = BIO_new_accept(PORT);
    if (!acc) {
        printf("error creating server socket.\n");
        ERR_print_errors_fp(stderr);
        exit(-1);
    }
    printf("[%s][%s-%d]BIO new accept %s.\n",
		gettime(),filename(__FILE__), __LINE__, PORT);
    BIO_set_bind_mode(acc, BIO_BIND_REUSEADDR);
    while(1) {
    	/* First call to BIO_accept() sets up accept BIO */
        if (BIO_do_accept(acc) <= 0) {
            printf("[%s][%s-%d]error setup accept.\n",
				gettime(),filename(__FILE__), __LINE__);
            ERR_print_errors_fp(stderr);
            exit(-1);
        }
        printf("[%s][%s-%d]BIO setup accept finish.\n",
			gettime(),filename(__FILE__), __LINE__);
        /* Wait for incoming connection */
        printf("[%s][%s-%d]waiting for connection.\n",
        	gettime(),filename(__FILE__), __LINE__);
        if (BIO_do_accept(acc) <= 0) {
			printf("[%s][%s-%d]error accept connection.\n",
				gettime(),filename(__FILE__), __LINE__);
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

        SSL *ssl;
        /* Retrieve BIO for connections */
        printf("[%s][%s-%d]retrieve BIO for connections.\n",
			gettime(),filename(__FILE__), __LINE__);
        client = BIO_pop(acc);
        if (!(ssl = SSL_new(ctx))) {
            printf("error creating SSL ctx.\n");
            ERR_print_errors_fp(stderr);
            exit(-1);
        }
        printf("[%s][%s-%d]BIO pop finish.\n",
        	gettime(),filename(__FILE__), __LINE__);
        SSL_set_bio(ssl, client, client);
        printf("[%s][%s-%d][t-%lu]SSL set bio.\n",
        	gettime(),filename(__FILE__), __LINE__,pthread_self());
        // Here should be created threads
//        acceptssl(ssl);
        pthread_t t;
		pthread_create(&t, NULL, &acceptssl, ssl);
		pthread_detach(t);

    }
    printf("[%s][%s-%d]quit, maybe something goes wrong.\n",
    	gettime(),filename(__FILE__), __LINE__);
    SSL_CTX_free(ctx);
}
