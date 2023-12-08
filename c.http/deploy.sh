app=srvr
old_img=`docker images | grep '<none>' | awk -F ' ' '{print$3}'`
make clean
make
docker stop ${app}
docker rmi ${app}
if [ "${old_img}" ];then
    docker rmi -f ${old_img}
else
   echo 'nothing need to be rm.'
fi
docker build -f Dockerfile ./  -t ${app}
docker run -dit --name ${app} --rm -p 8083:8083 ${app}
docker logs -f ${app}
