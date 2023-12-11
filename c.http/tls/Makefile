all: _test
_test: tlstest.c
	gcc -o _test test.c -lssl -lcrypto -lpthread

clean:
	rm -fr _* *.o *.so
