gcc -o _tlsserver tlsserver.c -lssl -lcrypto
gcc -o _tlsclient tlsclient.c -lssl -lcrypto
gcc -o _client1 client1.c -lssl -lcrypto
gcc -o _server1 server1.c -lssl -lcrypto
