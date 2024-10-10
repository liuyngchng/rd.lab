# setup

```sh
docker pull emqx/emqx
docker run -d --name emqx -p 4370:4370 -p 1883:1883 -p 8083:8083 -p 8084:8084 -p 8883:8883 -p 18083:18083 emqx/emqx:5.7.1
```

EMQX 默认使用以下端口，请确保这些端口未被其他应用程序占用，并按照需求开放防火墙以保证 EMQX 正常运行。

| 端口  | 协议 | 描述                                                         |
| :---- | :--- | :----------------------------------------------------------- |
| 1883  | TCP  | MQTT over TCP 监听器端口，主要用于未加密的 MQTT 连接。       |
| 8883  | TCP  | MQTT over SSL/TLS 监听器端口，用于加密的 MQTT 连接。         |
| 8083  | TCP  | MQTT over WebSocket 监听器端口，使 MQTT 能通过 WebSocket 进行通信。 |
| 8084  | TCP  | MQTT over WSS (WebSocket over SSL) 监听器端口，提供加密的 WebSocket 连接。 |
| 18083 | HTTP | EMQX Dashboard 和 REST API 端口，用于管理控制台和 API 接口。 |
| 4370  | TCP  | Erlang 分布式传输端口，根据节点名称不同实际端口可能是 `BasePort (4370) + Offset`。 |
| 5370  | TCP  | 集群 RPC 端口（在 Docker 环境下为 5369），根据节点名称不同实际端口可能是 `BasePort (5370) + Offset`。 |

# login dashboard

登录url

```
http://localhost:18083
```



默认用户名 admin, 密码 public，登录后修改密码为自己的密码。

# config

以docker 版的 emqx （emqx/emqx: 5.7.1）为例，配置文件的位置如下所示。

```shell
cat /opt/emqx/etc/emqx.conf
```

# cluster

EMQX 5.0单节点支持500万设备， EMQX 5.0 单集群可支撑1亿并发连接。集群相关的文档详见  https://docs.emqx.com/zh/emqx/latest/deploy/cluster/introduction.html。

集群各个节点间的网络时延应小于10ms, 当节点间网络时延大于100ms,集群将不可用。

# 连接器

## 将数据转发至 HTTP server

开源版本支持HTTP Server 连接器。可以在emqx中配置一个 http server 地址，将符合指定规则的数据自动发送至相应的 http server

（1）配置 HTTP Server。在数据集成(Integration) -> 连接器(Connector) 中添加 Http Server。

（2）配置转发规则。 在数据集成(Integration) -> 规则(Rules) 中添加 规则。在 SQL 中添加 SQL 语句

````sh
# your_topic 为需要转发的 topic, 或者使用默认的通配符。
SELECT
  *
FROM
  "your_topic"
````

至此，数据输入(Data Inputs)已经配置好了。接下来需要配置动作输出(Action Outputs)， 在添加动作处选择 Http Server，在 连接器(Connector) 下拉菜单中选择上一步已配置好的的连接器。可配置 URL 相对路径、Http Method 以及 Http Header，保存即可。

## 数据持久化至 MySQL

开源版本中不支持 MySQL 连接器。详见文档  https://docs.emqx.com/zh/emqx/latest/data-integration/data-bridge-mysql.html。

```sh
# 创建并选择数据库
CREATE DATABASE emqx_data CHARACTER SET utf8mb4;
use emqx_data;
```

我们需要在 MySQL 中创建两张表。

数据表 `emqx_messages` 存储每条消息的发布者客户端 ID、主题、Payload 以及发布时间：

```sql
CREATE TABLE emqx_messages (
    id INT AUTO_INCREMENT PRIMARY KEY,
    clientid VARCHAR(255),
    topic VARCHAR(255),
    payload TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

数据表 `emqx_client_events` 存储上下线的客户端 ID、事件类型以及事件发生时间：

```sql
CREATE TABLE emqx_client_events (
      id INT AUTO_INCREMENT PRIMARY KEY,
      clientid VARCHAR(255),
      event VARCHAR(255),
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

# 开源版与企业版的对比

 EMQX 企业版和开源版的主要功能对比。

|                          | **EMQX 企业版**                                             | **EMQX 开源版**                                             |
| :----------------------- | :---------------------------------------------------------- | :---------------------------------------------------------- |
| **定位**                 | 可靠且可扩展的企业级 MQTT 平台                              | 全球第一的开源 MQTT Broker                                  |
| **伸缩性**               | 支持最多 100 个节点的集群 每个集群最多支持 1 亿个 MQTT 连接 | 支持最多 3 个节点的集群 在生产环境中支持最多 100,000 个连接 |
| **性能**                 | 500 万 MQTT 消息每秒                                        | 10 万 MQTT 消息每秒                                         |
| **可用性**               | 核心-副本集群                                               | 无主集群                                                    |
| **可靠性**               | RocksDB 消息持久化，具备高可用副本                          | 会话持久化                                                  |
| **延迟**                 | 1~5 毫秒                                                    | 1~5 毫秒                                                    |
| **数据集成（开箱即用）** | 40+                                                         | 2                                                           |
| **License**              | Commercial License (Business source license)                | Apache Version 2.0                                          |
| **技术支持**             | 7x24 全球支持                                               | 开源社区                                                    |
