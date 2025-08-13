#!/bin/sh
# -i: 每次执行ping操作的间隔时间, 默认是1s;
# -c: 执行ping操作的次数, 默认是一直执行, 除非被中断;
# -s: 指定执行ping操作时发送的包的大小, 默认是56B, 添加报文头之后, 最终发送的是64B
# ping 86400次, 默认间隔1秒, 也就是24小时
# on centos
echo '' > ping.log
while true
do
    ping -c 1  192.168.1.1 | grep from | awk '{ print $0"\t" strftime("%D:%H:%M:%S",systime()) }' >> ping.log 
    sleep 1s
done
