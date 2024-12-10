# Ubuntu 22.04 LTS 安装中文输入法

## 检查系统中文环境

在 Ubuntu 设置中打开「区域与语言」—— 「管理已安装的语言」，然后会自动检查已安装语言是否完整。若不完整，根据提示安装即可

为使用 Fcitx 5，需要安装三部分基本内容：

1. Fcitx 5 主程序
2. 中文输入法引擎
3. 图形界面相关

按照这个思路，可以直接使用 `apt` 进行安装：

```shell
sudo apt install fcitx5 \
fcitx5-chinese-addons \
fcitx5-frontend-gtk3 fcitx5-frontend-gtk2 \
fcitx5-frontend-qt5 kde-config-fcitx5
```

## 安装中文词库

在 GitHub 打开[维基百科中文拼音词库](https://github.com/felixonmars/fcitx5-pinyin-zhwiki)的 [Releases](https://github.com/felixonmars/fcitx5-pinyin-zhwiki/releases) 界面，下载最新版的 `.dict` 文件。按照 README 的指导，将其复制到 `~/.local/share/fcitx5/pinyin/dictionaries/` 文件夹下即可。

```shell
# 下载词库文件
wget https://github.com/felixonmars/fcitx5-pinyin-zhwiki/releases/download/0.2.4/zhwiki-20220416.dict
# 创建存储目录
mkdir ~/.local/share/fcitx5/pinyin/dictionaries/
# 移动词库文件至该目录
mv zhwiki-20220416.dict ~/.local/share/fcitx5/pinyin/dictionaries/
```

## 设置为默认输入法

使用 im-config 工具可以配置首选输入法，在任意命令行输入：

```shell
im-config
```

根据弹出窗口的提示，将首选输入法设置为 Fcitx 5 即可。

## 环境变量

需要为桌面会话设置环境变量，即将以下配置项写入某一配置文件中：

```shell
export XMODIFIERS=@im=fcitx
export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
```

如果使用 Bash 作为 shell，则建议写入至 `~/.bash_profile`，这样只对当前用户生效，而不影响其他用户。

另一个可以写入此配置的文件为系统级的 `/etc/profile`。

将配置写入到/etc/profile文件末尾

## 开机自启动

安装 Fcitx 5 后并没有自动添加到开机自启动中，每次开机后需要手动在应用程序中找到并启动，非常繁琐。

解决方案非常简单，在 Tweaks（`sudo apt install gnome-tweaks`）中将 Fcitx 5 添加到「开机启动程序」列表中即可。

将Fcitx5添加到开机启动程序列表中

## Fcitx 配置

Fcitx 5 提供了一个基于 Qt 的强大易用的 GUI 配置工具，可以对输入法功能进行配置。有多种启动该配置工具的方法：

1. 在应用程序列表中打开「Fcitx 配置」
2. 在 Fcitx 托盘上右键打开「设置」
3. 命令行命令 `fcitx5-configtool`

根据个人偏好进行设置即可。需要注意的是「输入法」标签页下，应将「键盘 - 英语」放在首位，拼音（或其他中文输入法）放在后面的位置。

## 已知问题

（1）修复 JetBrains 系 IDE 显示问题。在 JetBrains 系 IDE（如 PyCharm）中，输入法选择框的位置始终固定于屏幕左下角，而非随输入光标移动，在中文输入很不方便。该问题为 IDE 的 [JetBrainsRuntime](https://github.com/JetBrains/JetBrainsRuntime) 缺陷所致。可尝试使用 [RikudouPatrickstar/JetBrainsRuntime-for-Linux-x64](https://github.com/RikudouPatrickstar/JetBrainsRuntime-for-Linux-x64) 这个仓库[发布](https://github.com/RikudouPatrickstar/JetBrainsRuntime-for-Linux-x64/releases)的 JBR 文件解决。

（2）卸载 iBus 影响 Fcitx 5 正常使用。出于精简空间和减少冲突干扰之考虑，使用 `sudo apt remove ibus` 卸载了 iBus，但重启（使生效）之后发现 Fcitx 5 受到了影响。具体表现为：除在终端中之外，其他输入场景无法切换至中文输入。使用 apt 装回 iBus，再次重启即又恢复正常。

检查包依赖关系，卸载 ibus 包后会自动移除 ibus-data、ibus-gtk4、python3-ibus-1.0 三个包，似乎都只是与 iBus 紧密联系的。暂为不解之谜。

# Ubuntu16.04

##  GTK+

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



# virtualBox 中 ubuntu 无法打开 terminal

## 环境

 host：ubuntu 22.04 LTS

virtualbox：7.0

v-host: ubuntu 22.04 LTS

##  ToDo

```sh
# 进入命令行模式（需要返回桌面时CTRL + ALT + F1）
#通过虚拟键盘发送 CTRL + ALT + F3 
cd /etc/default
sudo nano locale
# 把文件中的 “en_US” 改成 “en_US.UTF-8”
# 保存退出
sudo locale-gen --purge
reboot # 重启虚拟机
# 虚拟机- xxx is not in the sudoers
su root
gedit /etc/sudoers
# 找到 “root ALL=(ALL) ALL”，在下面插入“vboxuser ALL=(ALL) ALL”
exit
# 再次执行 sudo 命令，即可成功
```

#  cmd

netstat  need to run

```sh
sudo apt install net-tools
```

# share network between hosts 

host A：ubuntu16.04, 有两个网卡，一个接外网，一个与主机B相接  

hostB：ubuntu16.04  

两台机器通过网线共享网络

## 1 config host A 

run `iwconfig`  

wlp2s0 :这个是无线网卡。
enp1s0 :有线网卡，与B主机通过网线相连的网卡  

config ip

``` sudo vim /etc/network/interfaces```  
为接口enp1s0配置静态IP地址， 

```sh
iface enp1s0 inet static
address 192.168.49.1
netmask 255.255.255.0
gateway 192.168.49.1
```

restart interface enp1s0

```sh
ifdonw enp1s0
ifup enp1s0
ifconfig  命令查看enp1s0 ip配置是否成功
```

##  config host B

run  `iwconfig` 
获取网络接口卡名称 enpxxxx    
sudo vim /etc/network/interfaces 

```  sh
iface enpxxxx inet static
address 192.168.49.2
netmask 255.255.255.0
gateway 192.168.49.1
dns-nameservers 180.76.76.76
```

restart interface enpxxxxx

```sh
ifdonw enp1s0
ifup enp1s0
ifconfig  命令查看enpxxxx ip配置是否成功
```

ping host A OK `ping 192.168.49.1`

##  config NAT on host A

这一步是为了B主机能通过A主机访问外网  

```sh
su
echo 1 > /proc/sys/net/ipv4/ip_forward 
iptables -F
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -t nat -A POSTROUTING -o wlp2s0 -j MASQUERADE     （wlp2s0为host A接外网的网卡）
```

##  debug

配置完以上信息后，若发现 host A 无法上网，则是默认路由导致的，
执行

```sh
ip route show
sudo route del default gw 192.168.49.1
```

# ubuntu 录制 gif 文件

```sh
sudo apt install byzanz imagemagick -y
# x, y 起始点坐标， x方向为从左向右， y方向为从上到下，width为宽度；height为高度；
# duration为整个gif的持续时长，单位为秒； delay 为延迟开始的时间，单位为秒
byzanz-record --duration=10 --x=0 --y=50 --width=800 --height=600 --delay=5 git_output.gif
# 
byzanz-record --duration=15 --x=0 --y=100 --width=1280 --height=800 --delay=2  git_output.gif
```

# video edit

```sh
sudo apt update
sudo apt install ffmpeg
# 裁剪视频（截取视频的一部分）
ffmpeg -i input.ts -ss 00:00:10 -t 00:00:30 -c copy output.ts
# 裁剪视频中的特定区域（基于帧的坐标）
ffmpeg -i input.ts -vf "crop=in_w-200:in_h-200:200:200" -c:a copy output.ts
# 调整视频速度
ffmpeg -i input.ts -filter:v "setpts=0.5*PTS" output.ts
# 转换视频格式
ffmpeg -i input.ts -c:v libx264 -c:a aac -strict experimental output.mp4
```

