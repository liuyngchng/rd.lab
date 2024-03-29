<< !
***************************************************
author      : whoami
email       : whoami@hotmail.com
create      : 2022-06-01
description :
  (1) docker build image according to Dockerfile
  (2) deploy docker container service
  (3) tail log, terminate when success flag matched
***************************************************
!
app=tls

function check_log() {
  my_app=$1
  trigger='srv_started'
#  sed /"${trigger}"/q; kill $!;  < <(exec timeout 1m docker logs -f ${my_app})
    docker logs -f ${my_app} | while read line
    do
      echo $line
        if echo $line | grep "${trigger}" --color;then
          echo -e "\e[1;32m${my_app} app deploy OK\e[0m"
          kill $$
          break
        fi
    done
    return 0
}

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
cmd="check_log ${app}"
echo -e "\e[1;32m${cmd}\e[0m"
eval ${cmd}
