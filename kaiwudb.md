# 编译

文档详见 https://www.kaiwudb.com/kaiwudb_docs/#/

https://gitee.com/kaiwuDB-organiization/docs_kaiwuDB

ubuntu 22.04

```sh
yum install gcc*
apt-get install g++
apt-get install cmake
apt-get install protobuf-compiler
apt-get install git
apt-get install liblzma-dev
apt-get install squashfs-tools 
```

work dir

```sh
/home/workspace/kwdb_src/src/gitee.com/kwbasedb/bin/kwbase
```

# 启动

start

```sh
cd /home/workspace/kwdb_src/src/gitee.com/kwbasedb/bin
./start.sh
```



```sh
./kwbase start-single-node --insecure --listen-addr=:26257 --background
./kwbase start-single-node --insecure --listen-addr=0.0.0.0:26257 --advertise-addr=127.0.0.1:26257 --http-addr=0.0.0.0:8088
```

# dockerfile

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

# run

```sh
docker run --rm --security-opt seccomp=unconfined -dit --name kwdbtest -p 26257:26257 -p 8088:8088 \
  -v /data/kwdb/kwbase-data:/home/workspace/kwdb_src/src/gitee.com/kwbasedb/bin/kwbase-data \
  kwdb:2.0.4
```

# client 连接

## insecure

```sh
./kwbase sql --insecure --host=<your-host-ip>
```

## secure

```sh
./kwbase sql --host=<your-host-ip> -u user  --certs-dir ./certs
```



# monitor

```sh
http://127.0.0.1:8088/_status/vars
```

promethes config

```yml
# Prometheus configuration for kaiwudb clusters.
# Requires prometheus 2.X
#
# Run with:
# $ prometheus -config.file=prometheus.yml
global:
  scrape_interval: 10s
  evaluation_interval: 10s

rule_files:
- "rules/alerts.rules.yml"
- "rules/aggregation.rules.yml"

scrape_configs:
  - job_name: 'kaiwudb'
    metrics_path: '/_status/vars'
    # Insecure mode:
    scheme: 'http'
    # Secure mode:
    # scheme: 'https'
    tls_config:
      insecure_skip_verify: true

    static_configs:
    - targets: ['localhost:8080', 'localhost:8081', 'localhost:8082']
      labels:
        cluster: 'my-kaiwudb-cluster'
```

# sql

```
CREATE TS DATABASE iot_gas_meter;
```

