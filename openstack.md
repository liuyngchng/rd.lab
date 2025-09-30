# open stack DockerFile

解决问题 `System has not been booted with systemd as init system (PID 1). Can't operate`

vi open_stack_Dockerfile,

```dockerfile
FROM ubuntu/openstack:1.0.2

RUN apt-get update && apt-get install -y runit

ENTRYPOINT ["/sbin/runit"]
```

执行

```sh
docker build -f  ./open_stack_Dockerfile ./ -t openstack:1.0.0
```

查看images

```sh
docker images
REPOSITORY                          TAG                 IMAGE ID            CREATED             SIZE
openstack                           1.0.0               9ae79d8e0e60        5 seconds ago       2.07 GB
```

运行

```sh
docker run --privileged --name stack -dit openstack:1.0.0
```

