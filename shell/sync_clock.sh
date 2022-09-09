echo '#### start ####'
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
echo "server time: $(curl http://172.18.0.3:8082/data/test)"
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
echo '#### end ####'
