# prometheus

## setup

该组件用于收集(pull)监控所需的数据。在页面  https://prometheus.io/download/ 下载

```sh
wegt https://github.com/prometheus/prometheus/releases/download/v2.50.1/prometheus-2.50.1.linux-amd64.tar.gz
```

启动

```sh
tar -zxf prometheus-2.50.1.linux-amd64.tar.gz
cd /data/app/prometheus-2.50.1.linux-amd64
./prometheus

```

可以通过以下链接查看数据, 服务默认监听端口为9090

```sh
http://localhost:9090/graph
```

## 修改监听端口

```sh
./prometheus --web.listen-address=:9093
```

## 指定配置文件

```sh
./prometheus --config.file=./prometheus.yml
```



# node exporter

该组件用于收集监控数据。

```sh
wegt https://github.com/prometheus/node_exporter/releases/download/v1.7.0/node_exporter-1.7.0.linux-amd64.tar.gz
tar xvfz node_exporter-*.*-amd64.tar.gz
cd node_exporter-*.*-amd64
./node_exporter
```

# config prometheus

通过配置 `prometheus` 拉取(pull) `node exporter` 采集的数据。

```sh
cd prometheus-2.50.1.linux-amd64
vi prometheus.yml
```

配置信息如下， 每15s(scrape_interval) 拉取一次数据， 采集的目标(scrape_configs -> static_configs -> targets) 为 localhost:9100

```sh
global:
  scrape_interval: 15s

scrape_configs:
- job_name: node
  static_configs:
  - targets: ['localhost:9100']
```

重启 `prometheus` ，

```sh
./prometheus --config.file=./prometheus.yml
```

启动 prometheus 后，可以在prometheus 的页面 http://xxxxx:9090/targets 中看到刚配置的数据接收节点

# grafana

## setup

详见  https://grafana.com/grafana/download，

```sh
wget https://dl.grafana.com/enterprise/release/grafana-enterprise-10.3.3.linux-amd64.tar.gz
tar -zxf grafana-enterprise-10.3.3.linux-amd64.tar.gz
cd grafana-v10.3.3/
# start up
./bin/grafana-server
```

默认端口为3000, 可通过以下 URL 浏览

```sh
http://123.456.1.1:3000
```

输入用户名 (admin) 和密码 (admin) ,登录进入系统，修改密码。

## 配置数据源

可以添加  prometheus 数据源

# Kafka监控

## kafka_exporter

### setup

```sh
wget https://github.com/danielqsj/kafka_exporter/releases/download/v1.2.0/kafka_exporter-1.2.0.linux-amd64.tar.gz
tar -zxf kafka_exporter-1.2.0.linux-amd64.tar.gz
cd kafka_exporter-1.2.0.linux-amd64
nohup ./kafka_exporter --kafka.server=11.10.36.1:9092 &
# 可见 kafka_exporter 监听的是9308 端口
more nohup.out
```

### 查看数据

查看

```sh
http://11.10.36.1:9308/metrics
```

## config prometheus

```sh
scrape_configs:
  - job_name: 'kafka'
    static_configs:
     - targets:
        - 123.456.789.1:9308
```

进入 grafana  官网 https://grafana.com/, 搜索 Kafka Exporter， 下载JSON模板，在 grafana Home->Dashboards, 配置好之后，即可监控kafka的情况。