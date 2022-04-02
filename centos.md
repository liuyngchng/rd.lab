# 1. centos docker
```
docker pull centos          //centos8
yum update
```
出现

```
Error: Failed to download metadata for repo 'appstream': Cannot prepare internal mirrorlist: No URLs in mirrorlist
```

# 2. mirror list
执行

```
sed -i -e "s|mirrorlist=|#mirrorlist=|g" /etc/yum.repos.d/CentOS-*
sed -i -e "s|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g" /etc/yum.repos.d/CentOS-*
```
后可以正常update
# 3. 只下载不安装

```
yum -y install --downloadonly --downloaddir=./
```
or

```
yum install yum-utils
yumdownloader xxxx
```
# 4. install docker

```
yum-config-manager --add-repo http://download.docker.com/linux/centos/docker-ce.repo
or
yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
```
查看可以安装的docker 版本

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
yumdownloader docker-ce.19.03.13-3.el8

```
强制安装
```
rpm -ivh ***
```
