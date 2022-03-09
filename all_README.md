
# 1. Setup MySQL on CentOS
## 1.1 Setup

```
卸载  先停掉mysql进程   没有安装过的可以直接跳过
pkill -9 mysqld
rpm -qa|grep -i mysql
卸载
yum -y remove mysql-community-client-5.6.38-2.el7.x86_6
下载mysql的repo源 这个安装的mysql5.7.20  /**纠正一下，这源下载的是最新的版本  ****/
cd /usr/local/src/
wget http://repo.mysql.com/mysql57-community-release-el7-8.noarch.rpm
rpm -ivh mysql57-community-release-el7-8.noarch.rpm
yum -y install mysql-server
```
## 1.2 默认配置文件路径 

```
配置文件：/etc/my.cnf 
日志文件：/var/log/var/log/mysqld.log 
服务启动脚本：/usr/lib/systemd/system/mysqld.service 
socket文件：/var/run/mysqld/mysqld.pid
```

## 1.3 StartUp

启动mysql服务

```
service mysqld restart
```

在 docker 容器中启动 MySQL

```
docker run --privileged -dit --name test1  centos /usr/sbin/init
docker exec -it centos bash
systemctl start mysqld
A temporary password is generated for root@localhost: eiDIxJi8s1)h
mysql -uroot -p
```

重置密码

```
grep password /var/log/mysqld.log
```
A temporary password is generated for root@localhost: ab*******m1

```
mysql -hlocalhost -uroot -p
alter user 'root'@'localhost' identified
```

# 2. Setup Redis
```
yum install -y epel-release
yum install -y redis

```
# 3. Install MySQL
## 3.1 Install MySQL in windows
```
unzip mysql-8.0.16-winx64.zip
config ENV for ./mysql-8.0.16-winx64/bin/
mysqld.exe --initialize-insecure
mysqld.exe --install
net start mysql
```
## 3.2 Setup MySQL on ubuntu

```
sudo apt-get install mysql-server
mysqld
cd /etc/mysql
cat debian.cnf
```
user = debian-sys-maint
password = xedvSNKdLavjuEWV
```
mysql -udebian-sys-maint -pxedvSNKdLavjuEWV
show databases;
use mysql;
update user set authentication_string=PASSWORD("自定义密码") where user='root';
update user set plugin="mysql_native_password";
flush privileges;
quit;
```
restart MySQL
```
/etc/init.d/mysql restart
```
# 4. 查看挂载的硬盘

```
fdisk -l
lsblk
```
# 5. yum on centOS  
download rpm package only
```
yum install --downloadonly --downloaddir=/opt/rpms mysql
```
# 6. make a ISO start up flash disk  
## 6.1 under MacOS
如果是在Mac系统下，则 需要把下载的Ubuntu安装文件（.iso）  
转换成(.dmg)格式的文件,方便在Mac OS上面进行操作，转换命令
```
cd Downloads/
hdiutil convert -format UDRW -o ubuntu.dmg ubuntu-14.04.5-desktop-amd64.iso
```
hdiutil转换的文件后缀名为.dmg,所以需要把文件重命名为.iso，在安装的时候系统才能够更好的识别
```
mv ubuntu.dmg ubuntu.iso
```

打开终端，输入  

```
diskutil list
```

记录下U盘的地址, 然后卸载U盘命令  
```
diskutil unmountDisk [硬碟位置]
```

## 6.2  Create the installation medium in linux
Either you can burn the image onto CD/DVD, you use usb stick for the installation.  
Under linux, you can use the dd for that:
```
dd if=<source iso> of=<target device> bs=4M; sync
```
Make sure that the device does not include partition number, so example from my machine:
```
dd if=~/Downloads/alpine-standard-3.10.2-x86_64.iso of=/dev/sdb bs=4M
```
The target device will be erased, so make sure you use something without any data you do not want to lose.
## 6.3 然后移除U盘
on MacOS
```
diskutil eject /path/to/USB
```
on Linux
```
umount /path/to/USB
```
# 7. WebService Client Generation Error with JDK8

```$xslt
java.lang.AssertionError: org.xml.sax.SAXParseException;
systemId: jar:file:/path/to/glassfish/modules/jaxb-osgi.jar!/com/sun/tools/xjc/reader/xmlschema/bindinfo/binding.xsd;
lineNumber: 52; columnNumber: 88; schema_reference:
Failed to read schema document 'xjc.xsd',
because 'file' access is not allowed due to restriction set by the accessExternalSchema property.
```
Create a file named jaxp.properties (if it doesn't exist) under /path/to/jdk1.8.0/jre/lib and then write this line in it:
```$xslt
javax.xml.accessExternalSchema = all
```
# 8. use iphone as usb internet modem in ubuntu

```
sudo apt-get install ipheth-utils libimobiledevice-dev libimobiledevice-utils
```
# 9. setup atom in ubuntu 16.04
## 9.1 add source

```
sudo add-apt-repository ppa:webupd8team/atom  
sudo apt-get update  
sudo apt-get install atom
```
## 9.2 deb package

```
wget https://github.com/atom/atom/releases/download/v1.43.0/atom-amd64.deb
wget https://github.com/atom/atom/releases/download/v1.7.4/atom-amd64.deb
sudo dpkg -i atom-amd64.deb
```
# 10. pandoc

```
sudo apt-get install pandoc
sudo apt-get install texlive-lang-cjk texlive-latex-extra texlive-xetex
pandoc test.md -o test.docx
pandoc test.md -o test.pdf
```
# 11. TCP control info
## 11.1 TCP info in ubuntu

```
cd /proc/sys/net/ipv4
ls -al | grep tcp
```
## 11.2 use TCP congestion algorithm BBR
update kernel if necessary and config bbr as tcp congestion algorithm

```
wget --no-check-certificate \    'https://github.com/teddysun/across/raw/master/bbr.sh' \   && chmod +x bbr.sh && ./bbr.sh  
```
查看是否开启
```
sysctl net.ipv4.tcp_available_congestion_control
```
显示以下即已开启：
```
sysctl net.ipv4.tcp_available_congestion_control
net.ipv4.tcp_available_congestion_control = bbr cubic reno
```
```
sysctl net.ipv4.tcp_congestion_control
net.ipv4.tcp_congestion_control = bbr
```
查看BBR是否启动
```
lsmod | grep bbr
```
显示以下即启动成功：
```
lsmod | grep bbr
tcp_bbr 20480 14
```
## 11.3 停止BBR  

依次执行下面命令就可以了。   

```
sed -i '/net.core.default_qdisc/d' /etc/sysctl.conf
sed -i '/net.ipv4.tcp_congestion_control/d' /etc/sysctl.conf
sysctl -p       
reboot
```
# 12. mac terminal hostname

```
sudo scutil --set HostName my_host_name
```

# 13. some git things
## 13.1 git中文文件名变数字
修改配置
git config --global core.quotepath false
即可解决

## 13.2 use vim diff as git diff visual tool
```
sudo apt-get install vim
git config --global diff.tool vimdiff
git config --global difftool.prompt false   // 不再弹出 Launch  vimdiff ?
git config --global alias.d difftool        // 为输入方便，difftool输入实在太长， 用别名 d 来替代 difftool
git d your_file                             // enjoy your coding.
```

# 14. create ISO file in ubuntu
## 14.1  create ISO file from CD-ROM
```
sudo umount /dev/cdrom
dd if=/dev/cdrom of=file.iso bs=1024
```
## 14.2 add file or directory to ISO file
需要使用mkisofs这个工具,你想改的参数都可以修改，而且还有-gui这个参数。最简单的用法如下：
```
mkisofs -r -o file.iso your_folder_name/
```
生成一个MD5文件，执行
```
md5sum file.iso > file.iso.md5
```
## 14.3 burn ISO file to CD-ROM
 右键, write to disc...),点击这个选项

## 14.4 Create the installation medium
Either you can burn the image onto CD/DVD, you use usb stick for the installation.  
Under linux, you can use the dd for that:
```
dd if=<source iso> of=<target device> bs=4M; sync
```
Make sure that the device does not include partition number, so example from my machine:
```
dd if=~/Downloads/alpine-standard-3.10.2-x86_64.iso of=/dev/sdb bs=4M
```
The target device will be erased, so make sure you use something without any data you do not want to lose

# 15. convert GBK(gb18030 gbk) text file to readable file in ubuntu (UTF-8 format)

```
 iconv -f gbk -t utf8 gbk.txt > utf8.txt
```
# 16. network traffic monitoring/网络流量监控  
```
iftop -i interface
```
# 17. replace tab  

## 17.1 TAB替换为空格  
```
:set ts=4
:set expandtab
:%retab!
```

## 17.2 空格替换为TAB  

```
:set ts=4
:set noexpandtab
:%retab!
```
## 17.3 删除空白行（delete all blank lines)

run `g/^\s*$/d`

# 18. compile quant project

to debug the openssl version, download openssl new version,  
add parameter in cmake

`cmake ../ -DOPENSSL_ROOT_DIR=/usr/local/ssl -DOPENSSL_LIBRARIES=/usr/local/ssl/lib`

# 19. delete cache file after 'sudo apt-get install for ubuntu'  
run `sudo apt-get clean`
and then all file in /var/cache/apt/archives be deleted.

# 20. ubuntu support exfat disk format

run `sudo apt-get install exfat-utils`

# 21. kylin 4.0.2 source list

修改 apt 的源文件管理文件 :  `/etc/apt/sources.list`
， 修改为
```
deb http://archive.kylinos.cn/yhkylin juniper main restricted universe multiverse
deb http://cz.archive.ubuntu.com/ubuntu trusty main
deb [arch=amd64] https://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu trusty stable
```

保存后， 更新源：

apt-get update


# 22. ubuntu wifi driver setup (wireless card driver)  

run `lspci | grep Wireless`  
看到无线网卡类型为 
```
Broadcom Inc. and subsidiaries BCM4360 802.11ac Wireless Network Adapter (rev 03)
```
执行
```
apt install firmware-b43-installer
apt-get install bcmwl-kernel-source
```

# 23. rar and unrar in ubuntu with password
```
sudo apt-get install rar unrar -y
rar -p a test.csv.rar test.csv   // input password
unrar -x test.csv.rar 			// input password
```
# 24. disk IO
[userbenchmark](https://ssd.userbenchmark.com)
SSD write speed 1000~2000 MByte per second.
HDD,Hard Disk Drive write speed 100 MByte per second. 

# 25. 设置ubuntu默认登录为非图形化界面

如果想让系统默认不进入图形界面，只需编辑文件  
/etc/default/grub
把原来的  
GRUB_CMDLINE_LINUX_DEFAULT=”quiet splash”
改成  
GRUB_CMDLINE_LINUX_DEFAULT=”quiet splash text”
然后再运行  
sudo update-grub 

即可。

如果想进入图形界面，输入命令：   
sudo lightdm 

# 26. after install ubuntu on Mac and then delete ubuntu, efi boot is redundant.

在Mac安装ubuntu后开机默认进入Grub引导，删除ubuntu后Grub引导依旧存在，  
导致每次开机都要按住option才能进入Mac系统

打开终端，挂载EFI分区
```
mkdir /mnt
sudo mount -t msdos /dev/disk0s1 /mnt
```
查看当前EFI分区

```
ls /mnt/
ls /mnt/EFI/
cd /mnt/EFI/
ls
```

```
rd@mba: ls /mnt/
BOOTLOG     EFI     FSCK0000.REC
rd@mba: ls /mnt/EFI/
APPLE
rd@mba: cd /mnt/EFI/
rd@mab: ls
APPLE BOOT ubuntu
```
run
```
rm -rf ubuntu
sudo reboot
```
# 27. setup manpage

```
sudo apt-get update
sudo apt-get install manpages-posix

```
安装 C语言 库函数基本帮助文档:  
```
sudo apt-get install libc-dev
sudo apt-get install glibc-doc
sudo apt-get install manpages
sudo apt-get install manpages-zh
sudo apt-get install manpages-zh-dev
sudo apt-get install manpages-dev
```
安装 POSIX 函数帮助文档:  
```
sudo apt-get install manpages-posix
sudo apt-get install manpages-posix-dev
```
安装内核函数文档：
```
sudo apt-get install linux-doc
sudo apt-get install libcorelinux-dev
```
安装 C++ 帮助文档:
```
sudo apt-get install libstdc++-7-dev
sudo apt-get install libstdc++-7-doc
```
对于manpage可以直接一条命令：
```
sudo apt-get install manpages*
```

# 28. use smb to connect with windows doc sharing

ios version > 13  
file -> browser -> ...(right upper conner icon) -> connect server  
server: smb://192.168.1.123  
config user as guest or registed user.  
have fun!

# 29. 设置linux免密码登录 login linux without password      

client:192.168.0.1  
server:192.168.0.2  
on client   
```
ssh-keygen  
cd ~/.ssh/
scp id_pub.rsa user@192.168.0.2:/home/user/
```

on server
```
cat /home/user/id_pub.rsa >> ~/.ssh/authroized_keys
```
hava fun!

# 30. config ubuntu wifi driver, chinese and grub timeout input method after installed
## 30.1 wifi
```
sudo apt-get --reinstall install bcmwl-kernel-source
```
## 30.2 zh languge pack
```
sudo apt-get install  language-pack-zh-han*
sudo apt install $(check-language-support)
sudo apt install ibus-pinyin
sudo apt install ibus-libpinyin
```
## 30.3 grub time out

```
sudo vim /etc/default/grub
```
change the value of GRUB_TIMEOUT=2
```
sudo update-grub
```
close bluetooth when sys boot
```
sudo gedit /etc/rc.local
rfkill block bluetooth
```
# 31. Fn key in ubuntu
make F1 work as F1, Fn+F1 work as something else.  

```
sudo vim /etc/modprobe.d/hid_apple.conf
options hid_apple fnmode=2
sudo update-initramfs -u
```
# 32. 查看动态库so文件所在的目录

run `ldconfig -p`

# 33. git clone with shallow history

```
git clone xxxx.git --depth 1
```

# 34. fix the `/dev/loop* 100%` problem

run `sudo apt autoremove --purge snapd `

# 35. ubuntu 16.04 remote access desktop of windows 7/10
setup rdesktop first,
```
sudo apt-get install rdesktop libgssglue1
```
then run

```
rdesktop -g800*600 -a 16  192.168.1.112  // in a 800*600 windows
rdesktop -f -a 16  192.168.1.112		// full screen
```
run `crtl+alt+enter` to exit remote desktop
如果看到报错`ERROR:CREDSSP..... CredSSP required by Server`,则需要在windows上
开启远程桌面(我的电脑->属性->远程设置->允许远程连接到此计算机)时，  
取消勾选`仅允许使用网络级别身份认证...`
# 36. connect wifi via terminal on Ubuntu

查看可用wifi，
```
nmcli dev wifi
```
配置wifi，

```
nmcli dev wifi connect essid（网络名称） password password（密码）
```
# 37. get random number
```
dd if=/dev/urandom bs=1 count=16 | xxd -ps
```


# 38. dns lookup
```
dig @114.114.114.114 registry-1.docker.io
```

# 39. network interface card up down

```
ifdown eth1  /  ifconfig eth1 down 　　　　禁用网卡

ifup eth1  / ifconfig eth1 up 　　　　　　 启用网卡
```
# 40. mvn install
```
mvn install:install-file -DgroupId=com.dm -DartifactId=dmjdbc7 -Dversion=1.7.0 -Dpackaging=jar -Dfile=Dm7JdbcDriver17.jar
```

# 41. set root password
```
sudo passwd
```
# 42. start sshd service
```
sudo /etc/init.d/ssh start
```
# 43. kylin 开启root登录
```
cd /usr/share/lightm/ightm.conf.d
vi 50-unity-greeter.conf
add
greeter-show-manual-login=true   
allow-guest=false 

```
save and reboot

# 44. set IP use command
```
ifconfig 												//获取网卡名称，enp0
sudo ifconfig enp0 192.168.10.163 netmask 255.255.255.0	//set IP
sudo route add default gw 192.168.10.1					// set gateway
sudo /etc/init.d/networking stop
sudo /etc/init.d/networking start
```
# 45. ubuntu x11 forwarding

通过ssh X11 转发使用远程 GUI 程序 

client IP ：192.168.0.13  
server IP ：192.168.0.200

## 45.1 on server

```
sudo vim /etc/ssh/sshd_config 
修改或添加

X11Forwarding yes
X11DisplayOffset 10
X11UseLocalhost yes
```
restart sshd `sudo systemctl restart sshd.service`

## 45.2 on client

```
sudo vim /etc/ssh/ssh_config
修改或添加

ForwardAgent yes
ForwardX11 yes
ForwardX11Trusted yes
```
restart ssh `sudo systemctl restart ssh.service`

## 45.3 connect server with xhost
on client, run 
```
xhost +　　//允许服务器的的x11界面连接过来

ssh -X user@server_ip　　　　　　//-X参数表示转发X11数据， 把用户名称tsfh 以及服务器S的ip地址替换为你自己的
```

# 46. ubuntu firewall

```
sudo ufw status
sudo ufw disable
```

# 47. protobuf
```
protoc ./Message.proto --java_out=./

# 48. vim 空格和换行的删除和替换

 %s/\s//g

%s/\r//g

%s/\n//g
