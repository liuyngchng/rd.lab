# 10#表示将后面的数字作为10进制处理，否则，当出现0xxx时，
# shell会认为是8进制，会报错
# display milisecond
#echo -n $(date '+%Y-%m-%d %H:%M:%S').$((10#`date '+%N'`/1000000))
# nanosecond
echo $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')
