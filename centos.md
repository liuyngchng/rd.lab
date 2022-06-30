# network
解决centos minimal 安装无法上网的问题

```sh
cd /etc/sysconfig/network-scripts/
vi ifcfg-enp0s3         //ifcfg-之后为网卡名称

```
将最后一行`ONBOOT=NO` 修改为 `ONBOOT=YES`

# install  docker in centos offline

##  update
```sh
docker pull centos          //centos8
yum update
```
###  centos8

会出现

```sh
Error: Failed to download metadata for repo 'appstream': Cannot prepare internal mirrorlist: No URLs in mirrorlist
```

修改 mirror list ，执行

```sh
sed -i -e "s|mirrorlist=|#mirrorlist=|g" /etc/yum.repos.d/CentOS-*
sed -i -e "s|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g" /etc/yum.repos.d/CentOS-*
```
后可以正常update

只下载不安装

```sh
yum -y install --downloadonly --downloaddir=./
```
or

```sh
yum install yum-utils
yumdownloader xxxx
```
###  centos7

```sh
yum-config-manager --add-repo http://download.docker.com/linux/centos/docker-ce.repo
or
yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
```
###  查看可以安装的docker 版本

```
yum list docker-ce --showduplicates | sort -r
```
看到

```
docker-ce.x86_64                3:20.10.0-3.el8                 docker-ce-stable
docker-ce.x86_64                3:19.03.15-3.el8                docker-ce-stable
docker-ce.x86_64                3:19.03.14-3.el8                docker-ce-stable
docker-ce.x86_64                3:19.03.13-3.el8                docker-ce-stable
```
开始安装

```sh
mkdir docker_rpm
yumdownloader --resolve  docker-ce
ls | sort | tr "\n" " " | xargs rpm -ivh

```
启动并加入开机启动
```sh
systemctl start docker
systemctl enable docker
```
验证安装是否成功(有client和service两部分表示docker安装启动都成功了)
```sh
docker version
docker info
```
遇到

```
```

##  centos7 minimal VirtualBox Insert Guest Addtions CD Image

download the iso file first from VirtualBox GUI, and then see

```
Unable to insert the virtual optical disk /home/rd/.config/VirtualBox/VBoxGuestAdditions_6.1.32.iso into the machine centos.

Could not mount the media/drive '/home/rd/.config/VirtualBox/VBoxGuestAdditions_6.1.32.iso' (VERR_PDM_MEDIA_LOCKED).

Result Code: NS_ERROR_FAILURE (0x80004005)
Component: ConsoleWrap
Interface: IConsole {872da645-4a9b-1727-bee2-5585105b9eed}
Callee: IMachine {85632c68-b5bb-4316-a900-5eb28d3413df}

```
you should do
```
mount /dev/cdrom /mnt
cd /mnt
./VBoxLinuxAdditions.run
```
and then you see
```
:kernel headers not found for target kernel ****
```
you should do
```
yum update kernel -y
yum install kernel-headers kernel-devel gcc make -y
```

#  install mysql8 on centos7.9 minimal

##  install

download `mysql-8.0.28-1.el7.x86_64.rpm-bundle.tar`  
for `RedHadEnterprise Linux 7 /Oracle Linux 7 x86,64bit`  
in https://dev.mysql.com/downloads/mysql/

```
tar -xf  mysql-8.0.28-1.el7.x86_64.rpm-bundle.tar
```

看到一堆rpm文件，

try

```
ls *.rpm | sort | tr '\n' ' ' | xargs rpm -ivh
```

逐个下载缺少的依赖

```
yum search ****
yumdownloader --resolve ****
```

直到不再缺少依赖，只显示错误

```
error: Failed dependencies:
	mariadb-libs is obsoleted by mysql-community-libs-8.0.28-1.el7.x86_64
```

查看已经安装的rpm包，包含关键字“mariadb-lib”
```
rpm -qa | grep mariadb-libs
```

删除依赖

```
yum remove mariadb-libs-5.5.68-1.el7.x86_64
```

出现

```
file ****  from install of ***** conflicts with file from package *****
```

执行强制安装

```
ls *.rpm | sort | tr '\n' ' ' | xargs rpm -ivh --force
```

##  config

查看服务状态

```
systemctl status mysqld
```

停止服务

```
service mysqld stop
```

初始化数据库

```
mysqld --initialize --console
```

目录授权

```
chown -R mysql:mysql /var/lib/mysql/
```



启动mysql服务

```
systemctl start mysqld
```

查看临时密码

```
grep 'password' /var/log/mysqld.log
```

修改mysql密码

 ```
 mysql -uroot -p 				//输入临时密码
 alter USER 'root'@'localhost' IDENTIFIED BY 'mypassword';
 ```

授权远程连接

```
show databases;
use mysql;
select host, user, authentication_string, plugin from user;

update user set host = "%" where user='root';
select host, user, authentication_string, plugin from user;
flush privileges;
```
远程 telnet IP port， 若不通，则可能是防火墙原因，执行

```
systemctl status firewalld
systemctl stop firewall
```

远程连接若出现 Authenticationplugin‘caching_sha2_password’ cannot be loaded:XXXXX， 则


```
mysql -u root -p
use mysql;
alter USER 'root'@'%' IDENTIFIED WITH mysql_native_password BY 'mypassword';
flush privileges;
```

```
备注：
mysql8 之前的版本中加密规则是mysql_native_password，而在mysql8之后,加密规则是caching_sha2_password。
可以把mysql用户登录密码加密规则还原成mysql_native_password.。
```

##   start
出现
```
Docker: System has not been booted with systemd as init system
```
这是因为启动容器命令如下：

```
docker run -d --name centos_1 -it  centos:latest /bin/bash
```

或

```
docker run -d --name centos_1 -it  centos:latest
```

解决办法是更换启动命令：

```
docker run -tid --name centos_1 --privileged=true centos:latest /sbin/init
```

区别在于后面用了

```
/sbin/init
```

# FAQ

yum install xxxx.rpm 提示 `A conflicts with file from package B`

两种方法

(1)  rpm -ivh --replacefiles  A.rpm,

(2) yum -y remove B 然后再安装A



提示 `A is obsoleted by B`

yum -y remove  A, 然后再安装B



#  redis

```sh
yum -y install gcc gcc-c++ libstdc++-devel
tar -zxf  redis-stable.tar.gz
cd redis-stable
vi ./src/.make-settings
#将 OPT = -O2 修改为 OPT=-O2 -march=x86-64
# 否则会报 undefined *** je_malloc_usable_size *** error
cd deps
#deps目录下的所有组件，均需要make
cd jemalloc/
./configure
make MALLOC=libc
make install
cd ../..
make
redis-server --version
Redis server v=6.2.6 sha=00000000:0 malloc=libc bits=64 build=bcb67d162ef0f4f8

```

设置密码并启动

```sh
vi ./redis.conf
# 设置 requirepass yourpassword
redis-server /etc/redis.conf &
```

# nc

##  安装nc工具



```sh
yum install nc -y
```

##  端口探测

###  TCP端口探测

```sh
# server listen port
nc -l port
# client connect server
nc -w 1 IP port < /dev/null && echo "tcp port ok"
```

举例， 对方tcp端口可连接：

```sh
nc -w 1 192.168.21.17 34567 < /dev/null && echo "tcp port ok"
tcp port ok
```

对方tcp端口不可连接：

```sh
nc  -w 1 192.168.21.17 34567 < /dev/null && echo "tcp port ok"
Ncat: Connection refused.
```

###  UDP端口探测

使用方法：

```sh
nc -u -w 1 IP port < /dev/null && echo "udp port ok"
```

举例，对方UDP端口可连接

```sh
nc -u -w 1 192.168.21.17 34567 < /dev/null && echo -e "udp port ok"
udp port ok
```

#  iptables

##  basic

执行

```sh
yum install iptables-services
systemctl is-enabled iptables.service
##启用iptables
 systemctl enable iptables  
```


iptables规则重启自动生效--永久生效

```sh
iptables-save >  /etc/sysconfig/iptables
# 手动恢复
iptables-restore < /etc/sysconfig/iptables
# 开机自动恢复规则，把恢复命令添加到启动脚本：
echo '/sbin/iptables-restore /etc/sysconfig/iptables' >>/etc/rc.d/rc.local
# 现在reboot重启服务器，查看/root/restart.log日志如下：
chmod +x /etc/rc.d/rc.local
```

查看iptables现有规则

```bash
iptables -L -n
```

先允许所有,不然有可能会杯具

```bash
iptables -P INPUT ACCEPT
```

清空所有默认规则

```bash
iptables -F INPUT
```

清空所有自定义规则

```bash
iptables -X INPUT
```

所有计数器归0

```bash
iptables -Z INPUT
```

许来自于lo接口的数据包(本地访问)

```bash
iptables -A INPUT -i lo -j ACCEPT
```

开放22端口

```bash
iptables -A INPUT -p tcp --dport 22 -j ACCEPT
```

允许ping

```bash
iptables -A INPUT -p icmp --icmp-type 8 -j ACCEPT
```

禁用某网段

    iptables -I INPUT -p tcp -s 192.168.116.0/24 -j DROP

禁用某网段的22端口

    iptables -I INPUT -p tcp -s 192.168.116.0/24 --dport 22 -j DROP

禁用192.168.116.1~192.168.116.20 IP段的 22 端口

    iptables -I INPUT -m iprange --src-range 192.168.116.1-192.168.116.20 -p tcp --dport 22 -j DROP

禁用192.168.116.1~192.168.116.20 经过网卡eno1的 IP段的 22 端口

    iptables -I INPUT -m iprange --src-range 192.168.116.1-192.168.116.20 -p tcp -i eno1 --dport 22 -j DROP

禁用192.168.116.1~192.168.116.20 经过网卡eno1的 目的IP为192.168.116.118的 IP段的 22 端口

```
iptables -I INPUT -m iprange --src-range 192.168.116.1-192.168.116.20 -p tcp -i eno1 -d 192.168.116.118 --dport 22 -j DROP
```

##  example

假设希望仅仅允许 111.111.0.0/16 IP段ssh登录，而禁止其他所有ip登录。

最简单的命令是：

```sh
# 禁止所有对22端口的入访问
iptables -I INPUT -p tcp --dport 3307 -j DROP
# 允许111.111.0.0/16的访问
iptables -I INPUT -m iprange --src-range 192.168.1.1-192.168.1.2 -p tcp --dport 3307 -j ACCEPT
```



#  firewall
##  status

```sh
# 打开防火墙
systemctl start firewalld
# 关闭防火墙
systemctl stop firewalld
# 查看防火墙状态
systemctl status firewalld
# 启用防火墙
systemctl enable firewalld
# 禁用防火墙
systemctl disable firewalld

```
##  config
```sh
# 列出所支持的zone和查看当前的默认zone
firewall-cmd --get-zones
# 默认使用区域
firewall-cmd --get-default-zone
# 查看防火墙规则
firewall-cmd --list-all
# 查看规则状态
firewall-cmd --state
# 加载规则
firewall-cmd --reload
# 开放3306端口
firewall-cmd --add-port=3306/tcp --permanent
# 开放网段
 firewall-cmd --permanent --add-source=192.168.0.0/22
# 移除规则
 firewall-cmd --permanent --remove-source=192.168.0.0/22
# 开放服务
firewall-cmd --permanent --add-service=http
# 移除服务
firewall-cmd --permanent --remove-service=http

# 允许192.168.0.142访问80端口
firewall-cmd --permanent --add-rich-rule="rule family="ipv4" source address="192.168.0.142" port protocol="tcp" port="80" accept"
# family  对哪个协议
# source address	源地址
# accept	允许
# drop	拒绝

# 拒绝192.168.0.142访问80端口
firewall-cmd --permanent --add-rich-rule="rule family="ipv4" source address="192.168.0.142" port protocol="tcp" port="80" drop"
```

# 系统盘扩容

##  原始大小

磁盘占用率如下

```sh
# shell snippet 1
df -lh
Filesystem                 Size  Used Avail Use% Mounted on
/dev/mapper/rootvg-lvroot   30G   18G   13G  59% /
devtmpfs                    16G     0   16G   0% /dev
*****************************************
tmpfs                      3.2G     0  3.2G   0% /run/user/0
```

原始系统盘 64GB

```sh
# shell snippet 2
lsblk
NAME              MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
vda               252:0    0   64G  0 disk
├─vda1            252:1    0    1G  0 part /boot
└─vda2            252:2    0   63G  0 part
  ├─rootvg-lvroot 253:0    0   30G  0 lvm  /
  ├─rootvg-lvswap 253:1    0   12G  0 lvm  [SWAP]
  ├─rootvg-lvvar  253:2    0   10G  0 lvm  /var
  ├─rootvg-lvhome 253:3    0    1G  0 lvm  /home
  └─rootvg-lvopt  253:4    0   10G  0 lvm  /opt
```

通过云服务器控制台，将系统盘扩容到1000GB（1TB）后，进入操作系统查看

```sh
# shell snippet 3
lsblk
NAME              MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sr0                11:0    1 1024M  0 rom
vda               252:0    0 1000G  0 disk
├─vda1            252:1    0    1G  0 part /boot
└─vda2            252:2    0   63G  0 part
  ├─rootvg-lvroot 253:0    0   30G  0 lvm  /
  ├─rootvg-lvswap 253:1    0   12G  0 lvm  [SWAP]
  ├─rootvg-lvvar  253:2    0   10G  0 lvm  /var
  ├─rootvg-lvhome 253:3    0    1G  0 lvm  /home
  └─rootvg-lvopt  253:4    0   10G  0 lvm  /opt
```

可见vda已经显示为1000GB了, df 查看，磁盘占用率没有变化

```sh
# shell snippet 4
df -lh
Filesystem                 Size  Used Avail Use% Mounted on
/dev/mapper/rootvg-lvroot   30G   18G   13G  59% /

tmpfs                      3.2G     0  3.2G   0% /run/user/0
```

##  创建分区

```sh
# shell snippet 5
# dev/vda 为待扩容的盘符, 详见 shell snippet 3
fdisk /dev/vda
Command (m for help): m #输入m
# 看到结果
Command action
   a   toggle a bootable flag
   *****************
   x   extra functionality (experts only)
# 输入 n， 添加新分区
Command (m for help): n
Partition type:
   p   primary (2 primary, 0 extended, 2 free)
   e   extended
# 回车，选p， 选择主分区
Select (default p): p
# 回车，默认值
Partition number (3,4, default 3):
First sector (134217728-2097151999, default 134217728):
Using default value 134217728
Last sector, +sectors or +size{K,M,G} (134217728-2097151999, default 2097151999):
Using default value 2097151999
Partition 3 of type Linux and of size 936 GiB is set
# 将分区信息写入磁盘
Command (m for help): w
The partition table has been altered!

Calling ioctl() to re-read partition table.

WARNING: Re-reading the partition table failed with error 16: Device or resource busy.
The kernel still uses the old table. The new table will be used at
the next reboot or after you run partprobe(8) or kpartx(8)
Syncing disks.
# 让系统识别新的分区
partprobe
# 查看新的分区
lsblk
#  执行了partprobe，就不需要重启了
#reboot
```

##  扩容

看到出现了新的分区 vda3

```sh
# shell snippet 6
lsblk
NAME              MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sr0                11:0    1 1024M  0 rom
vda               252:0    0 1000G  0 disk
├─vda1            252:1    0    1G  0 part /boot
├─vda2            252:2    0   63G  0 part
## rootvg-lvroot 为根目录 / 的挂载点，后面会用到
│ ├─rootvg-lvroot 253:0    0   30G  0 lvm  /
│ ├─rootvg-lvswap 253:1    0   12G  0 lvm  [SWAP]
│ ├─rootvg-lvvar  253:2    0   10G  0 lvm  /var
│ ├─rootvg-lvhome 253:3    0    1G  0 lvm  /home
│ └─rootvg-lvopt  253:4    0   10G  0 lvm  /opt
└─vda3            252:3    0  936G  0 part
```

初始化物理卷

```sh
# shell snippet 7
pvcreate /dev/vda3     # /dev/vda3 详见 shell snippet 6
Physical volume "/dev/vda3" successfully created.
```

查看物理卷是否初始化成功， 可见新增的分区为936GB，   

原来磁盘分区` /dev/vda2` 的 `VG Name` 为 `rootvg`  

```sh
# shell snippet 8
pvdisplay
  --- Physical volume ---
  PV Name               /dev/vda2
  # rootvg 后面会用到
  VG Name               rootvg
  PV Size               <63.00 GiB / not usable 3.00 MiB
  Allocatable           yes (but full)
  PE Size               4.00 MiB
  Total PE              16127
  Free PE               0
  Allocated PE          16127
  PV UUID               OO1vVY-91Ed-vECv-fSE9-85OY-iei2-wtoGvP
 # /dev/vda3 后面会用到
  "/dev/vda3" is a new physical volume of "936.00 GiB"
  --- NEW Physical volume ---
  PV Name               /dev/vda3
  VG Name
  PV Size               936.00 GiB
  Allocatable           NO
  PE Size               0
  Total PE              0
  Free PE               0
  Allocated PE          0
  PV UUID               gUNmXK-osdS-s9NJ-cXp1-Yd1X-fX4x-KvQBwi
```

将物理卷/dev/vda3的容量添加到物理组，即当前系统的 `VG Name`

```sh
# shell snippet 9
vgextend rootvg /dev/vda3    # rootvg 详见 shell snippet 8
Volume group "rootvg" successfully extended
```

修改逻辑卷的容量，将逻辑卷的容量增加

```sh
# shell snippet 10
lvresize -L +900G /dev/rootvg/lvroot  # rootvg/lvroot  详见 shell snippet 6
  Size of logical volume rootvg/lvroot changed from <30.00 GiB (7679 extents) to <930.00 GiB (238079 extents).
  Logical volume rootvg/lvroot successfully resized
```

扩展XFS文件系统， 执行

```sh
# shell snippet 11
xfs_growfs /dev/rootvg/lvroot   # rootvg/lvroot  详见 shell snippet 6
meta-data=/dev/mapper/rootvg-lvroot isize=512    agcount=4, agsize=1965824 blks
         =                       sectsz=512   attr=2, projid32bit=1
         =                       crc=1        finobt=0 spinodes=0
data     =                       bsize=4096   blocks=7863296, imaxpct=25
         =                       sunit=0      swidth=0 blks
naming   =version 2              bsize=4096   ascii-ci=0 ftype=1
log      =internal               bsize=4096   blocks=3839, version=2
         =                       sectsz=512   sunit=0 blks, lazy-count=1
realtime =none                   extsz=4096   blocks=0, rtextents=0
data blocks changed from 7863296 to 243792896
```

此时查看系统磁盘使用情况

```sh
# shell snippet 12
df -h
Filesystem                 Size  Used Avail Use% Mounted on
/dev/mapper/rootvg-lvroot  930G   18G  913G   2% /
********************************************
tmpfs                      3.2G     0  3.2G   0% /run/user/0
```

# 挂载数据盘

```
# 能够看到物理磁盘， 比如 sdbc
lsblk
# mount
mount /dev/sdc1 /data
```
此时已挂载完毕， 通过 `df -h` 能够看到磁盘空间  
但是重启后挂载的盘会看不到，还需要执行   
```
vi /etc/fstab
/dev/sdc /data ext3 defaults 0 0
```
其中， ext3 通过执行 `blkid /dev/sdc`  获取
