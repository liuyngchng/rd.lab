# 1. ubuntu  share network between hosts (通过网线共享网络)

host A：ubuntu16.04, 有两个网卡，一个接外网，一个与主机B相接  
hostB：ubuntu16.04  

## 1.1 config host A 
run `iwconfig`  
wlp2s0 :这个是无线网卡。
enp1s0 :有线网卡，与B主机通过网线相连的网卡  

config ip  
sudo vim /etc/network/interfaces  
为接口enp1s0配置静态IP地址， 
```
iface enp1s0 inet static
address 192.168.49.1
netmask 255.255.255.0
gateway 192.168.49.1
```
restart interface enp1s0
```
ifdonw enp1s0
ifup enp1s0
ifconfig  命令查看enp1s0 ip配置是否成功
```
## 1.2 config host B

run  `iwconfig` 
获取网络接口卡名称 enpxxxx    
sudo vim /etc/network/interfaces 
```  
iface enpxxxx inet static
address 192.168.49.2
netmask 255.255.255.0
gateway 192.168.49.1
dns-nameservers 180.76.76.76
```
restart interface enpxxxxx
```
ifdonw enp1s0
ifup enp1s0
ifconfig  命令查看enpxxxx ip配置是否成功
```
ping host A OK `ping 192.168.49.1`

## 1.3 config NAT on host A

这一步是为了B主机能通过A主机访问外网  

```
su
echo 1 > /proc/sys/net/ipv4/ip_forward 
iptables -F
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -t nat -A POSTROUTING -o wlp2s0 -j MASQUERADE     （wlp2s0为host A接外网的网卡）
```
## 1.4 debug

配置完以上信息后，若发现 host A 无法上网，则是默认路由导致的，
执行
```
ip route show
sudo route del default gw 192.168.49.1
```

