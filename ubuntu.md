# Ubuntu 22.04 LTS

## 安装中文输入法

### 检查系统中文环境

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

### 安装中文词库

在 GitHub 打开[维基百科中文拼音词库](https://github.com/felixonmars/fcitx5-pinyin-zhwiki)的 [Releases](https://github.com/felixonmars/fcitx5-pinyin-zhwiki/releases) 界面，下载最新版的 `.dict` 文件。按照 README 的指导，将其复制到 `~/.local/share/fcitx5/pinyin/dictionaries/` 文件夹下即可。

```shell
# 下载词库文件
wget https://github.com/felixonmars/fcitx5-pinyin-zhwiki/releases/download/0.2.4/zhwiki-20220416.dict
# 创建存储目录
mkdir ~/.local/share/fcitx5/pinyin/dictionaries/
# 移动词库文件至该目录
mv zhwiki-20220416.dict ~/.local/share/fcitx5/pinyin/dictionaries/
```

### 设置为默认输入法

使用 im-config 工具可以配置首选输入法，在任意命令行输入：

```shell
im-config
```

根据弹出窗口的提示，将首选输入法设置为 Fcitx 5 即可。

### 环境变量

需要为桌面会话设置环境变量，即将以下配置项写入某一配置文件中：

```shell
export XMODIFIERS=@im=fcitx
export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
```

如果使用 Bash 作为 shell，则建议写入至 `~/.bash_profile`，这样只对当前用户生效，而不影响其他用户。

另一个可以写入此配置的文件为系统级的 `/etc/profile`。

将配置写入到/etc/profile文件末尾

### 开机自启动

安装 Fcitx 5 后并没有自动添加到开机自启动中，每次开机后需要手动在应用程序中找到并启动，非常繁琐。

解决方案非常简单，在 Tweaks（`sudo apt install gnome-tweaks`）中将 Fcitx 5 添加到「开机启动程序」列表中即可。

将Fcitx5添加到开机启动程序列表中

### Fcitx 配置

Fcitx 5 提供了一个基于 Qt 的强大易用的 GUI 配置工具，可以对输入法功能进行配置。有多种启动该配置工具的方法：

1. 在应用程序列表中打开「Fcitx 配置」
2. 在 Fcitx 托盘上右键打开「设置」
3. 命令行命令 `fcitx5-configtool`

根据个人偏好进行设置即可。需要注意的是「输入法」标签页下，应将「键盘 - 英语」放在首位，拼音（或其他中文输入法）放在后面的位置。

### 已知问题

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



