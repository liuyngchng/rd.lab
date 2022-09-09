##  env



```shell
docker pull ubuntu

docker network create --subnet=172.18.0.0/16 mynetwork
docker network ls
# 添加参数，可修改系统时间,可管理网络
docker run -itd --name h1 --net mynetwork --ip 172.18.0.2 --cap-add SYS_TIME --cap-add=NET_ADMIN -v /home/rd/workspace/clock.sync:/opt ubuntu:latest
docker run -itd --name h2 --net mynetwork --ip 172.18.0.3 --cap-add SYS_TIME --cap-add=NET_ADMIN -v /home/rd/workspace/clock.sync:/opt ubuntu:latest
docker run -itd --name h3 --net mynetwork --ip 172.18.0.110 --cap-add SYS_TIME --cap-add=NET_ADMIN -v /home/rd/workspace/clock.sync:/opt ubuntu:latest
docker exec -it h1 bash
unset HTTPS_PROXY HTTP_PROXY  http_proxy
apt-get update
# ip addr, tc ping ifconfig
apt-get install iproute2 inetutils-ping  net-tools -y 
# 添加网络延迟
tc qdisc add dev eth0 root netem delay 4000ms
# 限制带宽
# 将eth0网卡限速到500Kbit/s，15bk的buffer，TBF最多产生50ms的延迟
tc qdisc add dev eth0 root tbf rate 500Kbit latency 50ms burst 15kb
# 取消限速
tc qdisc del dev eth0 root

```



运行shell, 设置延迟

```shell
touch delay.sh
# 内容如下
#!/bin/bash
interface=eth0
ip=172.18.0.2
delay=4000ms
tc qdisc add dev $interface root handle 1: prio
tc filter add dev $interface parent 1:0 protocol ip prio 1 u32 match ip dst $ip flowid 2:1
tc qdisc add dev $interface parent 1:1 handle 2: netem delay $delay
```

运行shell ，设置限速

```sh
# 下行速率
down=5Mbit
# 上行速率
upload=5Mbit
#接口名称
interface=eth0
# set down
# 建立qdisc队列
tc qdisc add dev $interface root handle 2:0 htb
# 建立类,限速5M
tc class add dev $interface parent 2:1 classid 2:10 htb rate $down
tc class add dev $interface parent 2:2 classid 2:11 htb rate 1024kbps
# 为了不使一个会话永占带宽,添加随机公平队列sfq
tc qdisc add dev $interface parent 2:10 handle 1: sfq perturb 1
# 建立过滤器规则,对192.168.1.0/24下载限速为5M
tc filter add  dev $interface protocol ip parent 2:0  u32 match ip dst 192.168.1.0/24  flowid 2:10
#upload
# 限制192.168.1.0/24上传限速为5M
tc qdisc add dev $interface handle ffff: ingress
tc filter add dev $interface parent ffff: protocol ip  u32 match ip dst 192.168.1.0/24 police  rate $upload burst 100k drop flowid 2:11

# 删除所有 TC 限速规则
# tc qdisc del dev ppp0 root
# 显示qdisc队列状态
# tc -s -d qdisc show dev ppp0
# 显示class类状态
# tc -s -d class show dev ppp0
# 显示filter规则状态
# tc -s -d filter show dev ppp0
```

# run demo

```shell
java -jar spring-boot-test.jar &
curl http://172.18.0.3:8082/data/test
```

当前系统时间ms

```
echo $(date '+%Y-%m-%d %H:%M:%S').$((`date '+%N'`/1000000))
```

