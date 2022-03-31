# 1. mininet setup
## 1.1 env
OS: ubuntu 16.04 LTS
Instruction : http://mininet.org/
```
git clone git://github.com/mininet/mininet
cd mininet
git tag  # list available versions
git checkout -b 2.2.2 2.2.2  # 2.2.1 版本在checkout openflow时由于git地址问题会报错
cd ..
mkdir -p /usr/local/lib/mininet  # 创建build目录
mininet/util/install.sh -s /usr/local/lib/mininet -a
```
# 2. network monitor and config
## 2.1 network traffic
```
iftop -i interface
```
## 2.2 network traffic control  
把网卡eth0的传输设置为：  
延时100ms(上下误差10ms)、丢包6%、包重复0.8%、包损坏0.5%的网络环境
```
tc qdisc add dev eth0 root netem delay 100ms 10ms loss 6% duplicate 0.8% corrupt 0.5%
```
移除对网卡eth0的传输设置，恢复正常网络
```
tc qdisc del dev eth0 root netem
```
# 3. mininet topo
## 3.1 default topo
```
sudo mn
*** Adding links:
(h1, s1) (h2, s1)
```
1 switch ,2 host
```
s1
+- h1
\- h2
```
## 3.2 single
```
mn --topo=single,3
(h1, s1) (h2, s1) (h3, s1) 
```
one switch , 3 host
```
s1
+- h1
+- h2
\- h3
```
## 3.3 linar
```
mn --topo=linear,5
*** Adding links:
(h1, s1) (h2, s2) (h3, s3) (h4, s4) (h5, s5) (s2, s1) (s3, s2) (s4, s3) (s5, s4) 
```
linar connected switch
```

+- s1
|   \- h1
|
+- s2
|   \- h2
|
+- s3
|   \- h3
|
+- s4
|   \- h4
|
+- s5
    \- h5
```

## 3.4 tree
```
mn --topo=tree,depth=2,fanout=3
*** Adding links:
(s1, s2) (s1, s3) (s1, s4) (s2, h1) (s2, h2) (s2, h3) (s3, h4) (s3, h5) (s3, h6) (s4, h7) (s4, h8) (s4, h9)
```
tree with depth 2
```
s1 
+- s2+- h1
|    +- h2
|    \- h3
|
+- s3+- h4
|    +- h5
|    \- h6
|
+- s4+- h7
     +- h8
     \- h9
```
# 4 cmd to change topo
## 4.1 py命令 

执行Python表达式，例如添加网络设备。下面举例说明：

在mininet自带的网络拓扑的基础上，我们想添加一个主机h3，并且让它和s1之间连通，且可以和其他两个主机h1和h2互相ping通。
```
sudo mn
py net.addHost('h3')                       #给当前网络net添加新的主机h3
py net.addLink(s1,h3)                      #为主机h3和交换机s1之间添加一条链路
py s1.attach('s1-eth3')                    #为交换机s1添加一个借口用于与h3相连
py h3.cmd('ifconfig h3-eth0 10.3')         #为主机h3配置端口及改端口的IP地址
h1 ping h3                                 #重要！！h1知道h3的IP地址，而h3不知道h1的地址，因此需要h1告诉h3的地址
pingall                                    #测试所有主机都是连通的
```
## 4.2 link
和links不同，用法如下：
```
link s1 s2 down/up              #禁用/开启s1和s2这两个节点之间的链路
```
## 4.3 iperf
```
iperf h1 h2                     #测试两个主机h1和h2之间的TCP带宽
```
## 4.4 iperfupd
```
iperfupd bw h1 h2             测试两个主机h1和h2之间的UDP带宽
```
## 4.5 dpctl
对所有交换机操作流表，包括流表的显示、添加、修改、删除，具体用法，参考dpctl命令实践

## 4.6 xterm
如xterm h1打开主机h1的操作窗口
## 4.7 python customize
```python
#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel

class SingleSwitchTopo( Topo ):
    "Single switch connected to n hosts."
    def build( self, n=2 ):
    switch = self.addSwitch( 's1' )
    for h in range(n):
        # Each host gets 50%/n of system CPU
        host = self.addHost( 'h%s' % (h + 1),
                         cpu=.5/n )
        # 10 Mbps, 5ms delay, 2% loss, 1000 packet queue
        self.addLink( host, switch, bw=10, delay='5ms', loss=2,
                          max_queue_size=1000, use_htb=True )

def perfTest():
    "Create network and run simple performance test"
    topo = SingleSwitchTopo( n=4 )
    net = Mininet( topo=topo,
               host=CPULimitedHost, link=TCLink )
    net.start()
    print "Dumping host connections"
    dumpNodeConnections( net.hosts )
    print "Testing network connectivity"
    net.pingAll()
    print "Testing bandwidth between h1 and h4"
    h1, h4 = net.get( 'h1', 'h4' )
    net.iperf( (h1, h4) )
    net.stop()

if __name__ == '__main__':
    setLogLevel( 'info' )
    perfTest()
```
# 5. GUI
```
cd mininet/examples
python ./miniedit.py
```
File -> Export Level 2 Script -> test.py
```
sudo python ./test.py
```

