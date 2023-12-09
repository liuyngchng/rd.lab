all:_tlsserver _tlsclient _tls
_tls: tls.c
	gcc -o _tls tls.c -lssl -lcrypto -lpthread
_tlsserver: tlsserver.c
	gcc -o _tlsserver tlsserver.c -lssl -lcrypto -lpthread
_tlsclient:tlsclient.c
	gcc -o _tlsclient tlsclient.c -lssl -lcrypto

clean:
	rm -fr _* *.o *.so
