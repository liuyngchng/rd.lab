echo '#### start ####'
echo -e "t1\tt2\tt3\tt4:\n$(curl http://172.18.0.3:8082/data/t?t1=$(date +%s.%N))"
t4=$(date +%s.%N)
echo "${t4}"
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
echo '#### end ####'
