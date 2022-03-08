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



# 3. journal
To tail the journal:

`sudo journalctl -f`

# 4. healthy check
sudo curl --cacert /etc/elasticsearch/certs/http_ca.crt  'https://127.0.0.1:9200/_cluster/health?pretty' -u elastic

# 5. reset password

`bin/elasticsearch-reset-password -u elastic`

