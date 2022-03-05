# 1. healthy check
sudo curl --cacert /etc/elasticsearch/certs/http_ca.crt  'https://127.0.0.1:9200/_cluster/health?pretty' -u elastic
# 2. reset password

`bin/elasticsearch-reset-password -u elastic`

# 3. run Elasticsearch as a daemon
`./bin/elasticsearch -d -p pid`
