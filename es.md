# setup

##     doc

https://www.elastic.co/guide/en/elasticsearch/reference/7.6/targz.html

[url](https://www.elastic.co/guide/en/elasticsearch/reference/current/configuring-stack-security.html)

```sh
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
```
##      env config

openfiles 配置

```sh
vi /etc/security/limits.conf  
# 添加以下内容，机器重启生效
# End of file
soft nofile 65535  
hard nofile 65535   
*               soft    nproc         655535  
*               hard    nproc         655535  
*               soft    nofile        655535  
*               hard    nofile        655535  
root            soft    nofile        655535  
root            hard    nofile        655535  
root            soft    nproc         655535  
root            hard    nproc         655535

# reboot
reboot
# 验证 open files 是否增大
ulimit -a
```

虚拟内存配置

```sh
# 切换 root
su
sysctl -w vm.max_map_count=262144
```

##     传输层 SSL config

* 生成证书

```sh
cd elasticsearch-7.6.1
./bin/elasticsearch-certutil ca
ENTER ENTER
./bin/elasticsearch-certutil cert --ca elastic-stack-ca.p12
ENTER ENTER ENTER
```

此时，根目录生成了2个.p12文件

```sh
cd elasticsearch-7.6.1
ls | grep p12
elastic-certificates.p12
elastic-stack-ca.p12
```

将获得可用于加密通信的TLS/SSL证书`elastic-certificates.p12`，将证书复制到`config/certs`的目录中，

```sh
cd elasticsearch-7.6.1
mkdir ./config/certs
mv elastic-*.p12 ./config/certs/
```

* 配置 SSL

```sh
vi ./config/elasticsearch.yml
```

在`elasticsearch.yml`文件末尾，新增以下配置：

```sh
xpack.security.enabled: true
xpack.security.transport.ssl.enabled: true
xpack.security.transport.ssl.verification_mode: certificate
xpack.security.transport.ssl.keystore.path: certs/elastic-certificates.p12
xpack.security.transport.ssl.truststore.path: certs/elastic-certificates.p12
```


##     开启远程访问

elastic 默认无法远程IP 进行访问，若需要通过远程IP进行访问，则，需要尽心如下配置

```sh
vi ./config/elasticsearch.yml
# 修改以下内容
network.host: 192.168.1.111				# your real IP addr
http.port: 9200
discovery.seed_hosts: ["127.0.0.1", "[::1]"]
```

##    设置用户密码

```shell
# 重启动服务
kill -9 xxxx
# 开始 xpack security
xpack.security.enabled: true
./bin/elasticsearch -d -p pid  
# 手动设置密码
./bin/elasticsearch-setup-passwords interactive
# 自动生成并设置密码
./bin/elasticsearch-setup-passwords auto
```
会看到
```sh
Changed password for user apm_system
PASSWORD apm_system = ****

Changed password for user kibana
PASSWORD kibana = ****

Changed password for user logstash_system
PASSWORD logstash_system = ****

Changed password for user beats_system
PASSWORD beats_system = ****

Changed password for user remote_monitoring_user
PASSWORD remote_monitoring_user = ****

Changed password for user elastic
PASSWORD elastic = ****
```

此时说明登录已设置成功，浏览网页 http://IP:9200 时，则会提示输入密码

##   配置 HTTP 层 TLS/SSL加密传输

对于HTTP层通信，Elasticsearch节点仅用作服务器，因此可以使用服务器证书，即TLS/SSL证书不需要启用客户端身份验证。值得注意的是，用于加密HTTP通信的证书可以完全独立于用于传输通信的证书。为了简化操作，我们使用与已用于传输通信的相同的证书，即在elasticsearch.yml文件中配置如下：

```shell
xpack.security.http.ssl.enabled: true
xpack.security.http.ssl.keystore.path: certs/elastic-certificates.p12
xpack.security.http.ssl.truststore.path: certs/elastic-certificates.p12
```

这样就能通过 HJTTPS进行浏览了，  https://IP:9200 

##  启动

```sh
./bin/elasticsearch -d
```



# API

##     LOGIN

https://localhost:9200/

username: elastic
passowrd: ****

##     CLUSTER STATUS



```sh
curl -k --tlsv1  'https://127.0.0.1:9200/_cluster/health?pretty' -u elastic:****
```



##  search

```sh
curl -k --tlsv1  'https://127.0.0.1:9200/index_name/_search?pretty' -u elastic:******



curl  -k --tlsv1  -X GET "https://11.10.36.1:9200/filebeat-rd-7.6.2-2022.05.16/_search?pretty" -H 'Content-Type: application/json' -u elastic:PufPoocYcRBYtUXe2xoe -d'{"from":0,"size":10,"query":{"match_all":{"boost":1.0}},"sort":[{"@timestamp":{"order":"desc"}}]}' 

```

## grok

通过 grok 将非结构化文本解析为结构化文本

添加名称为`example_grok_pipeline`的管道

```sh
curl  -k --tlsv1  -X PUT "https://11.10.36.1:9200/_ingest/pipeline/example_grok_pipeline" -H 'Content-Type: application/json' -u elastic:PufPoocYcRBYtUXe2xoe -d'
{ 
  "description": "A simple example of using Grok", 
  "processors": [ 
    { 
      "grok": { 
        "field": "message", 
        "patterns": [ 
          "%{IP:host.ip} %{WORD:http.request.method} %{URIPATHPARAM:url.original} %{NUMBER:http.request.bytes:int} %{NUMBER:event.duration:double} %{GREEDYDATA}" 
        ] 
      } 
    } 
  ] 
}'
```







```sh
curl  -k --tlsv1  -X POST "https://11.10.36.1:9200/_ingest/pipeline/example_grok_pipeline/_simulate?pretty" -H 'Content-Type: application/json' -u elastic:PufPoocYcRBYtUXe2xoe  -d '{
  "docs": [
    {
      "_source": {
        "message": "55.3.244.1 GET /index.html 15824 0.043 other stuff"
      }
    }
  ]
}'
```





#  journal
To tail the journal:

`sudo journalctl -f`

# health check and Other API
##     health check

```sh
sudo curl --cacert /etc/elasticsearch/certs/http_ca.crt  'https://127.0.0.1:9200/_cluster/health?pretty' -u elastic
//不使用证书
curl -k --tlsv1  'https://127.0.0.1:9200/_cluster/health?pretty' -u elastic:inza42ePLWcTkfxvQykd
```

##     index list

```sh
https://127.0.0.1:9200/_cat/indices?format=json&index=[索引名称，可使用通配符]
curl -k --tlsv1  'https://127.0.0.1:9200/_cat/indices?format=json' -u elastic:inza42ePLWcTkfxvQykd
```

#  run

##      run Elasticsearch as a daemon

```
./bin/elasticsearch -d -p pid
```

##      reset password

```sh
bin/elasticsearch-reset-password -u elastic
```




#  FileBeats

##    Install and Config

###    doc

https://www.elastic.co/guide/en/beats/filebeat/7.6/filebeat-installation.html

###    install

```sh
# ubuntu
curl -L -O https://artifacts.elastic.co/downloads/beats/filebeat/filebeat-7.6.2-amd64.deb
sudo dpkg -i filebeat-7.6.2-amd64.deb
# linux
curl -L -O https://artifacts.elastic.co/downloads/beats/filebeat/filebeat-8.2.0-linux-x86_64.tar.gz
tar xzvf filebeat-8.2.0-linux-x86_64.tar.gz
```

###     config

Here is a sample of the `filebeat` section of the `filebeat.yml` file. Filebeat uses predefined default values for most configuration options.

```sh
cd filebeat-7.6.2-linux-x86_64/
vi filebeat.yml
```

配置内容如下所示，其中 `filebeat.inputs.paths` 是 `filebeats`需要读取的日志文件

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



##   index

Filebeat uses time series indices, by default, when index lifecycle management is disabled or unsupported. The indices are named `filebeat-7.6.2-yyyy.MM.dd`, where `yyyy.MM.dd` is the date when the events were indexed. To use a different name, you set the [`index`](https://www.elastic.co/guide/en/beats/filebeat/7.6/elasticsearch-output.html#index-option-es) option in the Elasticsearch output. The value that you specify should include the root name of the index plus version and date information. You also need to configure the `setup.template.name` and `setup.template.pattern` options to match the new name. For example:

```yml
output.elasticsearch.index: "rd-%{[agent.version]}-%{+yyyy.MM.dd}"
setup.template.name: "rd"
setup.template.pattern: "rd-*"
```

##    Setup and run

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



###    ES

创建生产证书的文件

```sh
./bin/elasticsearch-certutil ca
```

生成证书文件

```sh

bin/elasticsearch-certutil cert --ca elastic-stack-ca.p12
```

重启ES，

```sh
./bin/elasticsearch
```

生成证书对应秘钥, filebeat used

```sh
openssl pkcs12 -in elastic-stack-ca.p12 -out newfile.crt.pem -clcerts -nokeys
```

把这个文件拷入到filebeat目录中

###    config filebeats

```yml
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
  # for filebeat connect to old version elastic search
  allow_older_versions: true
```

###   启动filebeat

````sh
cd /usr/share/filebeat/bin
sudo filebeat -c /etc/filebeat/filebeat.yml
````

###   自定义filebeat的ES索引名称

```yaml
#==================== Elasticsearch template setting ==========================
setup.ilm.enabled: false
setup.template.name: "filebeat-rd"
setup.template.pattern: "filebeat-rd-*"

#-------------------------- Elasticsearch output ------------------------------
output.elasticsearch:
  # Array of hosts to connect to.
  hosts: ["localhost:9200"]
  index: "filebeat-rd-%{[agent.version]}-%{+yyyy.MM.dd}"

  # Protocol - either `http` (default) or `https`.
  protocol: "https"

  # Authentication credentials - either API key or username/password.
  #api_key: "id:api_key"
  username: "elastic"
  password: "inza42ePLWcTkfxvQykd"
  ssl.verification_mode: none
  ssl.certificate_authorities: ["/etc/filebeat/newfile.crt.pem"]
```
