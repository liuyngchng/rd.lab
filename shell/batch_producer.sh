#!/usr/bin/sh
echo '---------- start produce msg ----------'

#响应Ctrl+C中断
trap 'onCtrlC' INT

onCtrlC () {
    echo 'Ctrl+C is captured'
    exit 1
}

#kafka所在目录
kafkaPath=/home/rd/software/kafka_2.11-2.4.1
#broker
brokerlist=localhost:9092,localhost:9093,localhost:9094
#kafka的topic
topic=test_me
#消息总数
totalNum=100000
#一次批量发送的消息数
batchNum=10
#该标志为true，表示文件中的第一条记录
firstLineFlag='true'

for i in `seq ${totalNum}`
#for ((i=1; i<=${totalNum}; i ++))  
do  
	# datetime 
	timestamp=`date "+%Y-%m-%d %H:%M:%S.%N"`;
	# for test purpose only
	#echo $timestamp
	#消息内容，请按照实际需要自行调整
	
    messageContent='my_msg'$timestamp
    #$RANDOM
    #如果是每个批次的第一条，就要将之前的内容全部覆盖，如果不是第一条就追加到尾部
    if [ 'true' = "${firstLineFlag}" ] ; then
      echo ${messageContent} > batchMessage.txt
      #将标志设置为false，这样下次写入的时候就不会清理已有内容了
      firstLineFlag='false'
    else
      echo ${messageContent} >> batchMessage.txt
    fi

    #取余数
    modVal=$(( ${i} % ${batchNum} ))
    #如果达到一个批次，就发送一次消息
    if [ "${modVal}" = 0 ] ; then
      #在控制台显示进度
      echo “${i} of ${totalNum} sent”

      #批量发送消息，并且将控制台返回的提示符重定向到/dev/null
      cat batchMessage.txt | ${kafkaPath}/bin/kafka-console-producer.sh --socket-buffer-size 1024000 --broker-list ${brokerlist} --sync --topic ${topic} | > /dev/null
	  echo '--snd msg--'
	  cat  batchMessage.txt
      #将标志设置为true，这样下次写入batchMessage.txt时，会将文件中的内容先清除掉
      firstLineFlag='true'
    fi
done
