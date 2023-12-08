all:_tlsserver _tlsclient
#prsr:prsr.c
#	gcc -o ./libparser.so -fPIC -shared prsr.c
_tlsserver: tlsserver.c
	gcc -o _tlsserver tlsserver.c -lssl -lcrypto
_tlsclient:tlsclient.c
	gcc -o _tlsclient tlsclient.c -lssl -lcrypto

clean:
	rm -fr _* *.o *.so
