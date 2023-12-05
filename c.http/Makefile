all:_srvr _clnt _test
#prsr:prsr.c
#	gcc -o ./libparser.so -fPIC -shared prsr.c
_srvr:srvr.c util.c prsr.c pttn.c http.c peer.c rgx.c md5.c
	gcc -g -o _srvr srvr.c pttn.c util.c prsr.c http.c peer.c rgx.c md5.c -lpthread
_clnt:clnt.c peer.c http.c util.c
	gcc -o _clnt clnt.c peer.c http.c util.c
_test:test.c peer.c http.c util.c rgx.c md5.c
	gcc -g -o _test test.c peer.c http.c util.c rgx.c md5.c
clean:
	rm -fr _* *.o *.so
	
