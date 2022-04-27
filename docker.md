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

```sh
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
```sh
docker run -dit --name container_name --network host image_id
```

## 1.9 gdb in docker
linux 内核为了安全起见，采用了Seccomp(secure computing)的沙箱机制来保证系统不被破坏   
它能使一个进程进入到一种“安全”运行模式，该模式下的进程只能调用4种系统调用（system calls），  
即read(), write(), exit()和sigreturn()，否则进程便会被终止。
docker只有以--security-opt seccomp=unconfined的模式运行container才能利用GDB调试  

```sh
docker run --security-opt seccomp=unconfined -dit image_id
```
## 1.10 "No manual entry for xx" in docker
By default the centos containers are built using yum's nodocs  
注释掉这个选项，重新安装 rpm 包即可  
```sh
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
```   sh
apt-get install language-pack-zh-hans
locale-gen zh_CN.UTF-8
```
再次查看  
```   sh
locale -a
```
### 2.11.4 添加到文件
```sh
echo "export LC_ALL=zh_CN.UTF-8">> /etc/profile
source /etc/profile
```
如果这里添加失败，提示没有这种语言包，退出容器，再重新进入，就可以添加了
### 1.11. 5 完成
`locale`


# 2. docker permission

## 2.1 问题描述  
在终端执行"docker version"命令，出现如下报错：

```sh
”Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock: Get http://%2Fvar%2Frun%2Fdocker.sock/v1.26/images/json: dial unix /var/run/docker.sock: connect: permission denied“
```
## 2.2 原因分析  

来自docker mannual：
```sh
Manage Docker as a non-root user

The docker daemon binds to a Unix socket instead of a TCP port. By default that Unix socket is owned by the user root and other users can only access it using sudo. The docker daemon always runs as the root user.

If you don’t want to use sudo when you use the docker command, create a Unix group called docker and add users to it. When the docker daemon starts, it makes the ownership of the Unix socket read/writable by the docker group.
```

docker进程使用 Unix Socket 而不是 TCP 端口。而默认情况下，Unix socket 属于 root 用户，因此需要 root权限 才能访问。

## 2.3 解决方法  

```sh
sudo groupadd docker          #添加docker用户组
sudo gpasswd -a $XXX docker   #检测当前用户是否已经在docker用户组中，其中XXX为用户名，例如我的，rd
sudo gpasswd -a $USER docker  #将当前用户添加至docker用户组
newgrp docker                 #更新docker用户组
sudo chmod a+rw /var/run/docker.sock
```

# 3. docker group 
when you run `docker ps` in Ubuntu and it says as following
```sh
Got permission denied while trying to connect to the Docker daemon socket at
```
则只需要将当前用户加入到docker组中即可， 执行
```sh
sudo gpasswd -a $USER docker
newgrp docker
```

# 4. use GUI in docker

warning：非正常操作  
在宿主机中运行

```sh
sudo apt-get install x11-xserver-utils
xhost +
```
拉取docker 镜像并运行
```sh
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
```sh
Docker 官方中国区：https://registry.docker-cn.com
网易：http://hub-mirror.c.163.com
中国科技大学：https://docker.mirrors.ustc.edu.cn
阿里云：https://y0qd3iq.mirror.aliyuncs.com
```
## 5.2 修改配置文件  

增加Docker的镜像源配置文件 /etc/docker/daemon.json，  
如果没有配置过镜像该文件默认是不存的，在其中增加如下内容  

```sh
{
  "registry-mirrors": ["https://y0qd3iq.mirror.aliyuncs.com"]
}
```
## 5.3 restart service  
```sh
service docker restart
```
查看配置是否生效  

```sh
docker info|grep Mirrors -A 1
```
# 6. install app in ubuntu docker container
```sh
apt-get update
apt-get install xxx
```



# 7. docker-compose

## 7.1 setup

```sh
sudo curl -L https://github.com/docker/compose/releases/download/1.21.2/docker-compose-$(uname -s)-$(uname -m) -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
docker-compose -v
```



## 7.2 run



应用打包

```sh
mvn clean package
```

创建 Dockerfile

```sh
touch Dockerfile
vi Dockerfile
# 内容如下
FROM java:8
VOLUME /tmp
ADD docker-demo-0.0.1-SNAPSHOT.jar app.jar
RUN bash -c 'touch /app.jar'
EXPOSE 9000
ENTRYPOINT ["java","-Djava.security.egd=file:/dev/./urandom","-jar","app.jar"]
```

创建文件 docker-compose.yml， 8080为应用端口

```sh
touch docker-compose.yml
# 内容如下
version: '2' # 表示该 Docker-Compose 文件使用的是 Version 2 file
services:
  docker-demo:  # 指定服务名称
    build: .  # 指定 Dockerfile 所在路径
    ports:    # 指定端口映射
      - "9000:8080"
```

在 docker-compose.yml 所在路径下执行该命令 Compose 就会自动构建镜像并使用镜像启动容器

```sh
docker-compose up
docker-compose up -d  // 后台启动并运行容器
```

镜像服务器地址可以在 `docker-compose.yml` 中配置。



# 8. centOS7 离线安装docker



## 8.1 docker包下载地址

```sh
#下载docker-20.10.0包
wget https://download.docker.com/linux/static/stable/x86_64/docker-20.10.0.tgz
wget https://download.docker.com/linux/static/stable/x86_64/docker-17.12.1-ce.tgz

#上传到Centos系统/data/目录,如
scp docker-20.10.0.tgz root@192.168.0.5:/data/

#进入data目录,解压docker包
cd /data
tar -zxvf docker-20.10.0.tgz

#将解压出来的docker文件内容移动到 /usr/bin/ 目录下
cp docker/* /usr/bin/

#查看docker版本
docker version

#查看docker信息
docker info
```

## 8.2 配置Docker开机自启动服务

#添加docker.service文件

vi /etc/systemd/system/docker.service

#按i插入模式,复制如下内容:

```sh
[Unit]
Description=Docker Application Container Engine
Documentation=https://docs.docker.com
After=network-online.target firewalld.service
Wants=network-online.target
  
[Service]
Type=notify
# the default is not to use systemd for cgroups because the delegate issues still
# exists and systemd currently does not support the cgroup feature set required
# for containers run by docker
#ExecStart=/usr/bin/dockerd

# drivermanage 使用overlay2 ，需要配置 /etc/docker/daemon.json一起使用， 详细选择见
# docker 官网 https://docs.docker.com/storage/storagedriver/select-storage-driver/
ExecStart=/usr/bin/dockerd --graph=/data/docker --api-cors-header=*
# drivermanage 使用devicemapper
#ExecStart=/usr/bin/dockerd --graph=/data/docker -H tcp://0.0.0.0:4243 -H unix://var/run/docker.sock  --insecure-registry  dev.kmx.k2data.com.cn:5001 --storage-driver=devicemapper --api-cors-header=*
ExecReload=/bin/kill -s HUP $MAINPID
# Having non-zero Limit*s causes performance problems due to accounting overhead
# in the kernel. We recommend using cgroups to do container-local accounting.
LimitNOFILE=infinity
LimitNPROC=infinity
LimitCORE=infinity
# Uncomment TasksMax if your systemd version supports it.
# Only systemd 226 and above support this version.
#TasksMax=infinity
TimeoutStartSec=0
# set delegate yes so that systemd does not reset the cgroups of docker containers
Delegate=yes
# kill only the docker process, not all processes in the cgroup
KillMode=process
# restart the docker process if it exits prematurely
Restart=on-failure
StartLimitBurst=3
StartLimitInterval=60s
  
[Install]
WantedBy=multi-user.target
```

 添加配置文件， 注意，daemon.json中配置的参数不能与 /etc/systemd/system/docker.service 重复，否则启动服务会报 `start request repeated too quickly for docker.service`

```sh
touch /etc/docker/daemon.json
vi /etc/docker/daemon.json
# 添加如下内容
{
 
 "debug": true,
 "live-restore": false,
 "hosts":["unix:///var/run/docker.sock","tcp://0.0.0.0:4243"],
 "storage-driver": "overlay2",
 "storage-opts": [
    "overlay2.override_kernel_check=true"
  ],
  "insecure-registry": [ "hostName:port", "IP:port"] 
}
```



启动服务

```sh
#添加文件可执行权限
chmod +x /etc/systemd/system/docker.service

#重新加载配置文件
systemctl daemon-reload

#启动Docker
systemctl start docker

#查看docker启动状态
systemctl status docker

#查看启动容器
docker ps

#设置开机自启动
systemctl enable docker.service

#查看docker开机启动状态 enabled:开启, disabled:关闭
systemctl is-enabled docker.service

https://docs.docker.com/storage/storagedriver/select-storage-driver/

https://blog.csdn.net/doctorone/article/details/88536385
device-mapper :需要 

yum install -y yum-utils device-mapper-persistent-data lvm2

/usr/bin/dockerd --graph=/data/docker -H tcp://0.0.0.0:4243 -H unix:///var/run/docker.sock  --insecure-registry  dev.kmx.k2data.com.cn:5001 --storage-driver=devicemapper --api-cors-header=*
```



# 9. set proxy for docker in ubuntu

## 9.1 docker service
https://docs.docker.com/network/proxy/

```sh
sudo vi /lib/systemd/system/docker.service
# 添加如下内容
[Service]
Type=notify
# the default is not to use systemd for cgroups because the delegate issues still
# exists and systemd currently does not support the cgroup feature set required
# for containers run by docker
Environment=HTTP_PROXY=http://xxx.com:xxx
Environment=HTTPS_PROXY=http://xxx.com:xxx
Environment=NO_PROXY=*.xxx.com
```
## 9.2 docker config
run
```
sudo mkdir -p /etc/systemd/system/docker.service.d
sudo vi /etc/systemd/system/docker.service.d/http-proxy.conf
```
内容如下
```
[Service]
Environment="HTTP_PROXY=http://账号:密码@服务器:端口"
Environment="HTTPS_PROXY=http://账号:密码@服务器:端口"
Environment="NO_PROXY=localhost,127.0.0.1"
```

systemctl daemon-reload

systemctl restart docker

# 10. docker push
## 10.1 查看镜像

查看仓库里的镜像清单

```sh
curl http://IP:port/v2/_catalog
```

获取某个镜像的标签列表， image_name 为 镜像清单里的名称

```sh
curl -XGET http://IP:port/v2/image_name/tags/list

{"errors":[{"code":"NAME_UNKNOWN","message":"repositoryname not known toregistry","detail":{"name":"image_name"}}]}
```


## 10.2 docker pull

在本地登录到远程仓库：
```
docker login --username=[用户名] -p=[密码]  远程ip:端口
```
本地镜像打标签,   [远程ip:端口/自定义路径/*]:[版本号]  就是：[标签名:版本号]
```
docker tag [镜像id] [远程ip:端口/自定义路径/*]:[版本号]
```
推送
```
docker push [远程ip:端口/自定义路径/*]:[版本号]
```
当显示
```
The push refers to repository [IP:port/path]
Get https://IP:port/v2/: Service Unavailable
```
原因为 docker镜像仓库暂不支持 https
在“/etc/docker/“目录下, 创建”daemon.json“文件(如果有的话直接覆盖)。  
在文件中写入
```
{ "insecure-registries":["IP:port"] }
```
执行
```sh
systemctl daemon-reload
systemctl restart docker.service 
```
## 10.3 docker 删除私有仓库中的镜像
查看某个镜像的sha256值：
```
curl -v -s "127.0.0.1:5000/v2/nginx/manifests/1.7.9" 2>&1 | grep Docker-Content-Digest | awk '{print ($3)}'
```

删除私有仓库的镜像
```
$ curl -I -X DELETE http://127.0.0.1:5000/v2/fbgweb/manifests/sha256:6a67ba482a8dd4f8143ac96b1dcffa5e45af95b8d3e37aeba72401a5afd7ab8e
```
