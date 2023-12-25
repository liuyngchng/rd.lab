CC=gcc
#CFLAGS += -D__SSL1_0__
CM_OBJ:= utl.o prsr.o pttn.o http.o peer.o rgx.o md5.o cfg.o
CLI_OBJ:=clt.o peer.o http.o utl.o cfg.o
all: obj _tls _tcp _clt _test
obj:tls.c tcp.c pttn.c utl.c prsr.c http.c peer.c rgx.c md5.c cfg.c
	${CC} -c -g -Wpedantic *.c -I /home/rd/workspace/hiredis
clean:
_tls:${CM_OBJ} tls.o
	openssl version
	${CC} -g ${CFLAGS} -o _tls ${CM_OBJ} tls.o -lpthread -lssl -lcrypto
_tcp:${CM_OBJ} tcp.o
	${CC} -g -o _tcp ${CM_OBJ} tcp.o -lpthread
_clt:${CLI_OBJ}
	${CC} -o _clt ${CLI_OBJ}
_test:test.o peer.o http.o utl.o rgx.o md5.o cfg.o hmac_md5.o
	${CC} -g -o _test test.o peer.o http.o utl.o rgx.o md5.o cfg.o hmac_md5.o test/*.o -I /home/rd/workspace/hiredis
clean:
	rm -fr _* *.o *.so

