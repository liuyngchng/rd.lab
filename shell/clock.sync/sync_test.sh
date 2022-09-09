echo '#### start ####'
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
#echo $(date '+%Y-%m-%d %H:%M:%S').$((`date '+%N'`/1000000))
#echo -e '\nstart get server time'
echo "server time: $(curl -I --noproxy '*' http://www.baidu.com)"
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
#echo $(date '+%Y-%m-%d %H:%M:%S').$((`date '+%N'`/1000000))
echo '#### end ####'
