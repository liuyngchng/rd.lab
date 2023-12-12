app=tlssrvr
old_img=`docker images | grep '<none>' | awk -F ' ' '{print$3}'`
make clean
make
echo "docker stop ${app}"
docker stop ${app}
echo "docker rmi ${app}"
docker rmi ${app}
if [ "${old_img}" ];then
    echo "docker rmi -f ${old_img}"
    docker rmi -f ${old_img}
else
   echo 'nothing need to be rm.'
fi
echo "docker build -f Dockerfile ./  -t ${app}"
docker build -f Dockerfile ./  -t ${app}
echo "docker run -dit --name ${app} --rm -p 8083:8083 ${app}"
docker run -dit --name ${app} --rm -p 8083:8083 ${app}
echo "docker logs -f ${app}"
docker logs -f ${app}
