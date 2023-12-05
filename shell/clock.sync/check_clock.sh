echo '#### start clock check ####'
# 添加 & 作为后台进程，保证2条指令同时执行
echo "h1 clock: $(curl -s http://172.18.0.3:8082/data/test)" &
echo "h2 clock: $(curl -s http://172.18.0.2:8082/data/test)"
echo '#### end clock check ####'
