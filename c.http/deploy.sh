app=srvr
old_img=`docker images | grep '<none>' | awk -F ' ' '{print$3}'`
cmd='make clean'
echo -e "\e[1;32m${cmd}\e[0m"
eval ${cmd}
cmd='make'
echo -e "\e[1;32m${cmd}\e[0m"
eval ${cmd}
cmd="docker stop ${app}"
echo -e "\e[1;32m${cmd}\e[0m"
eval ${cmd}

cmd="docker rmi ${app}"
echo -e "\e[1;32m${cmd}\e[0m"
eval ${cmd}
if [ "${old_img}" ];then
    cmd="docker rmi -f ${old_img}"
    echo -e "\e[1;32m${cmd}\e[0m"
    eval ${cmd}
else
   echo 'nothing need to be rm.'
fi
cmd="docker build -f Dockerfile ./  -t ${app}"
echo -e "\e[1;32m${cmd}\e[0m"
eval ${cmd}
cmd="docker run -dit --name ${app} --rm -p 8083:8083 ${app}"
echo -e "\e[1;32m${cmd}\e[0m"
eval ${cmd}
cmd="docker logs -f ${app}"
echo -e "\e[1;32m${cmd}\e[0m"
eval ${cmd}
