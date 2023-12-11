CM_OBJ:= util.o prsr.o pttn.o http.o peer.o rgx.o md5.o cfg.o
CLI_OBJ:=clnt.o peer.o http.o util.o cfg.o
all: obj _tlssrvr _srvr _clnt _test
obj:tlssrvr.c srvr.c pttn.c util.c prsr.c http.c peer.c rgx.c md5.c cfg.c
	gcc -c *.c
_tlssrvr:${CM_OBJ} tlssrvr.o
	gcc -g -o _tlssrvr ${CM_OBJ} tlssrvr.o -lpthread -lssl -lcrypto  -ldl
_srvr:${CM_OBJ} srvr.o
	gcc -g -o _srvr ${CM_OBJ} srvr.o -lpthread
_clnt:${CLI_OBJ}
	gcc -o _clnt ${CLI_OBJ}
_test:test.c peer.c http.c util.c rgx.c md5.c cfg.c
	gcc -g -o _test test.c peer.c http.c util.c rgx.c md5.c cfg.c
clean:
	rm -fr _* *.o *.so

