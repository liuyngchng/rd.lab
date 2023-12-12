CC=gcc
#CFLAGS += -D__SSL1_0__
CM_OBJ:= util.o prsr.o pttn.o http.o peer.o rgx.o md5.o cfg.o
CLI_OBJ:=clnt.o peer.o http.o util.o cfg.o
all: obj _tlssrvr _srvr _clnt _test
obj:tlssrvr.c srvr.c pttn.c util.c prsr.c http.c peer.c rgx.c md5.c cfg.c
	${CC} -c *.c
_tlssrvr:${CM_OBJ} tlssrvr.o
	openssl version
	${CC} -g ${CFLAGS} -o _tlssrvr ${CM_OBJ} tlssrvr.o -lpthread -lssl -lcrypto  -ldl
_srvr:${CM_OBJ} srvr.o
	${CC} -g -o _srvr ${CM_OBJ} srvr.o -lpthread
_clnt:${CLI_OBJ}
	${CC} -o _clnt ${CLI_OBJ}
_test:test.c peer.c http.c util.c rgx.c md5.c cfg.c
	${CC} -g -o _test test.c peer.c http.c util.c rgx.c md5.c cfg.c
clean:
	rm -fr _* *.o *.so

