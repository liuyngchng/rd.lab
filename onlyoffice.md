# docker image

```sh
docker pull docker.1ms.run/onlyoffice/communityserver

docker stop onlyoffice
docker rm onlyoffice

docker run -i -t -d \
  -p 20080:80 \
  -p 20443:443 \
  --restart=always \
  -e MYSQL_ROOT_PASSWORD=0000 \
  -v /data/onlyoffice/data:/var/www/onlyoffice/Data \
  -v /data/onlyoffice/logs:/var/log/onlyoffice \
  -v /data/onlyoffice/mysql:/var/lib/mysql \
  --name onlyoffice \
  docker.1ms.run/onlyoffice/communityserver:latest
```



