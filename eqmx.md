# setup

```sh
docker pull emqx/emqx
docker run -d --name emqx -p 1883:1883 -p 8083:8083 -p 8084:8084 -p 8883:8883 -p 18083:18083 emqx/emqx:5.7.1
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



默认用户名 admin, 密码 public，登录后修改密码



