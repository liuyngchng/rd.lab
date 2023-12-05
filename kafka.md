# 1. context

## 1.1 logic

* 逻辑关系

  ```sh
  	kafka -> topic -> partition(分片) -> replica -> isr -> leader, 最终负载落在leader所在的机器上。leader负责读写请求的处理。
  
  	     		|
  
  		     	|topic1 -+ partition1（逻辑）- replica1（逻辑）
  
  			      			   |							    |- replica2 （逻辑）
  
  				       		  + partition2（逻辑）
  ```

* 负载（物理）关系

  ```shell
  consumer -> broker -> zk -> topic-> partition -> leader -> server
  ```

## 1.2 partition

 * 分区消息数量

   分区内存储的消息是整个topic下消息的子集。topic消息=partition1消息+ partition2消息 + ... + partitionN消息

 * 分区职责

   分区或者叫做分片，提升系统的并发处理能力，一个topic的多个partition 提升多个consumer消费消息时系统的吞吐量。

 * 逻辑分片

   分区是对集合的逻辑分片，与存储位置不直接产生关系

   

## 1.3 replica

 * replica 集合关系

   分区中的所有副本称为AR（Assigned Replicas）。所有与leader副本保持一定程度同步的副本（包括leader）组成ISR（in-sync replicas）。而与leader副本同步滞后过多的副本（不包括leader），组成OSR（out-sync replicas），所以，AR = ISR + OSR。在正常情况下，所有的follower副本都应该与leader副本保持一定程度的同步，即AR = ISR，OSR集合为空。

 * replica 职责

   提升系统的冗余度，保障系统的容错性和高可用性

## 1.4 group

* consumer 在不建group的情况下，各自消费各自的，独立使用各自的offset
* consumer 加入group的情况下，所有consumer作为一个整体，使用同一个offset 消费topic下的多个partition，所有的consumer处于一个队列中，有序消费消息，同时又不丧失并发性

## 1.5 ENV

本文中所有脚本在 `Ubuntu 20.04 `， `kafka_2.11-2.4.1`， `openjdk version "17.0.2"`环境下测试通过。

# 2. setup

进入  https://kafka.apache.org/downloads.html， 手动下载，或者执行

```
wget https://archive.apache.org/dist/kafka/2.4.1/kafka_2.11-2.4.1.tgz
tar -zxf kafka_2.11-2.4.1.tgz
```



## 2.1 zk

启动

```sh
./bin/zookeeper-server-start.sh -daemon config/zookeeper.properties
sudo netstat -anpl | grep 2181
```

停止

```sh
./bin/zookeeper-server-stop.sh
```

# Consumer

##  可获取的最大记录数

max.poll.records

```xml

The maximum number of records returned in a single call to poll(). Note, that max.poll.records does not impact the underlying fetching behavior. The consumer will cache the records from each fetch request and returns them incrementally from each poll.

Type:	int
Default:	500
Valid Values:	[1,...]
Importance:	medium
```

## 间隔时间

max.poll.interval.ms

```
The maximum delay between invocations of poll() when using consumer group management. This places an upper bound on the amount of time that the consumer can be idle before fetching more records. If poll() is not called before expiration of this timeout, then the consumer is considered failed and the group will rebalance in order to reassign the partitions to another member. For consumers using a non-null group.instance.id which reach this timeout, partitions will not be immediately reassigned. Instead, the consumer will stop sending heartbeats and partitions will be reassigned after expiration of session.timeout.ms. This mirrors the behavior of a static consumer which has shutdown.

Type:	int
Default:	300000 (5 minutes)
Valid Values:	[1,...]
Importance:	medium
```

## 最大字节数

fetch.max.bytes

```
The maximum amount of data the server should return for a fetch request. Records are fetched in batches by the consumer, and if the first record batch in the first non-empty partition of the fetch is larger than this value, the record batch will still be returned to ensure that the consumer can make progress. As such, this is not a absolute maximum. The maximum record batch size accepted by the broker is defined via message.max.bytes (broker config) or max.message.bytes (topic config). Note that the consumer performs multiple fetches in parallel.

Type:	int
Default:	52428800 (50 mebibytes)
Valid Values:	[0,...]
Importance:	medium
```

## 消息偏移量

auto.offset.reset

```
What to do when there is no initial offset in Kafka or if the current offset does not exist any more on the server (e.g. because that data has been deleted):

earliest: automatically reset the offset to the earliest offset
latest: automatically reset the offset to the latest offset
none: throw exception to the consumer if no previous offset is found for the consumer's group
anything else: throw exception to the consumer.
Type:	string
Default:	latest
Valid Values:	[latest, earliest, none]
Importance:	medium
```

## 隔离级别

isolation.level

```xml
Controls how to read messages written transactionally. If set to read_committed, consumer.poll() will only return transactional messages which have been committed. If set to read_uncommitted (the default), consumer.poll() will return all messages, even transactional messages which have been aborted. Non-transactional messages will be returned unconditionally in either mode.

Messages will always be returned in offset order. Hence, in read_committed mode, consumer.poll() will only return messages up to the last stable offset (LSO), which is the one less than the offset of the first open transaction. In particular any messages appearing after messages belonging to ongoing transactions will be withheld until the relevant transaction has been completed. As a result, read_committed consumers will not be able to read up to the high watermark when there are in flight transactions.

Further, when in read_committed the seekToEnd method will return the LSO

Type:	string
Default:	read_uncommitted
Valid Values:	[read_committed, read_uncommitted]
Importance:	medium
```

## 自动提交

enable.auto.commit

```xml
If true the consumer's offset will be periodically committed in the background.

Type:	boolean
Default:	true
Valid Values:	
Importance:	medium
```



# kafka server

config
```sh
cd config
cp server.properties server1.properties
vi server1.properties
broker.id=0
log.dirs=/tmp/kafka1-logs
port=9092
cp server.properties server2.properties
vi server2.properties
broker.id=1
log.dirs=/tmp/kafka2-logs
port=9093
cp server.properties server3.properties
broker.id=2
log.dirs=/tmp/kafka3-logs
port=9094
```
startup
```sh
./bin/kafka-server-start.sh -daemon config/server1.properties
./bin/kafka-server-start.sh -daemon config/server2.properties
./bin/kafka-server-start.sh -daemon config/server3.properties
```

stop

```sh
./bin/kafka-server-stop.sh config/server1.properties
```

# 3. op



## 3.1 创建 topic

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --create  --partitions 3 --replication-factor 3  --topic test_me 
```

## 3.2 查看topic

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --list
# you can see 
test_me
./bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test_me
# you can see
Topic: test_me	PartitionCount: 3	ReplicationFactor: 3	Configs: 
	Topic: test_me	Partition: 0	Leader: 0	Replicas: 2,0,1	Isr: 0
	Topic: test_me	Partition: 1	Leader: 0	Replicas: 0,1,2	Isr: 0
	Topic: test_me	Partition: 2	Leader: 0	Replicas: 1,2,0	Isr: 0
```



## 3.3 删除 topic

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --delete --topic test_me
```

删除后， 会显示

```sh
Topic test_me is marked for deletion.
Note: This will have no impact if delete.topic.enable is not set to true
```

设置彻底删除

```sh
vi config/server.properties
# 添加
delete.topic.enable=true
```

如果kafka启动之前没有配置delete.topic.enable=true，topic只会标记为marked for deletion，加上配置，重启kafka，之前的topick就真正删除了

## 3.4 发送msg

* snd 从第1,2 个broker发送

```sh
./bin/kafka-console-producer.sh --broker-list localhost:9092,localhost:9093  --topic test_me --producer.config config/producer.properties
```
* rcv 从第3个broker接收

```sh
./bin/kafka-console-consumer.sh --bootstrap-server localhost:9094 --topic test_me --from-beginning --consumer.config config/consumer.properties
```

## 3.5 获取topic list



```
./bin/kafka-topics.sh --bootstrap-server localhost:9093 --list
```

## 3.6 压测

```sh
bin/kafka-producer-perf-test.sh --topic test --num-records 100 --record-size 1 --throughput 100  --producer-props bootstrap.servers=localhost:9092
```

## 3.7 数据导入导出

* 导入

```sh
./bin/kafka-console-producer.sh --broker-list localhost:9092,localhost:9093,localhost:9094 --topic test_me < /tmp/test_me.txt &
```

* 导出

```shell
./bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test_me --from-beginning > /tmp/test_me.txt &
```

## 3.8 查看ZK中注册的 brokers

```sh
./bin/zookeeper-shell.sh localhost:2181
ls /brokers/ids
```

## 3.9 leader election

```sh
# server增加了权限验证后 执行timeout， 未成功，不设置权限验证执行形成
./bin/kafka-leader-election.sh --bootstrap-server localhost:9092 --topic test_me --election-type PREFERRED  --partition 1
```

## 3.10 查看消息总量

查看指定topic中的消息总量

```sh
./bin/kafka-run-class.sh kafka.tools.GetOffsetShell --broker-list localhost:9092 --topic test_me --time -1
test_me:0:11
test_me:1:5
test_me:2:4
test_me:3:3
test_me:4:0
```

## 3.11 查看消息消费进度

* 版本小于 0.9.0.0

```sh
# 查看消费进度
./bin/kafka-run-class.sh kafka.tools.ConsumerOffsetChecker
```

* 版本大于 0.9.0.0

```sh
# 获取所有的消费者组清单
./bin/kafka-consumer-groups.sh --bootstrap-server  localhost:9092 --list
console-consumer-85822
# 获取所有的消费者组清单
./bin/kafka-consumer-groups.sh --bootstrap-server  localhost:9092 --describe --group console-consumer-85822

```

可以看到

```sh
GROUP                  TOPIC           PARTITION  CURRENT-OFFSET  LOG-END-OFFSET  LAG             CONSUMER-ID                                                            HOST            CLIENT-ID
console-consumer-85822 test_me         3          -               3               -               consumer-console-consumer-85822-1-2ec73682-e0a5-452d-9150-9576b0918905 /127.0.0.1      consumer-console-consumer-85822-1
console-consumer-85822 test_me         4          -               0               -               consumer-console-consumer-85822-1-2ec73682-e0a5-452d-9150-9576b0918905 /127.0.0.1      consumer-console-consumer-85822-1
console-consumer-85822 test_me         2          -               4               -               consumer-console-consumer-85822-1-2ec73682-e0a5-452d-9150-9576b0918905 /127.0.0.1      consumer-console-consumer-85822-1
console-consumer-85822 test_me         0          -               11              -               consumer-console-consumer-85822-1-2ec73682-e0a5-452d-9150-9576b0918905 /127.0.0.1      consumer-console-consumer-85822-1
console-consumer-85822 test_me         1          -               5               -               consumer-console-consumer-85822-1-2ec73682-e0a5-452d-9150-9576b0918905 /127.0.0.1      consumer-console-consumer-85822-1

```

LOG-END-OFFSET为各个partition中的消息数量，求和后的总数即为某个topic下的消息数量

## 3.12 查看版本

```
./bin/kafka-topics.sh --version
2.4.1 (Commit:c57222ae8cd7866b)
```



# 4. 扩容

## 4.1 吞吐量提升方案	

​        通过扩容，提升系统的整体吞吐量。

（1）启动新的kafa实例加入集群。后续创建的topic，设置partition数量为实例数量，起到提升并发量进而提升吞吐量作用。

（2）对于已有的topic， 增加topic分区（partition）数量，同时，需要将分担各个partition流量的 leader移动一部分到新启动的kafka实例。

（3）由于leader是从ISR选举产生的，而ISR是从完成消息同步的replica中产生的。所以需要将已有的replica迁移一部分到新启动的kafka实例。

（4）对于数据量较大的topic，迁移replica，需要一定的时间，需要进行测试评估。

## 4.2 系统稳定性提升方案

​       通过扩容，提升系统的稳定性，即当部分实例（服务器）失效后，系统依然可以正常工作。

（1）修改现有topic各个partition的副本数量。

（2）启动新增副本数据同步计划。

（3）对于数据量较大的topic，生成新的达到ISR要求的replica，需要一定的时间，需要进行测试评估。

# 5. 分区扩容

## 5.1 新增分区

* kafka版本 < 2.2

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --alter --topic test_me --partitions 2
```
* kafka版本 >= 2.2

```sh
./bin/kafka-topics.sh --bootstrap-server localhost:9092 --alter --topic test_me --partitions 5
```
## 5.2 迁移副本

### 5.2.1 查看目前的负载现状

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test_me

Topic: test_me	PartitionCount: 3	ReplicationFactor: 3	Configs: 
	Topic: test_me	Partition: 0	Leader: 1	Replicas: 1,2,0	Isr: 1,2,0
	Topic: test_me	Partition: 1	Leader: 2	Replicas: 2,0,1	Isr: 2,0,1
	Topic: test_me	Partition: 2	Leader: 0	Replicas: 0,1,2	Isr: 0,1,2
```

### 5.2.2  启动新的实例

* 修改配置

```shell
  cp _server.properties server4.properties
  vi server4.properties
  broker.id=3
  log.dirs=/tmp/kafka4-logs
  port=9095
  cp _server.properties server5.properties
  vi server5.properties
  broker.id=4
  log.dirs=/tmp/kafka5-logs
  port=9096
```

* 启动服务

```shell
  ./bin/kafka-server-start.sh -daemon config/server4.properties
  ./bin/kafka-server-start.sh -daemon config/server5.properties
  sudo netstat -anpl | grep 9095						# 确认服务启动
  sudo netstat -anpl | grep 9096						# 确认服务启动
```

* 创建规则topic.json

```sh
cat > topic.json <<EOF
{
	"version":1,
	"topics":[
		{"topic":"test_me"}
	]
}
EOF
```

###  5.2.3  生成reassignment执行计划
```sh
./bin/kafka-reassign-partitions.sh --zookeeper localhost:2181  --topics-to-move-json-file /tmp/topic.json  --broker-list "0,1,2,3,4"  --generate
Current partition replica assignment
{
	"version": 1,
	"partitions": [{
		"topic": "test_me",
		"partition": 2,
		"replicas": [0, 1, 2],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 1,
		"replicas": [2, 0, 1],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 0,
		"replicas": [1, 2, 0],
		"log_dirs": ["any", "any", "any"]
	}]
}

Proposed partition reassignment configuration

{
	"version": 1,
	"partitions": [{
		"topic": "test_me",
		"partition": 3,
		"replicas": [0, 1, 2],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 0,
		"replicas": [2, 3, 4],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 2,
		"replicas": [4, 0, 1],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 4,
		"replicas": [1, 2, 3],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 1,
		"replicas": [3, 4, 0],
		"log_dirs": ["any", "any", "any"]
	}]
}

```

### 5.2.4 创建规则reassignment.json

```sh
cat > reassignment.json <<EOF
{
	"version": 1,
	"partitions": [{
		"topic": "test_me",
		"partition": 3,
		"replicas": [0, 1, 2],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 0,
		"replicas": [2, 3, 4],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 2,
		"replicas": [4, 0, 1],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 4,
		"replicas": [1, 2, 3],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 1,
		"replicas": [3, 4, 0],
		"log_dirs": ["any", "any", "any"]
	}]
}
EOF
```

### 5.2.5 执行

这个过程对于数据量较大时耗时较长，通过--throttle 参数(100000000, 100Mbps)限制对网络的占用，单位byte/s

```sh
./bin/kafka-reassign-partitions.sh --zookeeper localhost:2181  --reassignment-json-file /tmp/reassignment.json --throttle  100000000 --execute
Current partition replica assignment

{"version":1,"partitions":[{"topic":"test_me","partition":3,"replicas":[2,3,4],"log_dirs":["any","any","any"]},{"topic":"test_me","partition":4,"replicas":[3,4,0],"log_dirs":["any","any","any"]},{"topic":"test_me","partition":2,"replicas":[1,0,2],"log_dirs":["any","any","any"]},{"topic":"test_me","partition":0,"replicas":[4,3,0],"log_dirs":["any","any","any"]},{"topic":"test_me","partition":1,"replicas":[0,4,1],"log_dirs":["any","any","any"]}]}
```

### 5.2.6 检查进度（验证）

```sh
./bin/kafka-reassign-partitions.sh --zookeeper localhost:2181  --reassignment-json-file /tmp/reassignment.json  --verify
Status of partition reassignment: 
Reassignment of partition test_me-3 completed successfully
Reassignment of partition test_me-4 completed successfully
Reassignment of partition test_me-2 completed successfully
Reassignment of partition test_me-0 completed successfully
Reassignment of partition test_me-1 completed successfully
```



### 5.2.7 查看topic的分区及负载情况

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test_me
Topic: test_me	PartitionCount: 5	ReplicationFactor: 3	Configs: 
	Topic: test_me	Partition: 0	Leader: 4	Replicas: 2,3,4	Isr: 3,4,2
	Topic: test_me	Partition: 1	Leader: 3	Replicas: 3,4,0	Isr: 4,3,0
	Topic: test_me	Partition: 2	Leader: 4	Replicas: 4,0,1	Isr: 4,1,0
	Topic: test_me	Partition: 3	Leader: 0	Replicas: 0,1,2	Isr: 0,2,1
	Topic: test_me	Partition: 4	Leader: 3	Replicas: 1,2,3	Isr: 3,1,2
```


# 6.  扩充副本

## 6.1  创建规则json
注意，此处replica中副本节点的顺序会影响到leader的选举

```shell
cat > increase-replication-factor.json <<EOF
{"version":1, "partitions":[
{"topic":"test_me","partition":0,"replicas":[0,1]},
{"topic":"test_me","partition":1,"replicas":[0,1]},
{"topic":"test_me","partition":2,"replicas":[0,1]},
{"topic":"test_me","partition":3,"replicas":[0,1]},
{"topic":"test_me","partition":4,"replicas":[0,1]}]
}
EOF
```

## 6.2 执行

```sh
./bin/kafka-reassign-partitions.sh --zookeeper localhost:2181 --reassignment-json-file increase-replication-factor.json -- execute
```

## 6.3 验证

```sh
./bin/kafka-reassign-partitions.sh --zookeeper localhost:2181 --reassignment-json-file increase-replication-factor.json --verify
```

# 7. 设置JAAS验证

## 7.1 增加用户密码配置文件

在config下面新建kafka_client_jaas.conf文件内容如下

```sh
KafkaClient {  
	org.apache.kafka.common.security.plain.PlainLoginModule required  
    	username="admin"  
    	password="admin";  
};
```

在config下面新建kafka_server_jaas.conf文件内容如下

```sh
KafkaServer {
    org.apache.kafka.common.security.plain.PlainLoginModule required
        username="admin"
        password="password"
        user_admin="password";
};
```

## 7.2 修改 server 启动脚本

vim ./bin/kafka-server-start.sh

```sh
# 在 base_dir= 之后添加，使用现有的环境变量
export KAFKA_OPTS="-Djava.security.auth.login.config=$base_dir/../config/kafka_server_jaas.conf"
```

vim ./bin/kafka-console-producer.sh 

vim ./bin/kafka-console-consumer.sh

```sh
# 定义 base_dir
base_dir=$(dirname $0)
export KAFKA_OPTS="-Djava.security.auth.login.config=$base_dir/../config/kafka_client_jaas.conf"
```

## 7.3  修改 server 配置文件

vim ./config/server.properties

```sh
# 必须使用真实IP地址
listeners=SASL_PLAINTEXT://192.168.1.110:9092

#使用的认证协议
security.inter.broker.protocol=SASL_PLAINTEXT
#SASL机制
sasl.enabled.mechanisms=PLAIN
sasl.mechanism.inter.broker.protocol=PLAIN
#完成身份验证的类
authorizer.class.name=kafka.security.auth.SimpleAclAuthorizer
#如果没有找到ACL（访问控制列表）配置，则允许任何操作。
#allow.everyone.if.no.acl.found=true
#需要开启设置超级管理员, 开启以下命令
super.users=User:admin
```

## 7.4 修改 producer 和 consumer 配置文件

vim ./config/consumer.properties

vim ./config/producer.properties

```sh
# 添加命令
security.protocol=SASL_PLAINTEXT
sasl.mechanism=PLAIN
```



#  8. 扩容测试

## 8.1 扩容计划

broker清单如下所示

| No   | brokerId | bootstrap-server | dataDir          | 实例状态 |
| ---- | -------- | ---------------- | ---------------- | -------- |
| 1    | 0        | localhost:9092   | /tmp/kafka1-logs | 现有     |
| 2    | 1        | localhost:9093   | /tmp/kafka2-logs | 现有     |
| 3    | 2        | localhost:9094   | /tmp/kafka3-logs | 现有     |
| 4    | 3        | localhost:9095   | /tmp/kafka4-logs | **扩容** |
| 5    | 4        | localhost:9096   | /tmp/kafka5-logs | **扩容** |



## 8.2 测试 producer 和 consumer 使用不同的 borker list

从broker AB发送， 从broker C接收

partition如下所示

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test_me
Topic: test_me	PartitionCount: 3	ReplicationFactor: 3	Configs: 
	Topic: test_me	Partition: 0	Leader: 2	Replicas: 2,0,1	Isr: 2,0,1
	Topic: test_me	Partition: 1	Leader: 0	Replicas: 0,1,2	Isr: 0,1,2
	Topic: test_me	Partition: 2	Leader: 1	Replicas: 1,2,0	Isr: 1,2,0
```

zk中的broker节点
```sh
./bin/zookeeper-shell.sh localhost:2181
Connecting to localhost:2181
Welcome to ZooKeeper!
JLine support is disabled
WATCHER::
WatchedEvent state:SyncConnected type:None path:null
ls /brokers/ids
[0, 1, 2]
```



* snd 从第1,2 个broker发送msg

```sh
./bin/kafka-console-producer.sh --broker-list localhost:9092,localhost:9093  --topic test_me 
>msg1
>msg2
>msg3
>msg4
>msg5
>msg6
>msg7
>msg8
>
```

* rcv 从第3个broker接收msg

```sh
./bin/kafka-console-consumer.sh --bootstrap-server localhost:9094 --topic test_me --from-beginning
msg1
msg2
msg3
msg4
msg5
msg6
msg7
msg8
^CProcessed a total of 8 messages
```

各个 partition 的 msg 如下所示:

```sh
rd@rd-ubt:/tmp/kafka3-logs$ cat test_me-0/00000000000000000000.log 
<**********msg1***********msg3*************msg7
rd@rd-ubt:/tmp/kafka3-logs$ 
rd@rd-ubt:/tmp/kafka3-logs$ cat test_me-1/00000000000000000000.log 
<*********msg5<**************msg8
rd@rd-ubt:/tmp/kafka3-logs$ cat test_me-2/00000000000000000000.log 
<**********msg2*******msg4****************msg6
rd@rd-ubt:/tmp/kafka3-logs$ pwd
/tmp/kafka3-logs
rd@rd-ubt:/tmp/kafka3-logs$
```



## 8.3 启动新的实例

在8.1节已有的3个节点基础上，新启动2个实例

```sh
./bin/kafka-server-start.sh -daemon config/server4.properties
./bin/kafka-server-start.sh -daemon config/server5.properties
```

ZK中已有5个 broker 节点

```sh
./bin/zookeeper-shell.sh localhost:2181
Connecting to localhost:2181
Welcome to ZooKeeper!
JLine support is disabled

WATCHER::

WatchedEvent state:SyncConnected type:None path:null
ls /brokers/ids
[0, 1, 2, 3, 4]
```

## 8.4 启动新实例对现有系统的影响

收发消息，发送

```sh
rd@rd-ubt:~/software/kafka_2.11-2.4.1$ ./bin/kafka-console-producer.sh --broker-list localhost:9092,localhost:9093  --topic test_me 
>msg_snd_after_2_broker_added    
>msg1_snd_after_2_broker_added
>
```

接收

```sh
d@rd-ubt:~/software/kafka_2.11-2.4.1$ ./bin/kafka-console-consumer.sh --bootstrap-server localhost:9094 --topic test_me --from-beginning 
msg2
msg4
msg6
msg1
msg3
msg7
msg5
msg8


msg_snd_after_2_broker_added
msg1_snd_after_2_broker_added
```

查看 partition，启动新实例，zk已有 broker， 尚未分配 partition

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test_me
Topic: test_me	PartitionCount: 3	ReplicationFactor: 3	Configs: 
	Topic: test_me	Partition: 0	Leader: 2	Replicas: 2,0,1	Isr: 2,0,1
	Topic: test_me	Partition: 1	Leader: 0	Replicas: 0,1,2	Isr: 0,1,2
	Topic: test_me	Partition: 2	Leader: 1	Replicas: 1,2,0	Isr: 1,2,0
```

## 8.5 扩容 partition

在8.2节基础上， 新增2个分区，replica自动迁移，但leader中没有新增节点。由于消息量较少，新节点3, 4 很快加入到了isr

```sh
./bin/kafka-topics.sh --bootstrap-server localhost:9092 --alter --topic test_me --partitions 5
rd@rd-ubt:~/software/kafka_2.11-2.4.1$ ./bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test_me
Topic: test_me	PartitionCount: 5	ReplicationFactor: 3	Configs: 
	Topic: test_me	Partition: 0	Leader: 2	Replicas: 2,0,1	Isr: 2,0,1
	Topic: test_me	Partition: 1	Leader: 0	Replicas: 0,1,2	Isr: 0,1,2
	Topic: test_me	Partition: 2	Leader: 1	Replicas: 1,2,0	Isr: 1,2,0
	Topic: test_me	Partition: 3	Leader: 0	Replicas: 0,3,4	Isr: 0,3,4
	Topic: test_me	Partition: 4	Leader: 1	Replicas: 1,4,0	Isr: 1,4,0
```

原来节点的数据目录依然是3个分区

```sh
rd@rd-ubt:/tmp/kafka3-logs$ pwd
/tmp/kafka3-logs
rd@rd-ubt:/tmp/kafka3-logs$ ls -al
drwxrwxr-x  2 rd   rd    4096 4月  13 14:49 test_me-0
drwxrwxr-x  2 rd   rd    4096 4月  13 14:55 test_me-1
drwxrwxr-x  2 rd   rd    4096 4月  13 14:55 test_me-2
```

## 8.6 扩容 partition 对现有系统的影响

在生产者没有生产新消息的情况下， 原来的consumer收到了重复消息

```sh
rd@rd-ubt:~/software/kafka_2.11-2.4.1$ ./bin/kafka-console-consumer.sh --bootstrap-server localhost:9094 --topic test_me --from-beginning 
msg2
msg4
msg6
msg1
msg3
msg7
msg5
msg8


msg_snd_after_2_broker_added
msg1_snd_after_2_broker_added
msg1
msg3
msg7
msg_snd_after_2_broker_added
msg5
msg8
msg1_snd_after_2_broker_added
msg2
msg4
msg6
```



按照8.2节的配置，开始生产数据

```sh
./bin/kafka-console-producer.sh --broker-list localhost:9092,localhost:9093  --topic test_me
>msg1_after_2_new_partition
>msg2_after_2_new_partition
>msg3_after_2_new_partition
>msg4_after_2_new_partition
>msg5_after_2_new_partition
>msg6_after_2_new_partition
>msg7-after_2_new_partition
>

```

consumer 收到了新消息

```sh
 ./bin/kafka-console-consumer.sh --bootstrap-server localhost:9094 --topic test_me --from-beginning 
msg2
msg4
msg6
msg1
msg3
msg7
msg5
msg8


msg_snd_after_2_broker_added
msg1_snd_after_2_broker_added
msg1
msg3
msg7
msg_snd_after_2_broker_added
msg5
msg8
msg1_snd_after_2_broker_added
msg2
msg4
msg6



msg1_after_2_new_partition
msg2_after_2_new_partition
msg3_after_2_new_partition
msg4_after_2_new_partition
msg5_after_2_new_partition
msg6_after_2_new_partition
msg7-after_2_new_partition
```

各个节点数据目录如下所示：

```sh
ls -al kafka1-logs/
drwxrwxr-x  2 rd   rd    4096 4月  13 14:55 test_me-0
drwxrwxr-x  2 rd   rd    4096 4月  13 14:49 test_me-1
drwxrwxr-x  2 rd   rd    4096 4月  13 14:55 test_me-2
drwxrwxr-x  2 rd   rd    4096 4月  13 15:20 test_me-3
drwxrwxr-x  2 rd   rd    4096 4月  13 15:20 test_me-4

ls -al kafka2-logs/
drwxrwxr-x  2 rd   rd    4096 4月  13 14:55 test_me-0
drwxrwxr-x  2 rd   rd    4096 4月  13 14:55 test_me-1
drwxrwxr-x  2 rd   rd    4096 4月  13 14:49 test_me-2
drwxrwxr-x  2 rd   rd    4096 4月  13 15:20 test_me-4

ls -al kafka3-logs/
drwxrwxr-x  2 rd   rd    4096 4月  13 14:49 test_me-0
drwxrwxr-x  2 rd   rd    4096 4月  13 14:55 test_me-1
drwxrwxr-x  2 rd   rd    4096 4月  13 14:55 test_me-2

ls -al kafka4-logs/
drwxrwxr-x  2 rd   rd    4096 4月  13 15:29 test_me-3

ls -al kafka5-logs/
drwxrwxr-x  2 rd   rd    4096 4月  13 15:29 test_me-3
drwxrwxr-x  2 rd   rd    4096 4月  13 15:20 test_me-4
```

## 8.7 从新增节点收消息

从新节点收取消息

```sh
./bin/kafka-console-consumer.sh --bootstrap-server localhost:9096 --topic test_me --from-beginning 
msg1
msg3
msg7
msg_snd_after_2_broker_added
msg2_after_2_new_partition
msg5_after_2_new_partition
msg2
msg4
msg6
msg5
msg8
msg1_snd_after_2_broker_added
msg4_after_2_new_partition
msg6_after_2_new_partition
msg1_after_2_new_partition
msg3_after_2_new_partition
msg7-after_2_new_partition
```



## 8.8 迁移副本

### 8.8.1 确定需要迁移的 topic

创建需要迁移的topic文件topic.json

```sh
cat > topic.json <<EOF
{
	"version":1,
	"topics":[
		{"topic":"test_me"}
	]
}
EOF
```

 生成迁移计划

```sh
./bin/kafka-reassign-partitions.sh --zookeeper localhost:2181  --topics-to-move-json-file ./topic.json  --broker-list "0,1,2,3,4"  --generate
Current partition replica assignment
{
	"version": 1,
	"partitions": [{
		"topic": "test_me",
		"partition": 3,
		"replicas": [0, 3, 4],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 4,
		"replicas": [1, 4, 0],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 2,
		"replicas": [1, 2, 0],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 0,
		"replicas": [2, 0, 1],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 1,
		"replicas": [0, 1, 2],
		"log_dirs": ["any", "any", "any"]
	}]
}

Proposed partition reassignment configuration
{
	"version": 1,
	"partitions": [{
		"topic": "test_me",
		"partition": 3,
		"replicas": [4, 3, 0],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 0,
		"replicas": [1, 0, 2],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 2,
		"replicas": [3, 2, 4],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 4,
		"replicas": [0, 4, 1],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 1,
		"replicas": [2, 1, 3],
		"log_dirs": ["any", "any", "any"]
	}]
}
```

### 8.8.2  生成迁移计划文件

```sh
cat > reassignment.json <<EOF
{
	"version": 1,
	"partitions": [{
		"topic": "test_me",
		"partition": 3,
		"replicas": [4, 3, 0],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 0,
		"replicas": [1, 0, 2],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 2,
		"replicas": [3, 2, 4],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 4,
		"replicas": [0, 4, 1],
		"log_dirs": ["any", "any", "any"]
	}, {
		"topic": "test_me",
		"partition": 1,
		"replicas": [2, 1, 3],
		"log_dirs": ["any", "any", "any"]
	}]
}
EOF
```

### 8.8.3 执行迁移

记录迁移前副本情况

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test_me
Topic: test_me	PartitionCount: 5	ReplicationFactor: 3	Configs: 
	Topic: test_me	Partition: 0	Leader: 2	Replicas: 2,0,1	Isr: 2,0,1
	Topic: test_me	Partition: 1	Leader: 0	Replicas: 0,1,2	Isr: 0,1,2
	Topic: test_me	Partition: 2	Leader: 1	Replicas: 1,2,0	Isr: 1,2,0
	Topic: test_me	Partition: 3	Leader: 0	Replicas: 0,3,4	Isr: 0,3,4
	Topic: test_me	Partition: 4	Leader: 1	Replicas: 1,4,0	Isr: 1,4,0
```

执行迁移

```sh
# 执行计划，限制带宽占用不超过100Mbps，耗时任务
./bin/kafka-reassign-partitions.sh --zookeeper localhost:2181  --reassignment-json-file ./reassignment.json --throttle  100000000  --execute
# 查看进度
./bin/kafka-reassign-partitions.sh --zookeeper localhost:2181  --reassignment-json-file ./reassignment.json  --verify
Status of partition reassignment: 
Reassignment of partition test_me-3 completed successfully
Reassignment of partition test_me-4 completed successfully
Reassignment of partition test_me-2 completed successfully
Reassignment of partition test_me-0 completed successfully
Reassignment of partition test_me-1 completed successfully
Throttle was removed.
```

迁移后副本情况

```sh
./bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test_me
Topic: test_me	PartitionCount: 5	ReplicationFactor: 3	Configs: 
	Topic: test_me	Partition: 0	Leader: 2	Replicas: 1,0,2	Isr: 2,0,1
	Topic: test_me	Partition: 1	Leader: 2	Replicas: 2,1,3	Isr: 1,2,3
	Topic: test_me	Partition: 2	Leader: 3	Replicas: 3,2,4	Isr: 2,3,4
	Topic: test_me	Partition: 3	Leader: 0	Replicas: 4,3,0	Isr: 0,3,4
	Topic: test_me	Partition: 4	Leader: 1	Replicas: 0,4,1	Isr: 1,4,0
```

迁移副本后发送数据，可见，<font color="red">**迁移后，legacy code 不需要进行任何更改，依然可以正常进行生产和消费，不受影响**</font>

```sh
./bin/kafka-console-producer.sh --broker-list localhost:9092,localhost:9093  --topic test_me
>msg1_after_reassgin
>msg2_after_reassign
>
```



迁移过程中consumer可能会收到消息。

```sh
./bin/kafka-console-consumer.sh --bootstrap-server localhost:9094 --topic test_me --from-beginning 
msg1
msg3
msg7
msg_snd_after_2_broker_added
msg2_after_2_new_partition
msg5_after_2_new_partition
msg2
msg4
msg6
msg5
msg8
msg1_snd_after_2_broker_added
msg4_after_2_new_partition
msg6_after_2_new_partition
msg1_after_2_new_partition
msg3_after_2_new_partition
msg7-after_2_new_partition



msg1_after_reassgin
msg2_after_reassign
```



迁移后各个数据目录下的文件

```sh
d@rd-ubt:/tmp$ ls -al kafka1-logs/ | grep test
drwxrwxr-x  2 rd   rd    4096 4月  13 15:51 test_me-0
drwxrwxr-x  2 rd   rd    4096 4月  13 15:50 test_me-3
drwxrwxr-x  2 rd   rd    4096 4月  13 15:20 test_me-4
rd@rd-ubt:/tmp$ ls -al kafka2-logs/ | grep test
drwxrwxr-x  2 rd   rd    4096 4月  13 15:51 test_me-0
drwxrwxr-x  2 rd   rd    4096 4月  13 14:55 test_me-1
drwxrwxr-x  2 rd   rd    4096 4月  13 15:50 test_me-4
rd@rd-ubt:/tmp$ ls -al kafka3-logs/ | grep test
drwxrwxr-x  2 rd   rd    4096 4月  13 15:50 test_me-0
drwxrwxr-x  2 rd   rd    4096 4月  13 15:50 test_me-1
drwxrwxr-x  2 rd   rd    4096 4月  13 15:51 test_me-2
rd@rd-ubt:/tmp$ ls -al kafka4-logs/ | grep test
drwxrwxr-x  2 rd   rd    4096 4月  13 15:50 test_me-1
drwxrwxr-x  2 rd   rd    4096 4月  13 15:50 test_me-2
drwxrwxr-x  2 rd   rd    4096 4月  13 15:29 test_me-3
rd@rd-ubt:/tmp$ ls -al kafka5-logs/ | grep test
drwxrwxr-x  2 rd   rd    4096 4月  13 15:51 test_me-2
drwxrwxr-x  2 rd   rd    4096 4月  13 15:53 test_me-3
drwxrwxr-x  2 rd   rd    4096 4月  13 15:20 test_me-4
rd@rd-ubt:/tmp$
```



## 8.9 重新选举分区 leader

```sh
# 测试未成功
/bin/kafka-leader-election.sh --bootstrap-server localhost:9092 --topic test_me --election-type PREFERRED --partition 1
```

## 8.10 批量生产消息

执行脚本 batch_producer.sh

```
touch batch_producer.sh
:set paste
# 粘贴脚本文本
:wq!
chmod +x batch_producer.sh
./batch_producer.sh
```

脚本内容如下所示(tested on Ubuntu 20.04)，生产速率约1s1条，较慢

```sh
#!/usr/bin/sh
echo '---------- start produce msg ----------'

#响应Ctrl+C中断
trap 'onCtrlC' INT

onCtrlC () {
    echo 'Ctrl+C is captured'
    exit 1
}

#kafka所在目录
kafkaPath=/home/rd/software/kafka_2.11-2.4.1
#broker
brokerlist=localhost:9092,localhost:9093,localhost:9094
#kafka的topic
topic=test_me
#消息总数
totalNum=100000
#一次批量发送的消息数
batchNum=10
#该标志为true，表示文件中的第一条记录
firstLineFlag='true'

#C语法的for循环，部分 shell 可能不支持
#for ((i=1; i<=${totalNum}; i ++)) 
# 使用通用的shell 循环写法
for i in `seq ${totalNum}`
 
do  
	# 精确到纳秒
	timestamp=`date "+%Y-%m-%d %H:%M:%S.%N"`;
	# for test purpose only
	echo $timestamp
	#消息内容，请按照实际需要自行调整
	
    messageContent='my_msg'$timestamp
    #$RANDOM
    #如果是每个批次的第一条，就要将之前的内容全部覆盖，如果不是第一条就追加到尾部
    # for ubuntu
    if [ 'true' = "${firstLineFlag}" ] ; then 
    # for centos
    #if [ 'true' == "${firstLineFlag}" ] ; then
      echo ${messageContent} > batchMessage.txt
      #将标志设置为false，这样下次写入的时候就不会清理已有内容了
      firstLineFlag='false'
    else
      echo ${messageContent} >> batchMessage.txt
    fi

    #取余数
    modVal=$(( ${i} % ${batchNum} ))
    #如果达到一个批次，就发送一次消息
    if [ "${modVal}" = 0 ] ; then
      #在控制台显示进度
      echo “${i} of ${totalNum} sent”

      #批量发送消息，并且将控制台返回的提示符重定向到/dev/null
      cat batchMessage.txt | ${kafkaPath}/bin/kafka-console-producer.sh --socket-buffer-size 1024000 --broker-list ${brokerlist} --sync --topic ${topic} | > /dev/null
	  echo '--snd msg--'
	  cat  batchMessage.txt
      #将标志设置为true，这样下次写入batchMessage.txt时，会将文件中的内容先清除掉
      firstLineFlag='true'
    fi
done

```



# 9. 总结

* 扩容后，已有的代码连接扩容前的borker list，依然可以正常进行消息的生产和消费，不受扩容影响
* 扩容过程中，如果不停止consumer，可能会收到之前已经收到的消息（**重复消息**）
* TODO：扩容过程中，若生产者和消费者都不停止，对系统的影响待测试
* TODO： 大批量数据扩容时间估算，待测试。



# python consumer

## setup
Python 3.x

Install the Kafka library

```sh
pip3 install confluent-kafka
```

## demo

vi kafka.cfg

```sh
[default]
bootstrap.servers=11.10.36.1:9092

[consumer]
group.id=python_grp

# 'auto.offset.reset=earliest' to start reading from the beginning of
# the topic if no committed offsets exist.
auto.offset.reset=earliest
```

vi kafka_consumer.py

```python
#!/usr/bin/python3
#       #!/usr/bin/env python
#       author whoami
#       pip3 install pip3 install confluent-kafka
#       ./kafks_consumer.py kafka.cfg
import sys
from argparse import ArgumentParser, FileType
from configparser import ConfigParser
from confluent_kafka import Consumer, OFFSET_BEGINNING

if __name__ == '__main__':
    # Parse the command line.
    parser = ArgumentParser()
    parser.add_argument('config_file', type=FileType('r'))
    parser.add_argument('--reset', action='store_true')
    args = parser.parse_args()
    print("args", args)
    # Parse the configuration.
    # See https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md
    config_parser = ConfigParser()
    config_parser.read_file(args.config_file)
    config = dict(config_parser['default'])
    config.update(config_parser['consumer'])

    # Create Consumer instance
    consumer = Consumer(config)
    print("consumer_config", config)
    # Set up a callback to handle the '--reset' flag.
    def reset_offset(consumer, partitions):
        if args.reset:
            for p in partitions:
                p.offset = OFFSET_BEGINNING
            consumer.assign(partitions)

    # Subscribe to topic
    # topic = "test-up-rpt-dt"
    topic = "reg-info"
    consumer.subscribe([topic], on_assign=reset_offset)
    print("consumer.subscribe", topic)
    # Poll for new messages from Kafka and print them.
    try:
        while True:
            msg = consumer.poll(1.0)
            if msg is None:
                # Initial message consumption may take up to
                # `session.timeout.ms` for the consumer group to
                # rebalance and start consuming
                print("Waiting...")
            elif msg.error():
                print("ERR,", msg.error())
                break
            else:
                # Extract the (optional) key and value, and print.

                print("Consumed event from topic {topic}: key = {key:12} value = {value:12} offset = {offset}".format(
                    topic=msg.topic(), key=msg.key().decode('utf-8'), value=msg.value().decode('utf-8'), offset=msg.offset()))
    except KeyboardInterrupt:
        pass
    finally:
        # Leave group and commit final offsets
        consumer.close()

```

add permission

```
chmod +x kafka_consumer.py
```

run script

```sh
./kafka_consumer.py kafka.cfg
```



