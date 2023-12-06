make clean
make
app=srvr
docker stop ${app}
docker rmi ${app}
docker build -f Dockerfile ./  -t ${app}
docker run -dit --name ${app} --rm -p 8083:8083 ${app}
docker logs -f ${app}
