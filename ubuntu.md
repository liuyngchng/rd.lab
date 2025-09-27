# 1. Ubuntu 22.04 LTS 安装中文输入法

​		目前最新的系统，设置安装语言有除英文外，额外安装中文，按照系统提示安装完包，直接在输入法中添加 智能拼音输入法即可。

## 1.1 检查系统中文环境

​		在 Ubuntu 设置中打开「区域与语言」—— 「管理已安装的语言」，然后会自动检查已安装语言是否完整。若不完整，根据提示安装即可

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

## 1.2 安装中文词库

在 GitHub 打开[维基百科中文拼音词库](https://github.com/felixonmars/fcitx5-pinyin-zhwiki)的 [Releases](https://github.com/felixonmars/fcitx5-pinyin-zhwiki/releases) 界面，下载最新版的 `.dict` 文件。按照 README 的指导，将其复制到 `~/.local/share/fcitx5/pinyin/dictionaries/` 文件夹下即可。

```shell
# 下载词库文件
wget https://github.com/felixonmars/fcitx5-pinyin-zhwiki/releases/download/0.2.4/zhwiki-20220416.dict
# 创建存储目录
mkdir ~/.local/share/fcitx5/pinyin/dictionaries/
# 移动词库文件至该目录
mv zhwiki-20220416.dict ~/.local/share/fcitx5/pinyin/dictionaries/
```

## 1.3 设置为默认输入法

使用 im-config 工具可以配置首选输入法，在任意命令行输入：

```shell
im-config
```

根据弹出窗口的提示，将首选输入法设置为 Fcitx 5 即可。

## 1.4 环境变量

需要为桌面会话设置环境变量，即将以下配置项写入某一配置文件中：

```shell
export XMODIFIERS=@im=fcitx
export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
```

如果使用 Bash 作为 shell，则建议写入至 `~/.bash_profile`，这样只对当前用户生效，而不影响其他用户。

另一个可以写入此配置的文件为系统级的 `/etc/profile`。

将配置写入到/etc/profile文件末尾

## 1.5 开机自启动

安装 Fcitx 5 后并没有自动添加到开机自启动中，每次开机后需要手动在应用程序中找到并启动，非常繁琐。

解决方案非常简单，在 Tweaks（`sudo apt install gnome-tweaks`）中将 Fcitx 5 添加到「开机启动程序」列表中即可。

将Fcitx5添加到开机启动程序列表中

## 1.6 Fcitx 配置

Fcitx 5 提供了一个基于 Qt 的强大易用的 GUI 配置工具，可以对输入法功能进行配置。有多种启动该配置工具的方法：

1. 在应用程序列表中打开「Fcitx 配置」
2. 在 Fcitx 托盘上右键打开「设置」
3. 命令行命令 `fcitx5-configtool`

根据个人偏好进行设置即可。需要注意的是「输入法」标签页下，应将「键盘 - 英语」放在首位，拼音（或其他中文输入法）放在后面的位置。

## 1.6 已知问题

（1）修复 JetBrains 系 IDE 显示问题。在 JetBrains 系 IDE（如 PyCharm）中，输入法选择框的位置始终固定于屏幕左下角，而非随输入光标移动，在中文输入很不方便。该问题为 IDE 的 [JetBrainsRuntime](https://github.com/JetBrains/JetBrainsRuntime) 缺陷所致。可尝试使用 [RikudouPatrickstar/JetBrainsRuntime-for-Linux-x64](https://github.com/RikudouPatrickstar/JetBrainsRuntime-for-Linux-x64) 这个仓库[发布](https://github.com/RikudouPatrickstar/JetBrainsRuntime-for-Linux-x64/releases)的 JBR 文件解决。

（2）卸载 iBus 影响 Fcitx 5 正常使用。出于精简空间和减少冲突干扰之考虑，使用 `sudo apt remove ibus` 卸载了 iBus，但重启（使生效）之后发现 Fcitx 5 受到了影响。具体表现为：除在终端中之外，其他输入场景无法切换至中文输入。使用 apt 装回 iBus，再次重启即又恢复正常。

检查包依赖关系，卸载 ibus 包后会自动移除 ibus-data、ibus-gtk4、python3-ibus-1.0 三个包，似乎都只是与 iBus 紧密联系的。暂为不解之谜。

# 2. Ubuntu16.04

##  2.1 GTK+

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
##  2.2 查看 GTK 库版本

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

#  3. 安装字体

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


# 4. wechat 微信

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

#  5. proxy for apt

```shell
vi /etc/apt/apt.conf
Acquire::http::proxy "http://1.2.3.4:8080/";
Acquire::https::proxy "https://1.2.3.4:8080/";
Acquire::ftp::proxy "ftp://1.2.3.4:8080/";
Acquire::socks::proxy "socks://1.2.3.4:8080/";
```

#  6. get md5 value of a string on ubuntu

```
echo -n 'my_str' |  md5sum
```
convert all char to uppercase

```shell
echo -n 'my_str' | md5sum | tr [:lower:] [:upper:]
```
# 7. date
date in milliseconds 
```
date +"%T.%6N"
timedatectl timesync-status
```
# 8. network 

##  8.1 对 eth0 网卡进行延迟设置

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

##  8.2 对 eth0 网卡限制带宽

```shell
tc qdisc add dev eth0 root tbf rate 500Kbit latency 50ms burst 15kb
# 将eth0网卡限速到500Kbit/s，15bk的buffer，TBF最多产生50ms的延迟
# tbf是Token Bucket Filter的简写，适合于把流速降低到某个值
```
##  8.3 列出已有的策略

```shell
tc -s qdisc ls dev eth0
tc -q qdisc ls dev eth0
```
##  8.4 解除 eth0 网卡的限制

```shell
tc qdisc del dev eth0 root
```

# 9. 录屏

```sh
sudo apt install kazam
```

# 10. 远程桌面访问 windows

```sh
sudo apt-get install rdesktop
rdesktop -f -a 16 10.0.0.1
```

# 11. ubuntu 访问 windows 共享文件夹

```sh
sudo apt-get install samba
```

在ubuntu 的文件夹界面中点击 “connect to server”， 输入 smb://10.0.0.1/sharedFolderName

# 12. IP route 配置

## 12.1 添加路由规则

```
sudo ip route add 目标网络/子网掩码 via 网关 dev 网卡名称
```

子网掩码的写法示例如下，例如128.14.32.0/20，其中前20位为网络前缀，后12位为主机号。11.0.0.0/8代表 11.*的所有网络地址。

## 12.2 更改metric

例如，更改目标网络地址为 192.168.1.0/24路由规则中新的 metric 值为 100

```
sudo ip route change 192.168.1.0/24 metric 100
```



# 13. virtualBox 中 ubuntu 无法打开 terminal

## 13.1 环境

 host：ubuntu 22.04 LTS

virtualbox：7.0

v-host: ubuntu 22.04 LTS

##  13.2 ToDo

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

#  14. cmd

netstat  need to run

```sh
sudo apt install net-tools
```

# 15. share network between hosts 

host A：ubuntu16.04, 有两个网卡，一个接外网，一个与主机B相接  

hostB：ubuntu16.04  

两台机器通过网线共享网络

## 15.1 config host A 

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

##  15.2 config host B

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

##  15.3 config NAT on host A

这一步是为了B主机能通过A主机访问外网  

```sh
su
echo 1 > /proc/sys/net/ipv4/ip_forward 
iptables -F
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -t nat -A POSTROUTING -o wlp2s0 -j MASQUERADE     （wlp2s0为host A接外网的网卡）
```

##  15.4 debug

配置完以上信息后，若发现 host A 无法上网，则是默认路由导致的，
执行

```sh
ip route show
sudo route del default gw 192.168.49.1
```

# 16. ubuntu 录制 gif 文件

```sh
sudo apt install byzanz imagemagick -y
# x, y 起始点坐标， x方向为从左向右， y方向为从上到下，width为宽度；height为高度；
# duration为整个gif的持续时长，单位为秒； delay 为延迟开始的时间，单位为秒
byzanz-record --duration=10 --x=10 --y=50 --width=800 --height=600 --delay=5 git_output.gif
# 
byzanz-record --duration=20 --x=10 --y=100 --width=1280 --height=800 --delay=5  git_output.gif
```

# 17. video edit

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

# 18. 显卡安装

## 18.1 环境信息

​	（1）显卡型号。 Geforce RTX 3090 TI

​	（2）OS

```sh
uname -a
Linux rd-lt 6.8.0-52-generic #53~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC Wed Jan 15 19:18:46 UTC 2 x86_64 x86_64 x86_64 GNU/Linux
```

​	（3）查看显卡信息。接入硬件后，查看显卡信息，获得显卡型号为 2203

```sh
lspci | grep -i vga
22:00.0 VGA compatible controller: NVIDIA Corporation Device 2203 (rev a1)
```

## 18.2 安装驱动

​		**（1）通过系统提示安装**

​		在 ubuntu 系统的应用界面中，打开 “附加驱动（Additional Drivers）”， 或者在 “Software & Updates” 中打开 “Additional Drivers” 选项卡， 选择靠前的第一个驱动 “nvidia-550(proprietary, tested)”, 选择 “Apply Changes”即可。

​		**（2）通过英伟达官方网站安装**

​		可以通过页面 https://admin.pci-ids.ucw.cz/read/PC/10de/2203 查询显卡信息， 输入2203 ，获得

```
Name: GA102 [GeForce RTX 3090 Ti]
Stefan
2022-03-25 16:26:23
```

​		进入英伟达官网  https://www.nvidia.cn/drivers/lookup/， 下载驱动， 选项如下所示

<form class="minHeight" action="" method="post" id="manualSearchForm" style="padding: 5px 40px;" __bizdiag="360597525" __biza="WJ__"> 
  <div id="manualSearchElements" style="z-index: 1"> 
   <div id="manualSearchElementsSpinner" style="visibility: hidden;"> 
    <div class="brand-spinner" style="margin: 15vh 30vh; position: absolute; opacity: 0;"></div> 
   </div> 
   <div class="drvrContainer"> 
    <div class="search-query-box"> 
     <input id="nvProductInput" type="text" name="nvProductAutoCompletePOC" class="input-medium search-query search_1 ui-autocomplete-input" placeholder="按产品、产品类型或系列搜索" autocomplete="off"> 
     <i class="fa fa-search"></i> 
    </div> 
   </div> 
  </div> 
  <div id="driverDropdowns" style="z-index: 2; visibility: visible; opacity: 1;"><div class="drvrContainer" name="manualSearch-0-div" id="manualSearch-0-div"><select class="drvrtoField ui-selectmenu-button ui-widget ui-state-default ui-corner-all" name="manualSearch-0" id="manualSearch-0" style="display: none;"><option value="1" id="manualSearch-0-0">GeForce</option><option value="11" id="manualSearch-0-1">TITAN</option><option value="3" id="manualSearch-0-2">NVIDIA RTX / Quadro</option><option value="7" id="manualSearch-0-3">Data Center / Tesla</option><option value="9" id="manualSearch-0-4">GRID</option><option value="8" id="manualSearch-0-5">NVS</option><option value="6" id="manualSearch-0-6">ION</option><option value="4" id="manualSearch-0-7">Legacy</option><option value="5" id="manualSearch-0-8">3D Vision</option></select><span tabindex="0" id="manualSearch-0-button" role="combobox" aria-expanded="false" aria-autocomplete="list" aria-owns="manualSearch-0-menu" aria-haspopup="true" class="ui-selectmenu-button ui-selectmenu-button-closed ui-corner-all ui-button ui-widget"><span class="ui-selectmenu-icon ui-icon ui-icon-triangle-1-s"></span><span class="ui-selectmenu-text">GeForce</span></span><a id="IdentifyGpuSeries-Modal" class="btn-content btncta" tabindex="0" style="cursor:pointer;"><span class="fal fa-info-circle"></span></a></div><div class="drvrContainer" name="manualSearch-1-div" id="manualSearch-1-div"><select class="drvrtoField ui-selectmenu-button ui-widget ui-state-default ui-corner-all" name="manualSearch-1" id="manualSearch-1" style="display: none;"><option value="131" id="manualSearch-1-0">GeForce RTX 50 Series</option><option value="129" id="manualSearch-1-1">GeForce RTX 40 Series (Notebooks)</option><option value="127" id="manualSearch-1-2">GeForce RTX 40 Series</option><option value="123" id="manualSearch-1-3">GeForce RTX 30 Series (Notebooks)</option><option value="120" id="manualSearch-1-4">GeForce RTX 30 Series</option><option value="111" id="manualSearch-1-5">GeForce RTX 20 Series (Notebooks)</option><option value="107" id="manualSearch-1-6">GeForce RTX 20 Series</option><option value="125" id="manualSearch-1-7">GeForce MX500 Series (Notebooks)</option><option value="121" id="manualSearch-1-8">GeForce MX400 Series (Notebooks)</option><option value="117" id="manualSearch-1-9">GeForce MX300 Series (Notebooks)</option><option value="113" id="manualSearch-1-10">GeForce MX200 Series (Notebooks)</option><option value="104" id="manualSearch-1-11">GeForce MX100 Series (Notebook)</option><option value="115" id="manualSearch-1-12">GeForce GTX 16 Series (Notebooks)</option><option value="112" id="manualSearch-1-13">GeForce 16 Series</option><option value="101" id="manualSearch-1-14">GeForce 10 Series</option><option value="102" id="manualSearch-1-15">GeForce 10 Series (Notebooks)</option><option value="98" id="manualSearch-1-16">GeForce 900 Series</option><option value="99" id="manualSearch-1-17">GeForce 900M Series (Notebooks)</option><option value="97" id="manualSearch-1-18">GeForce 800M Series (Notebooks)</option><option value="95" id="manualSearch-1-19">GeForce 700 Series</option><option value="92" id="manualSearch-1-20">GeForce 700M Series (Notebooks)</option><option value="85" id="manualSearch-1-21">GeForce 600 Series</option><option value="84" id="manualSearch-1-22">GeForce 600M Series (Notebooks)</option><option value="76" id="manualSearch-1-23">GeForce 500 Series</option><option value="78" id="manualSearch-1-24">GeForce 500M Series (Notebooks)</option><option value="71" id="manualSearch-1-25">GeForce 400 Series</option><option value="72" id="manualSearch-1-26">GeForce 400M Series (Notebooks)</option><option value="70" id="manualSearch-1-27">GeForce 300 Series</option><option value="69" id="manualSearch-1-28">GeForce 300M Series (Notebooks)</option><option value="52" id="manualSearch-1-29">GeForce 200 Series</option><option value="62" id="manualSearch-1-30">GeForce 200M Series (Notebooks)</option><option value="59" id="manualSearch-1-31">GeForce 100 Series</option><option value="61" id="manualSearch-1-32">GeForce 100M Series (Notebooks)</option><option value="51" id="manualSearch-1-33">GeForce 9 Series</option><option value="53" id="manualSearch-1-34">GeForce 9M Series (Notebooks)</option><option value="1" id="manualSearch-1-35">GeForce 8 Series</option><option value="54" id="manualSearch-1-36">GeForce 8M Series (Notebooks)</option><option value="2" id="manualSearch-1-37">GeForce 7 Series</option><option value="55" id="manualSearch-1-38">GeForce Go 7 Series (Notebooks)</option><option value="3" id="manualSearch-1-39">GeForce 6 Series</option><option value="4" id="manualSearch-1-40">GeForce 5 FX Series</option></select><span tabindex="0" id="manualSearch-1-button" role="combobox" aria-expanded="false" aria-autocomplete="list" aria-owns="manualSearch-1-menu" aria-haspopup="true" class="ui-selectmenu-button ui-selectmenu-button-closed ui-corner-all ui-button ui-widget"><span class="ui-selectmenu-icon ui-icon ui-icon-triangle-1-s"></span><span class="ui-selectmenu-text">GeForce RTX 30 Series</span></span></div><div class="drvrContainer" name="manualSearch-2-div" id="manualSearch-2-div"><select class="drvrtoField ui-selectmenu-button ui-widget ui-state-default ui-corner-all" name="manualSearch-2" id="manualSearch-2" style="display: none;"><option value="985" id="manualSearch-2-0">GeForce RTX 3090 Ti</option><option value="930" id="manualSearch-2-1">GeForce RTX 3090</option><option value="964" id="manualSearch-2-2">GeForce RTX 3080 Ti</option><option value="929" id="manualSearch-2-3">GeForce RTX 3080</option><option value="965" id="manualSearch-2-4">GeForce RTX 3070 Ti</option><option value="933" id="manualSearch-2-5">GeForce RTX 3070</option><option value="934" id="manualSearch-2-6">GeForce RTX 3060 Ti</option><option value="942" id="manualSearch-2-7">GeForce RTX 3060</option><option value="975" id="manualSearch-2-8">GeForce RTX 3050</option></select><span tabindex="0" id="manualSearch-2-button" role="combobox" aria-expanded="false" aria-autocomplete="list" aria-owns="manualSearch-2-menu" aria-haspopup="true" class="ui-selectmenu-button ui-selectmenu-button-closed ui-corner-all ui-button ui-widget"><span class="ui-selectmenu-icon ui-icon ui-icon-triangle-1-s"></span><span class="ui-selectmenu-text">GeForce RTX 3090 Ti</span></span></div><div class="drvrContainer" name="manualSearch-4-div" id="manualSearch-4-div"><select class="drvrtoField ui-selectmenu-button ui-widget ui-state-default ui-corner-all" name="manualSearch-4" id="manualSearch-4" style="display: none;"><option value="57" id="manualSearch-4-0">Windows 10 64-bit</option><option value="135" id="manualSearch-4-1">Windows 11</option><option value="19" id="manualSearch-4-2">Windows 7 64-bit</option><option value="124" id="manualSearch-4-3">Linux aarch64</option><option value="12" id="manualSearch-4-4">Linux 64-bit</option><option value="22" id="manualSearch-4-5">FreeBSD x64</option></select><span tabindex="0" id="manualSearch-4-button" role="combobox" aria-expanded="false" aria-autocomplete="list" aria-owns="manualSearch-4-menu" aria-haspopup="true" class="ui-selectmenu-button ui-button ui-widget ui-selectmenu-button-closed ui-corner-all" aria-activedescendant="ui-id-109" aria-labelledby="ui-id-109" aria-disabled="false"><span class="ui-selectmenu-icon ui-icon ui-icon-triangle-1-s"></span><span class="ui-selectmenu-text">Linux 64-bit</span></span></div><div class="drvrContainer" name="manualSearch-5-div" id="manualSearch-5-div"><select class="drvrtoField ui-selectmenu-button ui-widget ui-state-default ui-corner-all" name="manualSearch-5" id="manualSearch-5" style="display: none;"><option value="1033" id="manualSearch-5-0">English (US)</option><option value="1078" id="manualSearch-5-1">English (UK)</option><option value="1093" id="manualSearch-5-2">English (India)</option><option value="2052" id="manualSearch-5-3">Chinese (Simplified)</option><option value="1028" id="manualSearch-5-4">Chinese (Traditional)</option><option value="1041" id="manualSearch-5-5">Japanese</option><option value="1042" id="manualSearch-5-6">Korean</option><option value="1031" id="manualSearch-5-7">Deutsch</option><option value="1069" id="manualSearch-5-8">Español (España)</option><option value="11274" id="manualSearch-5-9">Español (América Latina)</option><option value="1036" id="manualSearch-5-10">Français</option><option value="1040" id="manualSearch-5-11">Italiano</option><option value="1045" id="manualSearch-5-12">Polski</option><option value="1046" id="manualSearch-5-13">Português (Brazil)</option><option value="2092" id="manualSearch-5-14">Русский</option><option value="1055" id="manualSearch-5-15">Turkish</option><option value="1116" id="manualSearch-5-16">Other</option></select><span tabindex="0" id="manualSearch-5-button" role="combobox" aria-expanded="false" aria-autocomplete="list" aria-owns="manualSearch-5-menu" aria-haspopup="true" class="ui-selectmenu-button ui-selectmenu-button-closed ui-corner-all ui-button ui-widget"><span class="ui-selectmenu-icon ui-icon ui-icon-triangle-1-s"></span><span class="ui-selectmenu-text">Chinese (Simplified)</span></span></div><div id="driverTypeWindowsDiv" class="drvrContainer" style="display: none;"><span id="driverTypeWindowsSpan" style="display: none;"><select class="drvrtoField ui-selectmenu-button ui-widget ui-state-default ui-corner-all" name="driverTypeWindowsDCH" id="driverTypeWindowsDCH" style="display: none;"><option value="0">标准</option><option value="1">DCH</option></select><span tabindex="0" id="driverTypeWindowsDCH-button" role="combobox" aria-expanded="false" aria-autocomplete="list" aria-owns="driverTypeWindowsDCH-menu" aria-haspopup="true" class="ui-selectmenu-button ui-selectmenu-button-closed ui-corner-all ui-button ui-widget"><span class="ui-selectmenu-icon ui-icon ui-icon-triangle-1-s"></span><span class="ui-selectmenu-text">标准</span></span><a href="#DCH-Modal" data-href="#DCH-Modal" target="overlay" onclick="NVIDIAGDC.button.click(this, $(this).data(&quot;href&quot;),true, function() { ;return NVIDIAGDC.button.callbacks(this); }); return false;"><span class="fal fa-question-circle" id="DCHModal"></span></a></span></div><div id="driverTypeDiv" class="drvrContainer" style="display: none;"><span id="driverTypeSpan"><select class="drvrtoField ui-selectmenu-button ui-widget ui-state-default ui-corner-all" id="driverType" style="display: none;"><option value="all">All</option><option value="grd">Game Ready Driver</option><option value="qnf">New Feature Branch</option></select><span tabindex="0" id="driverType-button" role="combobox" aria-expanded="false" aria-autocomplete="list" aria-owns="driverType-menu" aria-haspopup="true" class="ui-selectmenu-button ui-selectmenu-button-closed ui-corner-all ui-button ui-widget"><span class="ui-selectmenu-icon ui-icon ui-icon-triangle-1-s"></span><span class="ui-selectmenu-text">All</span></span><a id="WqhlGrdSlb-Modal" class="btn-content btncta" style="cursor:pointer;"><span class="fal fa-info-circle"></span></a></span></div><div id="manualSearchButtonDiv" class="drvrContainer drvrContainerBtn text-right"> <a href="#" id="manualSearchButton" data-href="" data-analytics="btn-manual:" target="_self"><div class="btn">查找</div></a></div></div> 
  <!-- <div id="driverDropdowns"> --> 
 </form>

然后执行

```sh
chmod +x NVIDIA-Linux-x86_64-550.144.03.run
sudo ./NVIDIA-Linux-x86_64-550.144.03.run
```

​		**（3）查看GPU信息**

​		检查是否安装成功

```sh
nvidia-smi
```

出现

```sh
Fri Feb 21 11:18:48 2025       
+-----------------------------------------------------------------------------------------+
| NVIDIA-SMI 550.144.03             Driver Version: 550.144.03     CUDA Version: 12.4     |
|-----------------------------------------+------------------------+----------------------+
| GPU  Name                 Persistence-M | Bus-Id          Disp.A | Volatile Uncorr. ECC |
| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |
|                                         |                        |               MIG M. |
|=========================================+========================+======================|
|   0  NVIDIA GeForce RTX 3090 Ti     Off |   00000000:22:00.0 Off |                  Off |
| 34%   55C    P0            103W /  450W |       1MiB /  24564MiB |      0%      Default |
|                                         |                        |                  N/A |
+-----------------------------------------+------------------------+----------------------+
                                                                                         
+-----------------------------------------------------------------------------------------+
| Processes:                                                                              |
|  GPU   GI   CI        PID   Type   Process name                              GPU Memory |
|        ID   ID                                                               Usage      |
|=========================================================================================|
|  No running processes found                                                             |
+-----------------------------------------------------------------------------------------+
```

看到出现了  `NVIDIA GeForce RTX 3090 Ti`， 说明安装成功了。



# 19. grub 修复

针对Ubuntu系统开机后，卡在GRUB的黑屏下，无法引导进入系统的问题，可以按照以下步骤进行：

## 19.1 确认GRUB引导问题

首先，确保Ubuntu系统无法正常启动，并且已经排除了硬件故障等其他原因。系统启动时如果出现GRUB菜单无法显示、或者无法从GRUB菜单中选择系统启动等问题，很可能是GRUB引导出现问题。

## 19.2 制作Ubuntu安装介质

- 准备一个Ubuntu的安装介质，如USB驱动器或光盘。
- 确保安装介质是完整且未损坏的，可以从Ubuntu官网下载最新版本的ISO文件，并使用如Rufus（Windows）或Etcher（跨平台）等工具制作启动盘。

## 19.3 从安装介质启动Ubuntu

- 将制作好的Ubuntu安装介质插入电脑，并重启电脑。
- 在启动过程中，选择从安装介质启动（通常需要在BIOS/UEFI中设置启动顺序）。
- 在出现的启动菜单中，选择“Try Ubuntu without installing”（尝试Ubuntu而不安装）选项。

## 19.4 安装并运行Boot-Repair工具来修复GRUB引导

在Ubuntu的Live环境中，可以通过安装Boot-Repair工具来自动修复GRUB引导。具体步骤如下：

- 打开终端，输入以下命令添加Boot-Repair的PPA源并更新软件源：

```sh
bashCopy Code
sudo add-apt-repository ppa:yannubuntu/boot-repair
sudo apt-get update
```

- 安装Boot-Repair工具：

```sh
bashCopy Code
sudo apt-get install -y boot-repair
```

- 运行Boot-Repair工具：

```sh
bashCopy Code
boot-repair
```

- 在Boot-Repair的图形界面中，选择“Recommended repair”选项，然后按照提示进行操作。Boot-Repair会自动检测并修复GRUB引导问题。

# 20. 安装证书

浏览对方网站显示“Not Secure”， 因对方使用了自签名证书，导致警告。确认网站没问题后，下载对方证书（点击 not secure-> certificate detail -> detail-> export, 选择 base64-encoded ASCII, single certificate， 保存为 aaaa.crt）

**Chrome英文版导出PEM证书步骤：**

1. 访问目标网站，点击地址栏左侧的**锁形图标** → **"Certificate is valid"**
2. 在弹出窗口选择**"Details"标签** → 点击**"Export..."**
3. 保存类型选择**"PEM encoded chain"**（证书链）或**"PEM encoded"**（单证书）

```sh
# 点击Ubuntu Chrome浏览器URL前面的 not secure-> certificate detail -> detail-> export, 选择 
base64-encoded ASCII, single certificate     # (选择这个)， 导出的证书为 PEM
base64-encoded ASCII, certificate chain
DER-encoded binary, single certificate
PKCS#7, single certificate
PKCS#7, certificate chain

# 转换PEM证书为 crt证书, 详见 https://help.gitkraken.com/gitkraken-desktop/self-signed-certificates/
openssl x509 -outform der -in DOWNLOADED-CERT-NAME -out DOWNLOADED-CERT-NAME.crt
， 保存为 aaaa.crt
# (可选) 在 /usr/local/share/ca-certificates/ 下创建一个子目录，然后拷贝文件
mkdir 
sudo cp aaaa.crt /usr/local/share/ca-certificates/

# 执行后，有显示添加了一个证书，而且没有waring，则OK
sudo update-ca-certificates
#然后重启浏览器
# 验证，应该能看到已经添加的证书
ls -l /etc/ssl/certs | grep aaaa

# 验证， 如果输出为“Verify return code: 0 (ok)”，则表示证书验证成功，证书已成功导入并被系统信任
openssl s_client -connect www.customdomain.com:443 < /dev/null | grep "Verify return code"

# 验证握手，不会再提示证书的问题了
curl -s --noproxy '*' --tlsv1  'https://www.customdomain.com:443'
```

重启浏览器，会在 Chrome浏览器中看到已经导入的证书，需要编辑证书，选择3个“Trust***” 的复选框，此时浏览器访问的网址应该能正常工作了。

# 21. 离线下载安装包及其依赖包

安装工具



# 22. ubuntu24.02 MySQL



```sh
sudo apt install mysql-server
mysql -V
mysql  Ver 8.0.41-0ubuntu0.24.04.1 for Linux on x86_64 ((Ubuntu))
sudo systemctl status mysql.service
```

# 23. MP4 Player

安装VLC播放器
ubuntu 如果默认播放器无法播放MP4视频，可以尝试安装VLC播放器。安装步骤如下：



```sh
# 输入y并回车继续执行，等待解码器安装完成。
sudo apt install ubuntu-restricted-extras

# 安装VLC播放器：
sudo apt-get install vlc
```

# 24. ffmpeg 视频编辑

## 24.1 视频剪辑

```sh
sudo apt-get install ffmpeg

# 使用 ffmpeg 剪掉mp4 文件中间的一部分，比如剪掉   第5秒到第10秒   之间的部分，剪掉   第15到第20秒的部分，其他部分保留 需要无损剪辑
# -ss 设定起始时间点
# -t 设定从该点截取的时长
# 示例：-ss 5 -t 5 表示从第5秒开始，截取5秒内容（即5~10秒）。
# 截取三部分（0-5秒、10-15秒、20秒到结尾）
# 注意：-ss参数建议放在-i前加速切割，切割点必须在关键帧位置，否则时间会有偏移
ffmpeg -i input.mp4 -t 5 -c copy p1.mp4
ffmpeg -ss 10 -i input.mp4 -t 5 -c copy p2.mp4
ffmpeg -ss 20 -i input.mp4 -c copy p3.mp4

# 合并片段
echo "file 'p1.mp4'" > list.txt
echo "file 'p2.mp4'" >> list.txt
echo "file 'p3.mp4'" >> list.txt
ffmpeg -f concat -i list.txt -c copy output.mp4
```

## 24.2 添加字幕



```sh
touch sub.srt
vi sub.srt
# 内容如下 srt字幕文件格式为编号\n开始时间 --> 结束时间\n 字幕文字\n
1
00:00:10,000 --> 00:00:13,000
hello

2
00:02:00,000 --> 00:02:03,000
world

# 执行以下命令
ffmpeg -i input.mp4 -vf "subtitles=sub.srt" -c:a copy output.mp4
```

# 25. oracle client

ubuntu 24.04

```
sudo apt install alien




```

在页面下  https://www.oracle.com/database/technologies/instant-client/linux-x86-64-downloads.html 选择版本 Version 11.1.0.7.0

选择以下两个RPM包（需转换为Debian格式）：

Basic Package（基础库，oracle-instantclient11.2-basic-11.2.0.4.0-1.x86_64.rpm）

ODBC Package（ODBC驱动，oracle-instantclient11.2-odbc-11.2.0.4.0-1.x86_64.rpm）



# 26. audio track editor



```sh
sudo apt update && sudo apt install audacity
```

# 27. 启动盘制作(create startup disk)

```sh
sudo dd if=/a/b/ubuntu-24.04.2-desktop-amd64.iso of=/dev/sda bs=4M status=progress && sync
```





# 28. Ubuntu 有线网络连接手机热点

Ubuntu 通过RJ45有线网络连接client模式的路由器， 路由器无线连接手机热点，实现上网。

## 28.1 场景

Ubuntu在刚开始安装的时候，没有无线网卡驱动，只能通过有线网络RJ45接口连接网络安装无线网卡驱动。可现场没有有线网络怎么安装驱动？

手机能上网，也可以开热点。以TP-Link 的TL-WR800N（100块钱，比4G上网盒子便宜多了） 为例 连接苹果手机热点为例，进行说明。

（1）设置路由器为 client 模式（需要路由器支持，默认工作模式为 AP）。

（2）路由器连接手机热点。电脑通过有线网络连接路由器上网。

由于client模式默认网段为192.168.1.\*， 而苹果手机热点默认网段为172.20.10.\*， 需要调整

## 28.2 操作步骤

TL-WR800N client模式连接手机热点操作步骤如下。

（1）进入路由器管理界面（http://tplogin.cn, http://192.168.1.253）, 设置DHCP服务为启用（默认为自动），注意不要修改地支持开始结束号段，保存。

（2）在网络参数， LAN口设置 ，将默认的路由器IP地址由192.168.1.253 修改为172.20.10.2，与手机热点处于同一个号段，同时不与手机热点IP(172.20.10.1) 冲突。设置子网掩码为 255.255.255.240，保存。此时路由器重启， 172.20.10.2生效。

（3）电脑网线连接路由器，由于电脑自动分配的网段为192.168.1.\*, 无法直接与路由器直接相连。需要设置为手动分配IP

```sh
IP		172.20.10.11
Netmask	255.255.255.240
Route	172.20.10.1
DNS		172.20.10.1
```

此时，电脑与路由器处于同一个网段了，可以登录 http://172.20.10.2 了。

（4）设置苹果手机热点为“最大兼容模式”，工作在频段2.4GHz，耗电但兼容性强。

（5）启动路由器的 client 模式。设置向导， 下一步， 选择 client模式（客户端模式），扫描无线网，获取到主AP（苹果手机热点）的SSID和BSSID， 输入密码，连接。查看运行状态，在无线状态一栏中应该显示 连接状态：成功了。

（6）此时，通过电脑有线网络进行上网，应该是OK了。后续只要安装了无线网卡驱动，则可以通过无线网卡上网了。
