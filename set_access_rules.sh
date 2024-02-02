# 停止防火墙
systemctl stop firewalld
systemctl disable firewalld
systemctl mask firewalld
# setup iptables
yum install iptables iptables-services -y
# 允许所有访问
iptables -P INPUT ACCEPT
# 清空所有默认规则
iptables -F INPUT
# 清空所有自定义规则
iptables -X INPUT
# 所有计数器归0
iptables -Z INPUT

# 禁止ping
iptables -A INPUT -p icmp --icmp-type 8 -s 0/0 -j DROP

### 开启ping ### 
# 查看禁止ping的语句
#iptables -L INPUT --line-numbers  | grep -i icmp
# 删除语句编号
#iptables -D INPUT 14   #14代表编号
# 开启ping
#iptables -A INPUT -p icmp --icmp-type echo-request -j ACCEPT
#iptables -A OUTPUT -p icmp --icmp-type echo-reply -j ACCEPT

echo 'set rules for all app'
# docker
iptables -I INPUT -p tcp --dport 4243 -j DROP
# zookeeper
iptables -I INPUT -p tcp --dport 2181 -j DROP
iptables -I INPUT -p tcp --dport 2888 -j DROP
iptables -I INPUT -p tcp --dport 3888 -j DROP
# elastic search
iptables -I INPUT -p tcp --dport 9200 -j DROP
iptables -I INPUT -p tcp --dport 9300 -j DROP
#mysql
iptables -I INPUT -p tcp --dport 3306 -j DROP

#mysql5.7
iptables -I INPUT -p tcp --dport 3307 -j DROP

#redis
iptables -I INPUT -p tcp --dport 6379 -j DROP
#mt-api
iptables -I INPUT -p tcp --dport 8080 -j DROP
#mt-gw
iptables -I INPUT -p tcp --dport 8081 -j DROP
#mt-dbg
iptables -I INPUT -p tcp --dport 8082 -j DROP
#mt-pda
iptables -I INPUT -p tcp --dport 8083 -j DROP
#rancher
iptables -I INPUT -p tcp --dport 8084 -j DROP
iptables -I INPUT -p tcp --dport 8443 -j DROP
#kafka
iptables -I INPUT -p tcp --dport 9092 -j DROP


# 允许11.10.36.1-11.10.36.5 对 4243端口的访问
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 4243 -j ACCEPT

echo 'set rules for zookeeper'
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 2181 -j ACCEPT
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 2888 -j ACCEPT
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 3888 -j ACCEPT

echo 'set rules for elasticsearch'
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 9200 -j ACCEPT
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 9300 -j ACCEPT

echo 'set rules for mysqld'
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 3306 -j ACCEPT
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 3307 -j ACCEPT

echo 'set rules for app'
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 8080 -j ACCEPT
# a10
iptables -I INPUT -m iprange --src-range 11.11.115.57-11.11.115.66 -p tcp --dport 8080 -j ACCEPT

echo 'set rules for redis'
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 6379 -j ACCEPT

echo 'set rules for kafka'
iptables -I INPUT -m iprange --src-range 11.10.36.1-11.10.36.5 -p tcp --dport 9092 -j ACCEPT

echo 'other rules'
#rd lan
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 8443 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 9092 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 3306 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 3307 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 6379 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 8080 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 8081 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 8082 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 8083 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 8084 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 8443 -j ACCEPT
iptables -I INPUT -s 11.0.18.37 -p tcp --dport 2181 -j ACCEPT
# docker
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 8443 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 9092 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 3306 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 3307 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 6379 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 8080 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 8081 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 8082 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 8083 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 8084 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 8443 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 9022 -j ACCEPT
iptables -I INPUT -m iprange --src-range 172.17.0.1-172.17.0.255 -p tcp --dport 2181 -j ACCEPT

# DMZ
iptables -I INPUT -s 11.8.44.133 -p tcp --dport 8443 -j ACCEPT
iptables -I INPUT -s 11.8.44.133 -p tcp --dport 9092 -j ACCEPT
iptables -I INPUT -s 11.8.44.133 -p tcp --dport 3306 -j ACCEPT
iptables -I INPUT -s 11.8.44.133 -p tcp --dport 6379 -j ACCEPT
iptables -I INPUT -s 11.8.44.133 -p tcp --dport 8080 -j ACCEPT
iptables -I INPUT -s 11.8.44.133 -p tcp --dport 8081 -j ACCEPT
iptables -I INPUT -s 11.8.44.133 -p tcp --dport 8082 -j ACCEPT
iptables -I INPUT -s 11.8.44.133 -p tcp --dport 8083 -j ACCEPT
iptables -I INPUT -s 11.8.44.133 -p tcp --dport 2181 -j ACCEPT

iptables -I INPUT -s 11.11.115.87 -p tcp --dport 8443 -j ACCEPT
iptables -I INPUT -s 11.11.115.87 -p tcp --dport 9092 -j ACCEPT
iptables -I INPUT -s 11.11.115.87 -p tcp --dport 3306 -j ACCEPT
iptables -I INPUT -s 11.11.115.87 -p tcp --dport 6379 -j ACCEPT
iptables -I INPUT -s 11.11.115.87 -p tcp --dport 8080 -j ACCEPT
iptables -I INPUT -s 11.11.115.87 -p tcp --dport 8081 -j ACCEPT
iptables -I INPUT -s 11.11.115.87 -p tcp --dport 8082 -j ACCEPT
iptables -I INPUT -s 11.11.115.87 -p tcp --dport 8083 -j ACCEPT
iptables -I INPUT -s 11.11.115.87 -p tcp --dport 2181 -j ACCEPT


# 保存访问策略到文件
service iptables save
# 查看服务状态
systemctl is-enabled iptables.service
echo 'iptables-restore /etc/sysconfig/iptables'
iptables-restore /etc/sysconfig/iptables
echo '++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
echo '******* current host access rules as following **********'
echo '++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
# 查看当前访问策略
iptables -L -n
