# 1. Introduction to docker

## 1.1 Install docker

download docker desktop from [docker.com](https://www.docker.com/get-started)

| CMD | NOTE |
| --- |  --- |
| yum install docker -y     | setup docker      |
| dockerd &                 | startup dockerd   |
| docker pull centos        | pull centos image |  

## 1.2 start

执行
```
docker images
```
看到  

| REPOSITORY | TAG | IMAGE ID | CREATED | SIZE |
|     ---    | --- |   ---    |   ---   |  --- |
| docker.io/centos | latest | 9f38484d220f | 13 days ago | 202 MB |

执行

```
docker run -dit --name test image_id
docker ps
```

看到

| CONTAINER ID | IMAGE | COMMAND | CREATED | STATUS | PORTS | NAMES |
|    ---       |  ---  |   ---   |  ---    |  ---   |  ---  |  ---  |
| 11bd69099a06 | 9f38484d220f | "/bin/bash" | 10 minutes ago | Up 10 minutes |  | hardcore_curie |
执行

| CMD | NOTE |
| --- | ---  |
| docker rename hardcore_curie test | 重命名容器 |
| docker tag img_id name            | 重命名镜像(rename image) |
| docker exec -it test bash         | 进入容器   |
| vi /root/.bashrc                  | 配置环境变量，重新进入容器依然有效 |
| export PATH=$PATH:/opt/jre        | 配置 java 环境变量 |
| exit                              | 退出容器  |
docker cp jre.tar.gz test:/opt      # 将容器外的文件拷贝到容器里

## 1.3 生成新的 image

提交 container 生成新的 image

| CMD | NOTE |
| --- | ---  |
| docker ps | 获取 CONTAINER ID |
| docker commit container_id richard/test(repository column) | 提交更改，生成新的镜像 |
| docker images | 获取 IMAGE ID |
|docker rmi  image_id | 删除 image |

## 1.4 导出及导入 image
### 1.4.1 导出tar
| CMD | NOTE |
| --- |  --- |
| docker images | 获取 REPOSITORY |
| docker save richard/test -o ./test.tar | 导出为 tar 包 |
| docker load -i ./test.tar              | 导入 tar 包 |

### 1.4.2 导出img文件
| CMD | NOTE |
| --- | ---  |
| docker images | get image id |
| docker save 62cfce4d2e9a > /opt/aaa.img | output img file |
| `docker load < ~/images/aaa.img` | load img file |


## 1.5 端口和目录映射
执行端口映射时，会调用 docker-proxy 命令，为操作系统创建软链  

| CMD | NOTE |
| --- |  --- |
| cat /usr/lib/systemd/system/docker.service \ grep proxy | 查找安装目录 |
| ln -s /usr/libexec/docker/docker-proxy-current /usr/bin/docker-proxy | 建立软链 |
| docker run -dit -p 9088:9088 image bash | 启动 |
| docker run -dit -v /hostdir:/containerdir --name test repository_id | 目录映射 |

## 1.6 修改默认镜像存储目录

CentOS 下 docker 默认的存储路径在 /var/lib/docker下面。 
``
docker info | grep dir -i
```
修改docker的systemd的 docker.service的配置文件
不知道 配置文件在哪里可以使用systemd 命令显示一下.  

```
systemctl disable docker
systemctl enable docker
显示结果
Created symlink from /etc/systemd/system/multi-user.target.wants/docker.service to /usr/lib/systemd/system/docker.service.
i```
show

| CMD | NOTE |
| --- |  --- |
| vim /usr/lib/systemd/system/docker.service | 修改配置文件 |
| ExecStart=/usr/bin/dockerd --graph /data/docker | 在里面的EXECStart的后面增加 --graph /data/docker |
| systemctl disable docker | disable |
| systemctl enable docker  | enable |
| systemctl daemon-reload  | reload |
| systemctl start docker   | start |

## 1.7 限制 container 使用的 CPU 和 内存

| CMD | NOTE |
| --- | ---  |
| docker run -dit --rm --cpuset-cpus="1,3" -m=2g 9a5f12155efd bash | 限制使用编号为1，3的 CPU， 内存限制使用 2GB |
| yum install -y stress | 安装压力工具 |
| stress -c 8 | 启动 8 个任务不停地执行 sqrt() |
| top           | 按 1 键，查看各个 CPU 的利用率，验证 CPU 限制是否生效 |
| docker stats | 查看  MEM USAGE / LIMIT ，验证配置是否生效 |

## 1.8 docker network
### 1.8.1 为容器设置固定的 IP 地址

```
启动Docker容器的时候，使用默认的网络是不支持指派固定IP的，如下
docker run -itd --net bridge --ip 172.17.0.10 centos:latest /bin/bash
6eb1f228cf308d1c60db30093c126acbfd0cb21d76cb448c678bab0f1a7c0df6
docker: Error response from daemon: User specified IP address is supported on user defined networks only.
```
| CMD | NOTE |
| --- |  --- |
| docker network create --subnet=172.18.0.0/16 mynetwork | 创建自定义网络 |
| docker network ls | 查看自定义网络 |
| docker run -itd --name networkTest1 --net mynetwork --ip 172.18.0.2 centos:latest /bin/bash | 启动容器 |

### 1.8.2 使用host 网络模式
使用`docker network ls` 中的 host模式，容器的网络配置与宿主机完全一样，这样也不需要在做容器内外的端口映射了。
```
docker run -dit --name container_name --network host image_id
```

## 1.9 gdb in docker
linux 内核为了安全起见，采用了Seccomp(secure computing)的沙箱机制来保证系统不被破坏   
它能使一个进程进入到一种“安全”运行模式，该模式下的进程只能调用4种系统调用（system calls），  
即read(), write(), exit()和sigreturn()，否则进程便会被终止。
docker只有以--security-opt seccomp=unconfined的模式运行container才能利用GDB调试  
```
docker run --security-opt seccomp=unconfined -dit image_id
```  
## 1.10 "No manual entry for xx" in docker
By default the centos containers are built using yum's nodocs  
注释掉这个选项，重新安装 rpm 包即可  
```
docker exec -it container_id bash
vim /etc/yum.conf
```
注释掉 tsflags=nodocs  
```
#tsflags=nodocs
```
重新安装 rpm 包  
```
rpm -qa | xargs yum reinstall -y
```
退出容器, 提交修改到镜像  
```
exit
docker commit bbb046a8fefe image_repository
```
## 1.11 docker容器内设置ubuntu语言为中文
### 1.11.1 查看当前语言
`locale`
### 1.11.2 查看当前已安装的语言

`locale -a`

### 1.11.3 安装语言包
```   
apt-get install language-pack-zh-hans
locale-gen zh_CN.UTF-8
```
再次查看  
```   
locale -a
```  
### 2.11.4 添加到文件
```
echo "export LC_ALL=zh_CN.UTF-8">> /etc/profile
source /etc/profile
```
如果这里添加失败，提示没有这种语言包，退出容器，再重新进入，就可以添加了
### 1.11. 5 完成
`locale`


# 2. docker permission

## 2.1 问题描述  
在终端执行"docker version"命令，出现如下报错：

```
”Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock: Get http://%2Fvar%2Frun%2Fdocker.sock/v1.26/images/json: dial unix /var/run/docker.sock: connect: permission denied“
```
## 2.2 原因分析  

来自docker mannual：
```
Manage Docker as a non-root user

The docker daemon binds to a Unix socket instead of a TCP port. By default that Unix socket is owned by the user root and other users can only access it using sudo. The docker daemon always runs as the root user.

If you don’t want to use sudo when you use the docker command, create a Unix group called docker and add users to it. When the docker daemon starts, it makes the ownership of the Unix socket read/writable by the docker group.
```

docker进程使用 Unix Socket 而不是 TCP 端口。而默认情况下，Unix socket 属于 root 用户，因此需要 root权限 才能访问。

## 2.3 解决方法  

```
sudo groupadd docker          #添加docker用户组
sudo gpasswd -a $XXX docker   #检测当前用户是否已经在docker用户组中，其中XXX为用户名，例如我的，rd
sudo gpasswd -a $USER docker  #将当前用户添加至docker用户组
newgrp docker                 #更新docker用户组
sudo chmod a+rw /var/run/docker.sock
```

# 3. docker group 
when you run `docker ps` in Ubuntu and it says as following
```
Got permission denied while trying to connect to the Docker daemon socket at
```
则只需要将当前用户加入到docker组中即可， 执行
```
sudo gpasswd -a $USER docker
newgrp docker
```

# 4. use GUI in docker

warning：非正常操作  
在宿主机中运行

```
sudo apt-get install x11-xserver-utils
xhost +
```
拉取docker 镜像并运行
```
docker pull jess/libreoffice
docker run -d \
-v /etc/localtime:/etc/localtime:ro \
-v /tmp/.X11-unix:/tmp/.X11-unix \
-e DISPLAY=unix$DISPLAY \
-e GDK_SCALE \
-e GDK_DPI_SCALE \
--name libreoffice \
jess/libreoffice
```

# 5. Ubuntu Linux下修改docker镜像源  
## 5.1 国内亲测可用的几个镜像源  
```
Docker 官方中国区：https://registry.docker-cn.com
网易：http://hub-mirror.c.163.com
中国科技大学：https://docker.mirrors.ustc.edu.cn
阿里云：https://y0qd3iq.mirror.aliyuncs.com
```
## 5.2 修改配置文件  
 
增加Docker的镜像源配置文件 /etc/docker/daemon.json，  
如果没有配置过镜像该文件默认是不存的，在其中增加如下内容  
  
```
{
  "registry-mirrors": ["https://y0qd3iq.mirror.aliyuncs.com"]
}
```
## 5.3 restart service  
```
service docker restart
```
查看配置是否生效  

```
docker info|grep Mirrors -A 1
```
# 6. install app in ubuntu docker container
```
apt-get update
apt-get install xxx
```
