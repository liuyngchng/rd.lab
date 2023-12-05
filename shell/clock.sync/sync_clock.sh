echo '#### start ####'
t123=$(curl http://172.18.0.2:8082/data/t?t1=$(date +%s%N))
t4=$(date +%s%N)
echo "${t123},${t4}"
t1=`echo ${t123} | awk -F ',' '{print $1}'`
t2=`echo ${t123} | awk -F ',' '{print $2}'`
t3=`echo ${t123} | awk -F ',' '{print $3}'`
offset=$[$[$[$t4-$t1]-$[$t3-$t2]]/4]
echo "offset=${offset} ps"
quotient=$[${offset}/1000000000]
remainder=`echo $[${offset}%1000000000] | awk '{printf("%010d\n",$0)}'`
echo "offset=${quotient}.${remainder} s"

echo -e "t1:$t1\nt2:$t2\nt3:$t3\nt4:$t4"
s_t=$(curl http://172.18.0.2:8082/data/test)
echo "server time: ${s_t}"
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
sec=`echo $s_t | awk -F ':|\.' '{print $3}'`
micro_sec=`echo ${s_t} | awk -F ':|\.' '{print $4}'`
s_t_1="${sec}${micro_sec}"
echo "s_t_1      : ${s_t_1}"
echo 'server time: '$sec.${micro_sec}
a=${s_t_1//$'\r'}
b=${offset//$'\r'}
real_s_t=$[10#$a+10#$b]
echo 'real_s_t='${real_s_t=}
s1=`echo $s_t | awk -F ':|\.' '{print $1}'`
s2=`echo $s_t | awk -F ':|\.' '{print $2}'`
echo "calcu  time: ${s1}:${s2}:${real_s_t}"
echo '#### end ####'
