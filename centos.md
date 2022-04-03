# 1. network
解决centos minimal 安装无法上网的问题

```
cd /etc/sysconfig/network-scripts/
vi ifcfg-enp0s3         //ifcfg-之后为网卡名称

```
将最后一行`ONBOOT=NO` 修改为 `ONBOOT=YES`

# 2. install  docker in centos offline

## 2.1 update
```
docker pull centos          //centos8
yum update
```
### 2.1.1 centos8 

会出现

```
Error: Failed to download metadata for repo 'appstream': Cannot prepare internal mirrorlist: No URLs in mirrorlist
```

修改 mirror list ，执行

```
sed -i -e "s|mirrorlist=|#mirrorlist=|g" /etc/yum.repos.d/CentOS-*
sed -i -e "s|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g" /etc/yum.repos.d/CentOS-*
```
后可以正常update

只下载不安装

```
yum -y install --downloadonly --downloaddir=./
```
or

```
yum install yum-utils
yumdownloader xxxx
```
### 2.1.2 centos7

```
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

```
mkdir docker_rpm
yumdownloader --resolve  docker-ce
ls | sort | tr "\n" " " | xargs rpm -ivh

```
启动并加入开机启动
```
systemctl start docker
systemctl enable docker
```
验证安装是否成功(有client和service两部分表示docker安装启动都成功了)
```
docker version
docker info
```
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

 

