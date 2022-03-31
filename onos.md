# 1. docker image
```
docker search onos
docker pull onosproject/onos
docker run -dit -P onosproject/onos
docker ps                       // get container id
docker inspect container_id | grep ipaddress -i // get container ip
docker port container_id        // get ipAddress map
```
# 2. login to container
```
ssh -p 8101 karaf@172.17.0.2
```
input password karaf

# 3. web gui
打开浏览器，在地址栏输入：http://172.17.0.2:8181/onos/ui/login.html，  
使用帐号/密码：karaf/karaf进行登录  
# 4. activate openflow and fwd
```
ssh -p 8101 karaf@172.17.0.2            //input password karaf
app activate org.onosproject.openflow   // activate openflow
app activate org.onosproject.fwd        // activate forward

```
当然也可以在web图形界面启用，点击界面左上角的图标，  
选择Applications，搜索以上两个应用然后启用即可
# 5. 将mininet连接到ONOS
```
sudo mn --topo tree,2 --controller remote,ip=172.17.0.2
pingall
```
在浏览器（chrome）中，在地址  
```
http://172.17.0.2:8181/onos/ui/login.html
```
可以看到网络拓扑
