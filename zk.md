# 下载

```sh
wget https://dlcdn.apache.org/zookeeper/zookeeper-3.8.4/apache-zookeeper-3.8.4-bin.tar.gz
tar -zxf apache-zookeeper-3.8.4-bin.tar.gz
```

# doc

https://blog.csdn.net/qq_43700739/article/details/123816688

apache 官方文档详见如下。

https://cwiki.apache.org/confluence/display/ZOOKEEPER/Client-Server+mutual+authentication



# 配置节点数据权限

##  服务端认证文件配置

```sh
cd ./apache-zookeeper-3.8.4-bin/conf/
mv zoo_sample.cfg zoo_sample.cfg.bck
cp zoo_sample.cfg.bck zoo.cfg
vi zoo.cfg
```

添加以下内容

```sh
# 指定认证方法
authProvider.1=org.apache.zookeeper.server.auth.SASLAuthenticationProvider
# 默认为false,需要设置为true, 如果客户端没有通过认证，则关闭session,这个配置很重要
sessionRequireClientSASLAuth=true
requireClientAuthScheme=digest
jaasLoginRenew=3600000
```

创建认证文件

```sh
cd ./apache-zookeeper-3.8.4-bin/conf/
touch jaas.conf
vi jaas.conf
```

内容如下所示

```sh
Server {
  org.apache.zookeeper.server.auth.DigestLoginModule required
  user_admin="mypassword1"
  user_kafka="mypassword2";
};
```

配置了两个用户，用户1 用户名称为 admin， 密码为  mypassword1。 用户2的用户名称为 kafka， 密码为 mypassword2

修改环境变量，让 zk server 启动时启动认证配置

```sh
cd ./apache-zookeeper-3.8.4-bin/bin/
vi zkEnv.sh
```

在 `export SERVER_JVMFLAGS` 前面添加 `SERVER_JVMFLAGS="${SERVER_JVMFLAGS} -Djava.security.auth.login.config=/home/rd/software/docker_build/apache-zookeeper-3.8.4-bin/conf/jaas.conf "`， 如下所示

```sh
# default heap for zookeeper server
ZK_SERVER_HEAP="${ZK_SERVER_HEAP:-1000}"
SERVER_JVMFLAGS="${SERVER_JVMFLAGS} -Djava.security.auth.login.config=/home/rd/software/docker_build/apache-zookeeper-3.8.4-bin/conf/jaas.conf "
export SERVER_JVMFLAGS="-Xmx${ZK_SERVER_HEAP}m $SERVER_JVMFLAGS"
```

启动 zk server

```sh
cd ./apache-zookeeper-3.8.4-bin/bin/
./zkServer.sh start
```

查看配置是否生效

```sh
ps -ef | grep zookeeper | grep jaas.conf
```

可见 -D 参数已经生效。此时使用client连接 zookeeper server

```sh
cd ./apache-zookeeper-3.8.4-bin/bin/
./zkCli.sh
ls /
```

会看到客户端报错

```java
2024-05-16 16:24:20,249 [myid:localhost:2181] - WARN  [main-SendThread(localhost:2181):o.a.z.ClientCnxn$SendThread@1300] - Session 0x100351db2050000 for server localhost/127.0.0.1:2181, Closing socket connection. Attempting reconnect except it is a SessionExpiredException.
org.apache.zookeeper.ClientCnxn$EndOfStreamException: Unable to read additional data from server sessionid 0x100351db2050000, likely server has closed socket
	at org.apache.zookeeper.ClientCnxnSocketNIO.doIO(ClientCnxnSocketNIO.java:77)
	at org.apache.zookeeper.ClientCnxnSocketNIO.doTransport(ClientCnxnSocketNIO.java:350)
	at org.apache.zookeeper.ClientCnxn$SendThread.run(ClientCnxn.java:1289)

```

若注释掉 zoo.cfg 中的

```sh
# sessionRequireClientSASLAuth=true
```

重新启动zookeeper server，通过客户端连接，执行 `ls /`可正常查看数据。

##  客户端认证文件配置

创建客户端认证文件

```sh
cd ./apache-zookeeper-3.8.4-bin/conf/
touch client_jaas.conf
vi client_jaas.conf
# 内容如下所示
Client {
    org.apache.zookeeper.server.auth.DigestLoginModule required
    username="admin"
    password="youradminpassword";
};
```

此处的用户名和密码 需要和之前server的配置文件 jaas.conf 对应起来，否则认证无法通过

修改环境变量

```sh
cd ./apache-zookeeper-3.8.4-bin/bin/
vi zkEnv.sh
```

在 `export CLIENT_JVMFLAGS="-Xmx${ZK_CLIENT_HEAP}m $CLIENT_JVMFLAGS"` 前面添加 `CLIENT_JVMFLAGS="${CLIENT_JVMFLAGS} -Djava.security.auth.login.config=/home/rd/software/docker_build/apache-zookeeper-3.8.4-bin/conf/client_jaas.conf"`

如下所示

```sh
cd ./apache-zookeeper-3.8.4-bin/bin/
cat zkEnv.sh
# default heap for zookeeper client
ZK_CLIENT_HEAP="${ZK_CLIENT_HEAP:-256}"
CLIENT_JVMFLAGS="${CLIENT_JVMFLAGS} -Djava.security.auth.login.config=/home/rd/software/docker_build/apache-zookeeper-3.8.4-bin/conf/client_jaas.conf "
export CLIENT_JVMFLAGS="-Xmx${ZK_CLIENT_HEAP}m $CLIENT_JVMFLAGS"
```

启动客户端

```sh
cd ./apache-zookeeper-3.8.4-bin/bin/
./zkCli.sh
ls /
```

此时，客户端连接提交了正确的用户名和密码， 连接已正常
