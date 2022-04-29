# 1. docker proxy

Docker代理需要在其启动服务时设置，终端设置的代理在执行 docker 命令时并不会生效，

```shell
systemctl stop docker
vim /lib/systemd/system/docker.service
```

在 Service 部分下 **增加 Environment 变量，配置成你自己的代理地址**，如下

```shell
[Service]
 
Environment="HTTP_PROXY=http://[proxy-addr]:[proxy-port]/" "HTTPS_PROXY=https://[proxy-addr]:[proxy-port]/"
```

重启 docker

```shell
systemctl daemon-reload
systemctl start docker
```



Environment="HTTP_PROXY=http://10.22.96.29:8080/" "HTTPS_PROXY=https://10.22.96.29:8080/"