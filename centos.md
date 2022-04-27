# 1. network
解决centos minimal 安装无法上网的问题

```sh
cd /etc/sysconfig/network-scripts/
vi ifcfg-enp0s3         //ifcfg-之后为网卡名称

```
将最后一行`ONBOOT=NO` 修改为 `ONBOOT=YES`

# 2. install  docker in centos offline

## 2.1 update
```sh
docker pull centos          //centos8
yum update
```
### 2.1.1 centos8 

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
### 2.1.2 centos7

```sh
yum-config-manager --add-repo http://download.docker.com/linux/centos/docker-ce.repo
or
yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
```
### 2.1.3 查看可以安装的docker 版本

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
遇到 ``

## 2.2 centos7 minimal VirtualBox Insert Guest Addtions CD Image

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

# 3. install mysql8 on centos7.9 minimal

## 3.1 install

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

## 3.2 config

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

## 3.3  start
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

# 4. FAQ

yum install ***.rpm 提示 `A conflicts with file from package B`

两种方法

(1)  rpm -ivh --replacefiles  A.rpm, 

(2) yum -y remove B 然后再安装A



提示 `A is obsoleted by B`

yum -y remove  A, 然后再安装B



# 6. redis

```sh
yum -y install gcc gcc-c++ libstdc++-devel
tar -zxf  redis-stable.tar.gz
cd redis-stable
cd deps
cd jemalloc/
make MALLOC=libc
make install
redis-server --version
Redis server v=6.2.6 sha=00000000:0 malloc=libc bits=64 build=bcb67d162ef0f4f8

```

设置密码并启动

```sh
vi ./redis.conf
# 设置 requirepass yourpassword
redis-server /etc/redis.conf &
```

# 7. nc

## 7.1 安装nc工具



```sh
yum install nc -y
```

## 7.2 端口探测

### 7.2.1 TCP端口探测

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

### 7.2.2 UDP端口探测

使用方法：

```sh
nc -u -w 1 IP port < /dev/null && echo "udp port ok"
```

举例，对方UDP端口可连接

```sh
nc -u -w 1 192.168.21.17 34567 < /dev/null && echo -e "udp port ok"
udp port ok
```

# 8. iptables

## 8.1 basic

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
iptables -F
```

清空所有自定义规则

```bash
iptables -X
```

所有计数器归0

```bash
iptables -Z
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

## 8.2 example

假设希望仅仅允许 111.111.0.0/16 IP段ssh登录，而禁止其他所有ip登录。

最简单的命令是：

```sh
# 禁止所有对22端口的入访问
iptables -I INPUT -p tcp --dport 4243 -j DROP
# 允许111.111.0.0/16的访问
iptables -I INPUT -s 111.111.0.0/16 -p tcp --dport 4243 -j ACCEPT
```



