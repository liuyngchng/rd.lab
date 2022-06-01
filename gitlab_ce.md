# pull image

```sh
docker pull gitlab/gitlab-ce:latest
docker images
# image
REPOSITORY                                TAG       IMAGE ID       CREATED        SIZE
gitlab/gitlab-ce                          latest    12a51c5b4ce2   11 days ago    2.49GB
```

# make dir

```sh
sudo mkdir -p /data/gitlab/config  
sudo mkdir -p /data/gitlab/logs
sudo mkdir -p /data/gitlab/data
sudo chown myuser.mygroup -R /data
cd /data/gitlab
ls
# 目录已创建好
config  data  logs
```

# startup shell

```sh
cd /data/gitlab
touch gitlab_start.sh
vi gitlab_start.sh
:set paste
# 粘贴内容如下
#!/bin/sh
GITLAB_HOME=/data/gitlab
docker run --detach \
    -h 192.168.1.100 \
    -p 443:443 -p 80:80 -p 10080:22 \
    --name gitlab \
    --restart always \
    -v ${GITLAB_HOME}/config:/etc/gitlab \
    -v ${GITLAB_HOME}/logs:/var/log/gitlab \
    -v ${GITLAB_HOME}/data:/var/opt/gitlab \
    gitlab/gitlab-ce:latest
chmod +x gitlab_start.sh
```

# run

```sh
./gitlab_start.sh
docker ps
CONTAINER ID   IMAGE            COMMAND             CREATED   STATUS          PORTS        NAMES
b80942   git../g..-ce:latest   "/assets/wrapper"   1 min ago   Up    :::80->80/tcp,...     gitlab
```

# login

```sh
http://192.1681.100
```

获取 root 密码

```sh
sudo cat /data/gitlab/config/initial_root_password
Password: init_password
# username: root
# password:init_password
```

登录后修改密码 User -> Preferences -> Password