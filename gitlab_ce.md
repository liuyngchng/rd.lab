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
# 增加执行权限
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

# gitlab-runner

## Download and install binary

```sh
# Download the binary for your system
sudo curl -L --output /usr/local/bin/gitlab-runner https://gitlab-runner-downloads.s3.amazonaws.com/latest/binaries/gitlab-runner-linux-amd64
# Give it permission to execute
sudo chmod +x /usr/local/bin/gitlab-runner
# Create a GitLab Runner user
sudo useradd --comment 'GitLab Runner' --create-home gitlab-runner --shell /bin/bash
# Install and run as a service
sudo gitlab-runner install --user=gitlab-runner --working-directory=/home/gitlab-runner
sudo gitlab-runner start
```

##  Command to register runner

```sh
# --url 后面是 gitlab 的地址，如果有端口号，需要写上端口号
sudo gitlab-runner register --url http://123.456.82.789:8080/ --registration-token $REGISTRATION_TOKEN
# Enter tags for the runner
# 此时输入 的tag 需要记住，后续配置会使用到 rd_runner

# 使用 shell 作为 executor
# 当出现 Enter an executor: docker-ssh+machine, kubernetes, custom, docker, ssh, virtualbox, docker+machine, instance, docker-ssh, parallels, shell:
# 输入 shell后回车，出现
# Runner registered successfully. Feel free to start it, but if it's running already the config should be automatically reloaded!
# Configuration (with the authentication token) was saved in "/etc/gitlab-runner/config.toml"
# 说明注册成功了
```

查看配置文件

```sh
sudo vi /etc/gitlab-runner/config.toml
```

可见内容如下所示，

```yaml
concurrent = 1
check_interval = 0

[session_server]
  session_timeout = 1800

[[runners]]
  name = "11.10.36.1"
  url = "http://123.456.82.789:8080/"
  id = 3
  token = "_6YzeSjLegiuQA4hDVKy"
  token_obtained_at = 2022-11-08T02:40:42Z
  token_expires_at = 0001-01-01T00:00:00Z
  executor = "shell"
  [runners.custom_build_dir]
  [runners.cache]
    [runners.cache.s3]
    [runners.cache.gcs]
    [runners.cache.azure]
```

由于git http 地址没有使用默认的80 端口，需要在 url 下面添加一个配置

```yaml
url = "http://123.456.82.789:8080/"
clone_url = "http://123.456.82.789:8080/"
```



##  yml config

project 根目录添加文件 `.gitlab-ci.yml`，内容如下所示：

```yaml
stages:          # List of stages for jobs, and their order of execution
  - build
  - test
  - deploy

build-job:       # This job runs in the build stage, which runs first.
  stage: build
  script:
    - /data/deploy/build.sh
    - echo "Compiling the code..."
    - echo "Compile complete."
  only:
    - master
  tags:
    - rd_runner

unit-test-job:   # This job runs in the test stage.
  stage: test    # It only starts when the job in the build stage completes successfully.
  script:
    - echo "Running unit tests... This will take about 60 seconds."
    - sleep 60
    - echo "Code coverage is 90%"

lint-test-job:   # This job also runs in the test stage.
  stage: test    # It can run at the same time as unit-test-job (in parallel).
  script:
    - echo "Linting code... This will take about 10 seconds."
    - sleep 10
    - echo "No lint issues found."

deploy-job:      # This job runs in the deploy stage.
  stage: deploy  # It only runs when *both* jobs in the test stage complete successfully.
  script:
    - echo "Deploying application..."
    - echo "Application successfully deployed."

```

在 `script` 下添加需要执行的`shell` 脚本， 在 `tags` 目录下添加之前注册`runner`的时候，填写的名称, script 下shell 的根目录(./)默认是git clone 下的项目的根目录。