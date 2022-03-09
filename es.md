# 1. setup

[url](https://www.elastic.co/guide/en/elasticsearch/reference/current/configuring-stack-security.html)

-------------------------- Security autoconfiguration information ------------------------------

Authentication and authorization are enabled.
TLS for the transport and HTTP layers is enabled and configured.

The generated password for the elastic built-in superuser is : q+jy_9qzovxwajQRoVRe

If this node should join an existing cluster, you can reconfigure this with
'/usr/share/elasticsearch/bin/elasticsearch-reconfigure-node --enrollment-token <token-here>'
after creating an enrollment token on your existing cluster.

You can complete the following actions at any time:

Reset the password of the elastic built-in superuser with 
'/usr/share/elasticsearch/bin/elasticsearch-reset-password -u elastic'.

Generate an enrollment token for Kibana instances with 
 '/usr/share/elasticsearch/bin/elasticsearch-create-enrollment-token -s kibana'.

Generate an enrollment token for Elasticsearch nodes with 
'/usr/share/elasticsearch/bin/elasticsearch-create-enrollment-token -s node'.

-------------------------------------------------------------------------------------------------
### NOT starting on installation, please execute the following statements to configure elasticsearch service to start automatically using systemd
 sudo systemctl daemon-reload
 sudo systemctl enable elasticsearch.service
### You can start elasticsearch service by executing
 sudo systemctl start elasticsearch.service

# 2. API

## 2.1 LOGIN

https://localhost:9200/

username: elastic
passowrd: q+jy_9qzovxwajQRoVRe

## 2.2 CLUSTER STATUS

https://127.0.0.1:9200/_cluster/health?pretty



# 3.  journal
To tail the journal:

`sudo journalctl -f`

# 4. health check
sudo curl --cacert /etc/elasticsearch/certs/http_ca.crt  'https://127.0.0.1:9200/_cluster/health?pretty' -u elastic

# 5. run

## 5.1  run Elasticsearch as a daemon

```
./bin/elasticsearch -d -p pid
```

## 5.2 reset password

`bin/elasticsearch-reset-password -u elastic`

# 6. ES7.6 Doc

## 6.1 es 7.6

https://www.elastic.co/guide/en/elasticsearch/reference/7.6/targz.html

### 6.1.1 SSL config

* 生成证书

```shell
./bin/elasticsearch-certutil ca
ENTER ENTER
./bin/elasticsearch-certutil cert --ca elastic-stack-ca.p12
ENTER ENTER ENTER
```

将获得可用于加密通信的TLS/SSL证书`elastic-certificates.p12`，将证书复制到`config/certs`的目录中，

`mv elastic-*.p12 ./config/certs/`



```
cd config
vi elasticsearch.yml
```

在`elasticsearch.yml`文件配置如下：

```
xpack.security.enabled: true
xpack.security.transport.ssl.enabled: true
xpack.security.transport.ssl.verification_mode: certificate
xpack.security.transport.ssl.keystore.path: certs/elastic-certificates.p12
xpack.security.transport.ssl.truststore.path: certs/elastic-certificates.p12
```

### 6.1.2  设置内置用户密码

```shell
kill -9 xxxx
./bin/elasticsearch -d -p pid 重启动服务    
./bin/elasticsearch-setup-passwords interactive
// 或自动生成并设置密码
./bin/elasticsearch-setup-passwords auto
```

```shell
Changed password for user apm_system
PASSWORD apm_system = k1YrSWn6Q6iJsASfiiCK

Changed password for user kibana
PASSWORD kibana = 1vBiugd1pu9k4SEJkPN9

Changed password for user logstash_system
PASSWORD logstash_system = lfCYvpqTlUAaYAz8OwPe

Changed password for user beats_system
PASSWORD beats_system = NVgTrZPYOWMRPwrKvKgz

Changed password for user remote_monitoring_user
PASSWORD remote_monitoring_user = RPA0kLi90XUTkTnQfPzf

Changed password for user elastic
PASSWORD elastic = inza42ePLWcTkfxvQykd
```

### 6.1.3 配置HTTP层TLS/SSL加密传输

对于HTTP层通信，Elasticsearch节点仅用作服务器，因此可以使用服务器证书，即TLS/SSL证书不需要启用客户端身份验证。值得注意的是，用于加密HTTP通信的证书可以完全独立于用于传输通信的证书。为了简化操作，我们使用与已用于传输通信的相同的证书，即在elasticsearch.yml文件中配置如下：

```shell
xpack.security.http.ssl.enabled: true
xpack.security.http.ssl.keystore.path: certs/elastic-certificates.p12
xpack.security.http.ssl.truststore.path: certs/elastic-certificates.p12
```

# 7. FileBeats

## 7.1 Install and Config

### 7.1.1 doc

https://www.elastic.co/guide/en/beats/filebeat/7.6/filebeat-installation.html

### 7.1.2 install

```sh
curl -L -O https://artifacts.elastic.co/downloads/beats/filebeat/filebeat-7.6.2-amd64.deb
sudo dpkg -i filebeat-7.6.2-amd64.deb
```

### 7.1.3 config

Here is a sample of the `filebeat` section of the `filebeat.yml` file. Filebeat uses predefined default values for most configuration options.

```yaml
filebeat.inputs:
- type: log
  enabled: true
  paths:
    - /var/log/*.log
    #- c:\programdata\elasticsearch\logs\*
```

```yaml
output.elasticsearch:
  hosts: ["myEShost:9200"]
  username: "filebeat_internal"
  password: "YOUR_PASSWORD" 
```



## 7.2 index

Filebeat uses time series indices, by default, when index lifecycle management is disabled or unsupported. The indices are named `filebeat-7.6.2-yyyy.MM.dd`, where `yyyy.MM.dd` is the date when the events were indexed. To use a different name, you set the [`index`](https://www.elastic.co/guide/en/beats/filebeat/7.6/elasticsearch-output.html#index-option-es) option in the Elasticsearch output. The value that you specify should include the root name of the index plus version and date information. You also need to configure the `setup.template.name` and `setup.template.pattern` options to match the new name. For example:

```sh
output.elasticsearch.index: "customname-%{[agent.version]}-%{+yyyy.MM.dd}"
setup.template.name: "customname"
setup.template.pattern: "customname-*"
```

## 7.3 Setup and run

生成证书

```sh
#生成ca私钥
openssl genrsa 2048 > ca.key
 
#使用ca私钥建立ca证书
openssl req -new -x509 -nodes -days 1000 -key ca.key -subj /CN=elkCA\ CA/OU=dev\ group/O=cnpc\ SIA/DC=elk/DC=com > ca.crt
 
#生成服务器csr证书请求文件
openssl req -newkey rsa:2048 -days 1000 -nodes -keyout server.key -subj /CN=server.t.com/OU=dev\ group/O=cnpc\ SIA/DC=elk/DC=com > server.csr
 
#使用ca证书与私钥签发服务器证书
openssl x509 -req -in server.csr -days 1000 -CA ca.crt -CAkey ca.key -set_serial 01 > server.crt
 
#生成客户端csr证书请求文件
openssl req -newkey rsa:2048 -days 1000 -nodes -keyout client.key -subj /CN=client.t.com/OU=dev\ group/O=cnpc\ SIA/DC=elk/DC=com > client.csr
 
#使用ca证书与私钥签发客户端证书
openssl x509 -req -in client.csr -days 1000 -CA ca.crt -CAkey ca.key -set_serial 01 > client.crt
```



### 7.3.1 ES

创建生产证书的文件

```
./bin/elasticsearch-certutil ca
```

生成证书文件

```

bin/elasticsearch-certutil cert --ca elastic-stack-ca.p12
```

重启ES，

```
./bin/elasticsearch
```

生成证书对应秘钥

```
openssl pkcs12 -in elastic-stack-ca.p12 -out newfile.crt.pem -clcerts -nokeys
```

把这个文件拷入到filebeat目录中

### 7.3.2 config filebeats

```sh
output.elasticsearch:
  # Array of hosts to connect to.
  hosts: ["localhost:9200"]
  index: "%{[fields.source]}-*"
  indices:
    - index: "ops-systemlog-%{+yyyy.MM.dd}"
      when.equals: 
        fields: 
          source: "ops-systemlog"
    #- index: "opslog-operationlog-%{+yyyy.MM.dd}"
    - index: "ops-operationlog-%{+yyyy.MM.dd}"
      when.equals:
        fields:
          #source: "operationlog"
          source: "ops-operationlog"

  # Protocol - either `http` (default) or `https`.
  protocol: "https"
  ssl.verification_mode: none
  ssl.certificate_authorities: ["newfile.crt.pem"]
```

启动filebeat

`sudo filebeat -c /etc/filebeat/filebeat.yml`

