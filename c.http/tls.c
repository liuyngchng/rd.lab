/**
 * TLS util with openSSL
 * @author whoami@hotmail.com
 * @since 2022-06-01
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
#include <sys/time.h>
#include <pthread.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "pttn.h"
#include "cfg.h"
#include "utl.h"
extern int _SRV_PORT_;
//#define _SRV_PORT_ "8899"
#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x

pthread_key_t tdt;

void destructor(void* data) {
    printf("[%s][%s-%d]free thread-specific data\n", gettime(),
		filename(__FILE__),__LINE__);
    free(data);
}

SSL_CTX* initssl(void){
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    // depend on openssl version
#ifdef __SSL1_0__
    method=TLSv1_2_server_method(); // for OpenSSL 1.0.2g
#else
    method = TLS_server_method();	// for OpenSSL 1.1.1f
#endif
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
    if (SSL_accept(ssl) == -1) {
        ERR_print_errors_fp(stderr);
    } else {
    	printf("[%s][%s-%d]show cert\n", gettime(), filename(__FILE__), __LINE__);
        showcert(ssl);
        printf("[%s][%s-%d]SSL read\n", gettime(), filename(__FILE__), __LINE__);
        n = SSL_read(ssl, buf, sizeof(buf));
        unsigned long *tidp = NULL;
		tidp = (unsigned long *)malloc(sizeof(unsigned long));
		*tidp = gettimestamp();
		int tmp = pthread_setspecific(tdt,tidp);
        if (n > 0) {
            buf[n] = 0;
            printf("[%s][%s-%d]rcv %d bytes msg,"
            	"tdt %lu, pthread_getspecific %lu\n++++\n%s\n++++\n",
            	gettime(), filename(__FILE__), __LINE__, n,
				*tidp, *(unsigned long *)pthread_getspecific(tdt), buf);
            char resp[4096] = {0};
//            char *msg="HTTP/1.1 200 OK\r\n"
//            	"Content-Type: application/json\r\n\r\n"
//            	"{\"status\":200}";
//            strcpy(resp, msg);
            dispatch(buf,resp);
            SSL_write(ssl, resp, strlen(resp));
            printf("[%s][%s-%d]snd msg: \n----\n%s\n----\n",
            	gettime(), filename(__FILE__), __LINE__, resp);

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

int starttlssrv() {
    SSL_CTX *ctx;
    BIO *acc, *client;
    SSL_library_init();
    ctx = initssl();
    loadcert(ctx, "./config/ca.crt", "./config/ca.key");
    ERR_load_crypto_strings();
    printf("[%s][%s-%d]load cert finish.\n",
		gettime(),filename(__FILE__), __LINE__);
    char port[8]={0};
    sprintf(port, "%d", _SRV_PORT_);
    acc = BIO_new_accept(port);
    if (!acc) {
        printf("error creating server socket.\n");
        ERR_print_errors_fp(stderr);
        exit(-1);
    }
    printf("[%s][%s-%d]BIO new accept %s.\n",
		gettime(),filename(__FILE__), __LINE__, port);
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
        printf("[%s][%s-%d]waiting for connection,_srv_started.\n",
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

int main(int argc, char* argv[]) {
	initcfg();
	pthread_key_create(&tdt, destructor);
    printf("[%s][%s-%d][t-%ld]server starting\n", gettime(), filename(__FILE__), __LINE__, pthread_self());
    starttlssrv();
    return 0;
}
