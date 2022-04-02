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
