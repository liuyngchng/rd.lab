ubuntu 22.04

```sh
apt-get install g++
apt-get install cmake
apt-get install protobuf-compiler
apt-get install git
apt-get install liblzma-dev
apt-get install *squashfs-tools* 
```



work dir

```sh
/home/workspace/kwdb_src/src/gitee.com/kwbasedb/bin/kwbase
```

start

```sh
cd /home/workspace/kwdb_src/src/gitee.com/kwbasedb/bin
./start.sh
```





```sh
./kwbase start-single-node --insecure --listen-addr=:26257 --background
```



/home/workspace/kwdb_src/src/gitee.com/kwbasedb/bin/kwbase-data



kwdb_Dockerfile

````sh
touch 	kwdb_Dockerfile
vi		kwdb_Dockerfile
````



```dockerfile
FROM kwdb:2.0.4
VOLUME /tmp
WORKDIR /home/workspace/kwdb_src/src/gitee.com/kwbasedb/bin/
EXPOSE 26257
ENTRYPOINT ["sh", "./start.sh"]
```



```dockerfile
docker build -f ./kwdb_Dockerfile ./ -t kaiwudb:2.0.4
```





```sh
docker run -dit --name kwdbtest -p 26257:26257 \
  -v /data/kwdb/kwbase-data:/home/workspace/kwdb_src/src/gitee.com/kwbasedb/bin/kwbase-data \
  kaiwudb:2.0.4
```





```sh
docker run -dit --name kwdbtest -p 26257:26257 \
  -v /data/kwdb/kwbase-data:/home/workspace/kwdb_src/src/gitee.com/kwbasedb/bin/kwbase-data \
  kwdb:2.0.4
```

