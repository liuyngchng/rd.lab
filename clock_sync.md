##  env



```shell
docker pull ubuntu

docker network create --subnet=172.18.0.0/16 mynetwork
docker network ls
# 添加参数，可修改系统时间,可管理网络
docker run -itd --name test1 --net mynetwork --ip 172.18.0.2 --cap-add SYS_TIME --cap-add=NET_ADMIN ubuntu:latest
docker run -itd --name test2 --net mynetwork --ip 172.18.0.3 --cap-add SYS_TIME --cap-add=NET_ADMIN ubuntu:latest
docker exec -it test1 bash
# ip addr, tc
apt-get install iproute2 -y
# ping
apt-get install inetutils-ping  -y
# ifconfig
apt-get install net-tools -y  
# 添加网络延迟
tc qdisc add dev eth0 root netem delay 150ms
# 限制带宽
# 将eth0网卡限速到500Kbit/s，15bk的buffer，TBF最多产生50ms的延迟
tc qdisc add dev eth0 root tbf rate 500Kbit latency 50ms burst 15kb

```



