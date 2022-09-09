#!/bin/bash
# 接口名称
interface=eth0
# 删除所有规则
tc qdisc del dev $interface root
# 被限制的IP
ip=172.18.0.3
delay=4000ms
tc qdisc add dev $interface root handle 1: prio
tc filter add dev $interface parent 1:0 protocol ip prio 1 u32 match ip dst $ip flowid 2:1
tc qdisc add dev $interface parent 1:1 handle 2: netem delay $delay
