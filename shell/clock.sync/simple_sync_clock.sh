echo '#### start sync clock ####'
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
s_t=$(curl http://172.18.0.2:8082/data/test)
echo "server time: ${s_t}"
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
date -s "${s_t}"
echo "local  time: $(date '+%Y-%m-%d %H:%M:%S').$(date '+%N')"
echo '#### end sync clock ####'
