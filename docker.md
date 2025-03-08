# 1. Introduction to docker

##  1.1 Install docker

​		download docker desktop from [docker.com](https://www.docker.com/get-started)

| CMD | NOTE |
| --- |  --- |
| yum install docker -y     | setup docker      |
| dockerd &                 | startup dockerd   |
| docker pull centos        | pull centos image |  

##  1.2 start up

​		ubuntu

​		systemctl start docker

​		执行
```
docker images
```
​		看到  

| REPOSITORY | TAG | IMAGE ID | CREATED | SIZE |
|     ---    | --- |   ---    |   ---   |  --- |
| docker.io/centos | latest | 9f38484d220f | 13 days ago | 202 MB |

​		执行

```
docker run -dit --name test image_id
docker ps
```

​		看到

| CONTAINER ID | IMAGE | COMMAND | CREATED | STATUS | PORTS | NAMES |
|    ---       |  ---  |   ---   |  ---    |  ---   |  ---  |  ---  |
| 11bd69099a06 | 9f38484d220f | "/bin/bash" | 10 minutes ago | Up 10 minutes |  | hardcore_curie |
执行

| CMD | NOTE |
| --- | ---  |
| docker rename hardcore_curie test | 重命名容器 |
| docker tag img_id name(repository:tag) | 重命名镜像(rename image)，生成一条新的记录 |
| docker tag SOURCE_IMAGE[:TAG] TARGET_IMAGE[:TAG] | 重命名镜像(rename image)，更改原始的记录 |
| docker exec -it test bash         | 进入容器   |
| vi /root/.bashrc                  | 配置环境变量，重新进入容器依然有效 |
| export PATH=$PATH:/opt/jre        | 配置 java 环境变量 |
| exit                              | 退出容器  |
|docker cp jre.tar.gz test:/opt      # 将容器外的文件拷贝到容器里||

##  1.3 生成新的 image

​		提交 container 生成新的 image

| CMD | NOTE |
| --- | ---  |
| docker ps | 获取 CONTAINER ID |
| docker commit container_id richard/test(repository column):tag | 提交更改，生成新的镜像 |
| docker images | 获取 IMAGE ID |
|docker rmi  image_id | 删除 image |
|docker images \| grep '<none>' \| awk -F ' ' '{print $3}' \| xargs docker rmi | 删除 tag 为 <none> 的image |

##  1.4 导出及导入 image
###  1.4.1 导出tar

| CMD | NOTE |
| --- |  --- |
| docker images | 获取 REPOSITORY |
| docker save richard/test -o ./test.tar | 导出为 tar 包 |
| `docker save myimage:1.0 |  gzip > myimage_1.0.tar.gz` | 导出为 tar.gz包 |
| docker load -i ./test.tar              | 导入 tar 包 |

###  1.4.2 导出img文件
| CMD | NOTE |
| --- | ---  |
| docker images | get image id |
| docker save 62cfce4d2e9a > /opt/aaa.img | output img file |
| `docker load < ~/images/aaa.img` | load img file |


##  1.5 端口和目录映射
​		执行端口映射时，会调用 docker-proxy 命令，为操作系统创建软链  

| CMD | NOTE |
| --- |  --- |
| cat /usr/lib/systemd/system/docker.service \ grep proxy | 查找安装目录 |
| ln -s /usr/libexec/docker/docker-proxy-current /usr/bin/docker-proxy | 建立软链 |
| docker run -dit -p host_port:container_port image bash | 启动 |
| docker run -dit -v host_dir:container_dir --name test repository_id | 目录映射 |
| docker run -u username | 指定运行镜像所使用的用户 |
| docker run -it  --entrypoint="/bin/bash" | 覆盖Dockerfile中ENTRYPOINT设置的命令 |

## 1.6 修改默认镜像存储目录

​		CentOS 下 docker 默认的存储路径在 /var/lib/docker下面。 

```sh
docker info | grep dir -i
```
​		修改docker的systemd的 docker.service的配置文件，查找配置文件位置可以使用systemd 命令显示一下.  

```sh
# 查找 service 文件位置  Loaded: loaded 
systemctl status docker  
systemctl disable docker
# 设置开机启动docker
systemctl enable docker

```
显示结果
Created symlink from /etc/systemd/system/multi-user.target.wants/docker.service to /usr/lib/systemd/system/docker.service.
i```
show

| CMD | NOTE |
| --- |  --- |
| vim /usr/lib/systemd/system/docker.service | 修改配置文件 |
| ExecStart=/usr/bin/dockerd --graph /data/docker | 在里面的EXECStart的后面增加 --graph /data/docker |
| systemctl disable docker | 设置开机不启动docker |
| systemctl enable docker  | 设置开机启动docker |
| systemctl daemon-reload  | reload |
| systemctl start docker   | start |

##  1.7 限制 container 使用的 CPU 和 内存

| CMD | NOTE |
| --- | ---  |
| docker run -dit --rm --cpuset-cpus="1,3" -m=2g 9a5f12155efd bash | 限制使用编号为1，3的 CPU， 内存限制使用 2GB |
| yum install -y stress | 安装压力工具 |
| stress -c 8 | 启动 8 个任务不停地执行 sqrt() |
| top           | 按 1 键，查看各个 CPU 的利用率，验证 CPU 限制是否生效 |
| docker stats | 查看  MEM USAGE / LIMIT ，验证配置是否生效 |

##  1.8 docker network
###  1.8.1 为容器设置固定的 IP 地址

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

###  1.8.2 使用host 网络模式
使用`docker network ls` 中的 host模式，容器的网络配置与宿主机完全一样，这样也不需要在做容器内外的端口映射了。
```sh
docker run -dit --name container_name --network host image_id
```

##  1.9 gdb in docker
linux 内核为了安全起见，采用了Seccomp(secure computing)的沙箱机制来保证系统不被破坏   
它能使一个进程进入到一种“安全”运行模式，该模式下的进程只能调用4种系统调用（system calls），  
即read(), write(), exit()和sigreturn()，否则进程便会被终止。
docker只有以--security-opt seccomp=unconfined的模式运行container才能利用GDB调试  

```sh
docker run --security-opt seccomp=unconfined -dit image_id
```
##  1.10 "No manual entry for xx" in docker
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
##  1.11 docker容器内设置ubuntu语言为中文
###  1.11.1 查看当前语言
`locale`
###  1.11.2 查看当前已安装的语言

`locale -a`

###  1.11.3 安装语言包
```   sh
apt-get install language-pack-zh-hans
locale-gen zh_CN.UTF-8
```
再次查看  
```   sh
locale -a
```
###  1.11.4 添加到文件
```sh
echo "export LC_ALL=zh_CN.UTF-8">> /etc/profile
source /etc/profile
```
如果这里添加失败，提示没有这种语言包，退出容器，再重新进入，就可以添加了
###  1.11.5 完成
`locale`

## 1.12 修改docker容器中的系统时间

若需要在docker 中修改系统时间，则在执行 docker run 时需要加上参数
```
docker run --cap-add SYS_TIME
```


# 2. docker permission

##  2.1 问题描述  
在终端执行"docker version"命令，出现如下报错：

```sh
”Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock: Get http://%2Fvar%2Frun%2Fdocker.sock/v1.26/images/json: dial unix /var/run/docker.sock: connect: permission denied“
```
##  2.2 原因分析  

来自docker mannual：
```sh
Manage Docker as a non-root user

The docker daemon binds to a Unix socket instead of a TCP port. By default that Unix socket is owned by the user root and other users can only access it using sudo. The docker daemon always runs as the root user.

If you don’t want to use sudo when you use the docker command, create a Unix group called docker and add users to it. When the docker daemon starts, it makes the ownership of the Unix socket read/writable by the docker group.
```

docker进程使用 Unix Socket 而不是 TCP 端口。而默认情况下，Unix socket 属于 root 用户，因此需要 root权限 才能访问。

## 2.3 解决方法  

```sh
sudo groupadd docker			#添加docker用户组
sudo gpasswd -a $USER docker	#将当前用户添加至docker用户组
newgrp docker					#更新docker用户组
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
##  5.3 restart service  
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



# 6.1 docker-compose

## 6.1.1 setup

```sh
sudo curl -L https://github.com/docker/compose/releases/download/1.21.2/docker-compose-$(uname -s)-$(uname -m) -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
docker-compose -v
```

##  6.1.2 run

应用打包

```shell
mvn clean package
```

创建 Dockerfile

```sh
touch Dockerfile
vi Dockerfile
# 内容如下
# 初始拉取的镜像名称及其版本
FROM java:8
VOLUME /tmp
# 将容器内的目录切换到 /opt 下
WORKDIR /opt/
# 将宿主机当前目录下的 docker-demo-0.0.1-SNAPSHOT.jar 添加到容器当前目录下，并重命名为 app.jar
ADD docker-demo-0.0.1-SNAPSHOT.jar app.jar
# 设置文件编码
ENV LC_ALL "C.UTF-8"
ENV LANG "C.UTF-8"
# 设置时区
RUN bash -c 'rm /etc/localtime'
RUN bash -c 'ln -s /usr/share/zoneinfo/Asia/Shanghai /etc/localtime'
RUN bash -c 'touch /app.jar'
# 这个告诉查看Dockerfile的读者，此应用需要暴露 9000 端口
EXPOSE 9000
# 设置 entrypoint 也可是使用 CMD
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

#  6.2 离线安装docker

以下内容在 CentOS 7.4 、RHEL 7.4 和  docker 23.0.6 下进行了验证 

## 6.2.1 下载静态编译包

```sh
#下载docker-20.10.0包
wget https://download.docker.com/linux/static/stable/x86_64/docker-20.10.0.tgz
wget https://download.docker.com/linux/static/stable/x86_64/docker-17.12.1-ce.tgz

#上传到Centos系统/data/目录,如
scp docker-20.10.0.tgz root@192.168.0.5:/data/

#进入data目录,解压docker包
cd /data
tar -zxvf docker-20.10.0.tgz
graph
#将解压出来的docker文件内容移动到 /usr/bin/ 目录下
cp docker/* /usr/local/bin/

#查看docker版本
docker version

#查看docker信息
docker info
```

##  6.2.2 配置系统服务

#添加docker.service文件

```sh
# root 用户
su
vi /etc/systemd/system/docker.service
# vi /usr/lib/systemd/system/docker.service
# ubuntu 22.04 LTS 通过 sudo-apt install docker.io 安装的docker
vi /lib/systemd/system/docker.service
```

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
# docker 版本为 23.0.6 时，--graph 选项应替换为 --data-root
ExecStart=/usr/local/bin/dockerd --graph=/data/docker --api-cors-header=*
# drivermanage 使用devicemapper
# 若需要在当前配置文件中添加多个私有仓库，可以在 dockerd 后面通过添加多个 --insecure-registry 来解决
#ExecStart=/usr/bin/dockerd --graph=/data/docker -H tcp://0.0.0.0:4243 -H unix://var/run/docker.sock  --insecure-registry test1.com.cn:5001 --insecure-registry test2.com.cn:5001 --storage-driver=devicemapper --api-cors-header=*
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

完整的配置文件`/etc/systemd/system/docker.service ` 内容如下所示

```sh
[Unit]
Description=Docker Application Container Engine
Documentation=https://docs.docker.com
After=network-online.target firewalld.service
Wants=network-online.target
  
[Service]
Type=notify
# docker 版本为 20 时，--graph 选项应替换为 --data-root
#ExecStart=/usr/local/bin/dockerd --data-root=/data/docker --api-cors-header=*
ExecStart=/usr/local/bin/dockerd --graph=/data/docker --api-cors-header=*
ExecReload=/bin/kill -s HUP $MAINPID
LimitNOFILE=infinity
LimitNPROC=infinity
LimitCORE=infinity
Delegate=yes
KillMode=process
Restart=on-failure
StartLimitBurst=3
StartLimitInterval=60s
  
[Install]
WantedBy=multi-user.target
```

 

添加配置文件， 注意，daemon.json中配置的参数不能与 /etc/systemd/system/docker.service 重复，否则启动服务会报 `start request repeated too quickly for docker.service`,    

注意，  `"insecure-registry": [ "hostName:port", "IP:port"]`  一行的  `[ "hostName:port", "IP:port"]  `需要替换成真是的域名端口号，或者IP和端口号，是用于 docker pull， docker push 的私有registry使用的，若不需要此功能，则不配置这一项。

```sh
touch /etc/docker/daemon.json
vi /etc/docker/daemon.json
```

添加如下内容

```sh
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
sudo chmod +x /etc/systemd/system/docker.service

#重新加载配置文件
sudo systemctl daemon-reload

#启动Docker
sudo systemctl restart docker

#查看docker启动状态
systemctl status docker

#查看启动容器
docker ps



https://docs.docker.com/storage/storagedriver/select-storage-driver/

https://blog.csdn.net/doctorone/article/details/88536385
device-mapper :需要 

yum install -y yum-utils device-mapper-persistent-data lvm2
# 服务启动测试命令
/usr/local/bin/dockerd --graph=/data/docker -H tcp://0.0.0.0:4243 -H unix:///var/run/docker.sock  --insecure-registry  dev.kmx.k2data.com.cn:5001 --storage-driver=devicemapper --api-cors-header=*
```

以上配置完成后，即可配置开启自启动

```sh
#设置开机自启动
sudo systemctl enable docker.service

#查看docker开机启动状态 enabled:开启, disabled:关闭
systemctl is-enabled docker.service
```

当执行 `sudo systemctl start docker` 没有明显错误的时候，仍然无法正常启动服务时，需要杀掉2个服务，dockerd 和 containerd

```sh
ps -ef | grep docker
root     15195     1  1 16:54 ?        00:00:00 /usr/local/bin/dockerd --graph=/data/docker --api-cors-header=*
root     15270 15195  1 16:54 ?        00:00:00 containerd --config /var/run/docker/containerd/containerd.toml 
```



# 6.3 set proxy for docker in ubuntu

##  6.3.1 docker service
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
##  6.3.2 docker config
run
```sh
sudo mkdir -p /etc/systemd/system/docker.service.d
sudo vi /etc/systemd/system/docker.service.d/http-proxy.conf
```
内容如下
```sh
[Service]
Environment="HTTP_PROXY=http://账号:密码@服务器:端口"
Environment="HTTPS_PROXY=https://账号:密码@服务器:端口"
Environment="NO_PROXY=localhost,127.0.0.1"
```

systemctl daemon-reload

systemctl restart docker

# 7. docker push
##  7.1 查看镜像

查看仓库里的镜像清单

```sh
curl http://IP:port/v2/_catalog
```

获取某个镜像的标签列表， image_name 为 镜像清单里的名称

```sh
curl -XGET http://IP:port/v2/image_name/tags/list
curl --noproxy '*' -XGET --tlsv1 -k https://hub.docker.com/v2/mysql/tags/list

{"errors":[{"code":"NAME_UNKNOWN","message":"repositoryname not known toregistry","detail":{"name":"image_name"}}]}
```


##  7.2 docker pull

在本地登录到远程仓库：
```sh
docker login --username=[用户名] -p=[密码]  远程ip:端口
```
本地镜像打标签,   [远程ip:端口/自定义路径/*]:[版本号]  就是：[标签名:版本号]
```sh
docker tag [镜像id] [远程IP:端口/自定义路径/*]:[版本号]
```
推送
```sh
docker push [远程IP:端口/自定义路径/*]:[版本号]
```
当显示
```sh
The push refers to repository [IP:port/path]
Get https://IP:port/v2/: Service Unavailable
```
原因为 docker镜像仓库暂不支持 https
在“/etc/docker/“目录下, 创建”daemon.json“文件(如果有的话直接覆盖)。  
在文件中写入

```sh
{ "insecure-registries":["IP:port"] }
```
执行
```sh
systemctl daemon-reload
systemctl restart docker.service 
```
如果是windows docker， 则在图形化界面中的 docker engine的配置 JSON中，加入以上配置，然后重启即可.

##   7.3 docker 删除私有仓库中的镜像

首先， 镜像库服务器上需要进行配置，更改registry容器内/etc/docker/registry/config.yml文件

```yaml
storage:
  delete:
    enabled: true
```

查看某个镜像的sha256值：

```sh
curl  --header "Accept: application/vnd.docker.distribution.manifest.v2+json" -I -X GET https://<仓库地址>/v2/<镜像名>/manifests/<tag>
# 获取 digest_hash
Docker-Content-Digest: <digest_hash>
#例如 
curl --header "Accept: application/vnd.docker.distribution.manifest.v2+json" -I -X GET reg.docker.com:5001/v2/rd/centos8_es7.6.1/manifests/1.0.0  
# 获取到了结果
HTTP/1.1 200 OK
Content-Length: 742
Content-Type: application/vnd.docker.distribution.manifest.v2+json
Docker-Content-Digest: sha256:0c31ab1a4ac0ebeec6979a3f1524b646539b98662d81ae3de9a092f8b214e5e2
Docker-Distribution-Api-Version: registry/2.0
Etag: "sha256:0c31ab1a4ac0ebeec6979a3f1524b646539b98662d81ae3de9a092f8b214e5e2"
X-Content-Type-Options: nosniff
Date: Thu, 28 Apr 2022 02:14:01 GMT
```

可见 digest_hash 为 sha256:0c31ab1a4ac0ebeec6979a3f1524b646539b98662d81ae3de9a092f8b214e5e2

执行删除， 

```sh
$ curl -I -X DELETE <protocol>://<registry_host>/v2/<repo_name>/manifests/<digest_hash>
# 例如
$ curl -I -X DELETE https://10.109.252.221:5000/v2/wordpress/manifests/sha256:0c31ab1a4ac0ebeec6979a3f1524b646539b98662d81ae3de9a092f8b214e5e2
```



# 8. docker build

创建Dockerfile

```sh
mkdir docker.test
cd docker.test
touch testDockerfile
vi testDockerfile
```

文件内容如下：

```sh
# docker pull 需要拉取的镜像名称以及tag
FROM centos 
# 挂载的宿主机的目录
VOLUME /tmp
WORKDIR /opt
ADD ./test.sh  a.sh
RUN bash -c 'pwd'
RUN bash -c 'ls -lh'
ENTRYPOINT ["/usr/bin/sh", "./a.sh"]
```

run,  -f 表示要读取的Dockfile的位置， -t 镜像名称 : tag

```sh
docker build -f ./testDockerfile ./ -t myimg:1.0.0
```

# 9. docker 写宿主机权限问题

* docker 中的root 用户 和宿主机的 root 用户的uid是相同的，所以只要宿主机root用户有权限的，那么docker容器内执行也没有问题。

```sh
rd@rd-ubt:/$ docker exec -it test bash
[root@26484718385f /]# id root
uid=0(root) gid=0(root) groups=0(root)
[root@26484718385f /]# exit
exit
rd@rd-ubt:/$ id root
#  可见容器内外的root用户的uid是相同的
uid=0(root) gid=0(root) groups=0(root)
rd@rd-ubt:/$ 
```

* 涉及到docker 里的非root用户， 在写挂载的宿主机的目录权限时，需要进行如下处理

```sh
# 进入docker容器
docker exec -it xxxxx bash
# 获取docker 里的 userId，一定是进入docker里头获取的用户的id。在宿主机上面创建的同名用户与docker中同名用户的uid 不同
id userName
# 退出docker 容器， 1234为docker里用户的id，5678为的docker里头用户的groupid
exit
# 在宿主机上授权
chown -R 1234：5678 dir
```

# 10. docker TLS

##  10.1 生成 CA 公私钥

​		生成 CA 认证机构的 key 和 证书签名请求（CSR）

```sh
sudo mkdir -p /data/ssl/srv
cd /data/ssl/srv
# 生成私钥(PEM RSA private key)
openssl genrsa -aes256 -out ca-key.pem 4096
# 输入密码 helloworld
```

​		补全CA证书信息（Certificate Signing Request）

```sh
cd /data/ssl/srv
# 生成 PEM certificate
openssl req -new -x509 -days 365 -key ca-key.pem -sha256 -out ca.pem
# 输入密码 helloworld
```

##  10.2 生成 server 证书

​		使用 CA 签署 server 的公钥，推荐使用域名（如果没有域名，采用 host 绑定的方式， IP 的方式容易失败）的方式，生成 server 端经过 CA 签名的证书。

生成 server 端的私钥以及经过 CA 签名的证书

```sh
cd /data/ssl/srv
# 生成私钥(PEM RSA private key)
openssl genrsa -out server-key.pem 4096
# 生成 PEM certificate request
openssl req -subj "/CN=my.docker.test" -sha256 -new -key server-key.pem -out server.csr
```


 		在发起请求的机器（client）的 host 里绑定域名

```sh
sudo vi /etc/hosts
123.456.789.0 my.docker.test
```

​		生成 server 端扩展配置文件

```sh
cd /data/ssl/srv
# 匹配白名单， 只允许域名为 my.docker.test， 或者 IP 为 1.2.3.4 和 2.3.4.5 的机器访问 docker-daemon 的机器
echo subjectAltName = DNS:my.docker.test, IP:1.2.3.4, IP:2.3.4.5 >> extfile.cnf
# 将Docker守护程序密钥的扩展使用属性设置为仅用于服务器身份验证
echo extendedKeyUsage = serverAuth >> extfile.cnf
```

​		生成经过 CA 签名的服务端证书

```sh
cd /data/ssl/srv
# 生成 PEM certificate
openssl x509 -req -days 365 -sha256 -in server.csr -CA ca.pem -CAkey ca-key.pem \
  -CAcreateserial -out server-cert.pem -extfile extfile.cnf
# 输入密码 helloworld
```

##  10.3 生成 client 证书

​		生成 client 端的私钥以及经过 CA 签名的证书

```sh
sudo mkdir /data/ssl/srv
cd /data/ssl/srv
# 生成 PEM RSA private key(客户端私钥 key.pem)
openssl genrsa -out key.pem 4096
# 生成 PEM certificate request
openssl req -subj '/CN=client' -new -key key.pem -out client.csr
# 创建client 端扩展配置文件
echo extendedKeyUsage = clientAuth > extfile-client.cnf
# 生成签名数据（PEM certificate）
openssl x509 -req -days 365 -sha256 -in client.csr -CA ca.pem -CAkey ca-key.pem -CAcreateserial -out cert.pem -extfile extfile-client.cnf
```

## 10.4 删除中间文件

​		删除不必要得文件

```sh
rm -v client.csr server.csr extfile.cnf extfile-client.cnf
```

​		为了了保护密钥免于意外损坏，请删除其写入权限。要使它们仅供阅读，请按以下方式更改文件模式

```sh
chmod -v 0400 ca-key.pem key.pem server-key.pem
```

​		证书可以使对外可读的，删除写入权限以防止意外损坏

```sh
chmod -v 0444 ca.pem server-cert.pem cert.pem
```

## 10.5 归集服务端证书

执行

```sh
sudo mkdir /usr/local/ca
cp server-*.pem /usr/local/ca
cp ca.pem /usr/local/ca
ls -al /usr/local/ca
.  ..  ca.pem  server-cert.pem  server-key.pem
```

## 10.6 修改Docker配置

​		Docker守护程序仅接收来自提供CA信任的证书的客户端的链接， 需要配置 

（1）CA 证书签名请求（CSR）；

（2）server端经过CA 签名的证书；

（3） server端的私钥。

```sh
# 具体需要看 server 上 docker service对应的配置
vim /lib/systemd/system/docker.service
vim /etc/systemd/system/docker.service
ExecStart=/usr/bin/dockerd --tlsverify --tlscacert=/usr/local/ca/ca.pem --tlscert=/usr/local/ca/server-cert.pem --tlskey=/usr/local/ca/server-key.pem -H tcp://0.0.0.0:2375 -H unix:///var/run/docker.sock
```

或者修改配置文件 /etc/docker/daemon.json, 添加的内容如下

```json
{
  "tls": true,
  "tlscacert": "/usr/local/ca/ca.pem",
  "tlscert": "/usr/local/ca/server-cert.pem",
  "tlskey": "/usr/local/ca/server-key.pem",
  "hosts": ["tcp://0.0.0.0:2376", "unix:///var/run/docker.sock"]
}
```



## 10.7 重启 docker

```sh
sudo systemctl daemon-reload
sudo systemctl restart docker
```

## 10.8 客户端验证

​		通过使用客户端证书，以及CA证书，验证 docker 接口，需要提供：

（1）CA 证书签名请求（CSR）；

（2）client 端经过 CA 签名的证书；

（3）client 端的私钥。

```sh
cd /data/ssl/srv
curl --cacert ./ca.pem --cert ./cert.pem --key ./key.pem  'https://IP:port/version'
```

#  11. docker registry(docker 镜像仓库)	

​		通过以下脚本，可搭建docker私有镜像仓库

```sh
# pull image
docker pull registry
# mkdir
sudo mkdir -p /data/docker/reg
#chown to current user logged in
sudo chown -R ${USER}:${USER} /data
# run container map host 5001 to container 5000
docker stop registry
docker rm registry
docker run -d \
    --restart=always \
    --name registry	\
    -p 5001:5000 \
    -v /data/docker/reg:/var/lib/registry \
    registry
# wait for service init
sleep 5
curl -XGET --noproxy '*' http://localhost:5001/v2/_catalog
# 可见 {"repositories":[]}
```

​		启用证书

```sh
docker run -d \
  --restart=always \
  --name registry \
  --memory 512m \
  -p 443:443  \
  -v /root/regimages:/var/lib/registry \
  --memory-swap -1 \
  -v /root/docker/pki:/etc/certs.d \   # 把刚才生成证书的目录挂载到 容器中
  -e REGISTRY_HTTP_ADDR=0.0.0.0:443 \
  -e REGISTRY_HTTP_SECRET=secret \
  -e REGISTRY_HTTP_TLS_CERTIFICATE=/etc/certs.d/registry.crt \  #指定了https的证书
  -e REGISTRY_HTTP_TLS_KEY=/etc/certs.d/registry_private.key \    #指定了https的私钥
  registry:2
```



# 12. build base image

```sh
mkdir test
vi hello.c
```

内容如下

```c
#include<stdio.h>
int main()
{
    printf("hello docker\n");
}
```

执行 

```sh
cd test
gcc -static hello.c
```

生成  a.out 可执行文件

```sh
cd test
vi Dockerfile
```

内容如下

```sh
from scratch
copy  a.out  /
CMD ["/a.out"]
```

执行

```sh
cd test
docker build -f ./Dockerfile ./ -t mydocker
docker images
#可见如下的镜像
REPOSITORY                         TAG       IMAGE ID       CREATED          SIZE
mydocker                           latest    8f88a6999f73   4 minutes ago    900kB
```

运行

```sh
docker run --rm mydocker
```

# 13. docker inspect

```sh
# 获取容器信息
docker ps -a	
CONTAINER ID   IMAGE     COMMAND       CREATED       STATUS                   PORTS     NAMES
40811498e373   centos    "/bin/bash"   2 hours ago   Up 2 hours                         test
# 查看某个容器的信息 ， 网络模式， IP 等	
docker inspect test | grep network -i
docker inspect test | grep ipadd -i
```

#  13.1 trouble shooting

##  13.1.1 pthread_create failed

​	Golang 编写项目并打包成 Docker 镜像后，容器启动时遇到了线程创建失败问题，典型错误如下：

```sh
runtime/cgo: pthread_create failed: Operation not permitted
SIGABRT: abort
```

### 13.1.2 调整 seccomp 配置

Seccomp 是限制系统调用的一种机制，默认情况下 Docker 使用的 seccomp 配置文件可能会阻止某些与线程相关的系统调用。解决这个问题的一个方案是禁用 seccomp 或使用自定义的 seccomp 配置文件。

- **禁用 seccomp**（不推荐用于生产环境，调试时可以使用）

```routeros
docker run --rm --security-opt seccomp=unconfined ...
```

- **使用自定义 seccomp 配置**：你可以基于默认的 seccomp 配置文件，允许 `pthread_create` 所需的系统调用。

###  13.1.3 增加 capabilities

容器的默认 `Capabilities` 设置可能不允许某些系统调用，如与线程管理相关的系统调用。你可以通过 `--cap-add` 添加更多权限，或者删除不必要的限制：

- **添加 `SYS_PTRACE` 权限**

```routeros
docker run --rm --cap-add=SYS_PTRACE ...
```

- **删除所有额外的 Capabilities 限制**（用于调试）：

```shell
docker run --rm --cap-drop ALL ...
```

但需要注意，移除 Capabilities 或添加过多权限会增加潜在的安全风险，因此应谨慎操作

## 13.2 WARNING: IPv4 forwarding is disabled. Networking will not work

docker服务启动的时候报以上错误， 在centOS 上进行以下操作可排除

```sh
# 在宿主机上执行
echo "net.ipv4.ip_forward=1" >>/usr/lib/sysctl.d/00-system.conf
sudo systemctl restart network && systemctl restart docker
```

