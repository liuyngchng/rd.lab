# Ubuntu16.04下安装GTK+

##  安装

安装gcc/g++/gdb/make 等基本编程工具  

```
sudo apt-get install build-essential  
```
安装 libgtk2.0-dev libglib2.0-dev 等开发相关的库文件  

```
sudo apt-get install gnome-core-devel   
```
用于在编译GTK程序时自动找出头文件及库文件位置  

```
sudo apt-get install pkg-config
```
安装 devhelp GTK文档查看程序

```
sudo apt-get install devhelp
```
安装 gtk/glib 的API参考手册及其它帮助文档

```
sudo apt-get install libglib2.0-doc libgtk2.0-doc
```
安装基于GTK的界面GTK是开发Gnome窗口的c/c++语言图形库 

```
sudo apt-get install glade libglade2-dev
```
或者
```
sudo apt-get install glade-gnome glade-common glade-doc
```
安装gtk2.0 或者 将gtk+2.0所需的所有文件统通下载安装完毕

```
sudo apt-get install libgtk2.0-dev
```
或者
```
sudo apt-get install libgtk2.0*
```
##  查看 GTK 库版本

查看 2.x 版本

```
pkg-config --modversion gtk+-2.0
```
查看pkg-config的版本

```
pkg-config --version
```
查看是否安装了gtk

```
$pkg-config --list-all grep gtk
```

#  安装字体

```
sudo cp simsun.ttc /usr/share/fonts
cd /usr/share/fonts
sudo chmod 644 simsun.ttc
```

 更新字体缓存:

```
sudo mkfontscale
sudo mkfontdir
sudo fc-cache -fsv
```


# wechat 微信

```shell
wine '/home/rd/.wine/drive_c/Program Files (x86)/Tencent/WeChat/[3.6.0.18]/WeChat.exe'
```
 wine是把windows的接口翻译成Linux，但是有一部分没有翻译，所以就导致一部分功能不能用，   
比如中文的显示异常。这是因为缺少一些windows的库。winetricks应运而生，他可以图形化的安装wine缺少的库

```shell
sudo apt-get install winetricks
```
解决 Ubuntu 20.04 桌面平台下 Deepin 版微信的中文乱码问题。

```shell
安装相关字体

sudo apt-get install -y ttf-wqy-microhei  #文泉驿-微米黑
sudo apt-get install -y ttf-wqy-zenhei  #文泉驿-正黑
sudo apt-get install -y xfonts-wqy #文泉驿-点阵宋体
```

Bash
注销当前系统用户的登录（可能要重启系统） 
解决ubuntu下wine qq或微信等软件无法用ibus输入法输入中文的问题
先修改文件/etc/profile，打开终端输入sudo vim /etc/profile运行然后按要求输入密码进行文件编辑，
如果没有安装vim(先sudo apt-get install vim)
进入文件编辑页面后，按i进入编辑状态，把以下内容复制到文件末尾，

XMODIFIERS="@im=ibus"
XIM="ibus"
GTK_IM_MODULE="xim"
QT_IM_MODULE="xim"
ibus-daemon -d -x -r

编辑完成，按esc返回，然后输入:wq，回车保存并退出。重启后可以输入了。

Ubuntu18.04 idea快捷键ctrl-alt-left失效
Ctrl+Alt+Left/Right 被系统占用
查看
gsettings get org.gnome.desktop.wm.keybindings switch-to-workspace-left
修改：
gsettings set org.gnome.desktop.wm.keybindings switch-to-workspace-left "[]"

#  proxy for apt

```shell
vi /etc/apt/apt.conf
Acquire::http::proxy "http://1.2.3.4:8080/";
Acquire::https::proxy "https://1.2.3.4:8080/";
Acquire::ftp::proxy "ftp://1.2.3.4:8080/";
Acquire::socks::proxy "socks://1.2.3.4:8080/";
```

#  get md5 value of a string on ubuntu

```
echo -n 'my_str' |  md5sum
```
convert all char to uppercase

```shell
echo -n 'my_str' | md5sum | tr [:lower:] [:upper:]
```
# date
date in milliseconds 
```
date +"%T.%6N"
timedatectl timesync-status
```
# network 

##  对 eth0 网卡进行延迟设置

```shell
tc qdisc add dev eth0 root netem delay 150ms
# 设置eth0包延迟 150ms
tc qdisc change dev eth0 root netem delay 150ms 10ms
# 设置eth0包延迟 150ms ± 10ms
tc qdisc change dev eth0 root netem delay 150ms 10ms 25%
# 设置eth0包延迟 150ms ± 10ms，下一个随机元素取决于上一个的25%（约）
tc qdisc change dev eth0 root netem gap 5 delay 10ms
# 设置eth0包延迟每5个包有一个包延迟10ms
```

##  对 eth0 网卡限制带宽

```shell
tc qdisc add dev eth0 root tbf rate 500Kbit latency 50ms burst 15kb
# 将eth0网卡限速到500Kbit/s，15bk的buffer，TBF最多产生50ms的延迟
# tbf是Token Bucket Filter的简写，适合于把流速降低到某个值
```
##  列出已有的策略

```shell
tc -s qdisc ls dev eth0
tc -q qdisc ls dev eth0
```
##  解除 eth0 网卡的限制

```shell
tc qdisc del dev eth0 root
```

# 录屏

```sh
sudo apt install kazam
```

# 远程桌面访问 windows

```sh
sudo apt-get install rdesktop
rdesktop -f -a 16 10.0.0.1
```

# ubuntu 访问 windows 共享文件夹

```sh
sudo apt-get install samba
```

在ubuntu 的文件夹界面中点击 “connect to server”， 输入 smb://10.0.0.1/sharedFolderName

# IP route 配置

## 添加路由规则

```
sudo ip route add 目标网络/子网掩码 via 网关 dev 网卡名称
```

子网掩码的写法示例如下，例如128.14.32.0/20，其中前20位为网络前缀，后12位为主机号。11.0.0.0/8代表 11.*的所有网络地址。

## 更改metric

例如，更改目标网络地址为 192.168.1.0/24路由规则中新的 metric 值为 100

```
sudo ip route change 192.168.1.0/24 metric 100
```



