
# 1. Setup MySQL on CentOS
## 1.1 Setup

```sh
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

```sh
配置文件：/etc/my.cnf 
日志文件：/var/log/var/log/mysqld.log 
服务启动脚本：/usr/lib/systemd/system/mysqld.service 
socket文件：/var/run/mysqld/mysqld.pid
```

## 1.3 StartUp

启动mysql服务

```sh
service mysqld restart
```

在 docker 容器中启动 MySQL

```sh
docker run --privileged -dit --name test1  centos /usr/sbin/init
docker exec -it centos bash
systemctl start mysqld
A temporary password is generated for root@localhost: eiDIxJi8s1)h
mysql -uroot -p
```

重置密码

```sh
grep password /var/log/mysqld.log
```
A temporary password is generated for root@localhost: ab*******m1

```sh
mysql -hlocalhost -uroot -p
alter user 'root'@'localhost' identified
```

# 2. Setup Redis
```sh
yum install -y epel-release
yum install -y redis

```
# 3. Install MySQL
## 3.1 Install MySQL in windows
```sh
unzip mysql-8.0.16-winx64.zipsh
config ENV for ./mysql-8.0.16-winx64/bin/
mysqld.exe --initialize-insecure
mysqld.exe --install
net start mysql
```
## 3.2 Setup MySQL on ubuntu

```sh
sudo apt-get install mysql-server
mysqld
cd /etc/mysql
cat debian.cnf
```
user = debian-sys-maint
password = xedvSNKdLavjuEWV
```sh
mysql -udebian-sys-maint -pxedvSNKdLavjuEWV
show databases;
use mysql;
update user set authentication_string=PASSWORD("自定义密码") where user='root';
update user set plugin="mysql_native_password";
flush privileges;
quit;
```
restart MySQL
```sh
/etc/init.d/mysql restart
```
# 4. 查看挂载的硬盘

```sh
fdisk -l
lsblk
```
# 5. yum on centOS  
download rpm package only
```sh
yum install --downloadonly --downloaddir=/opt/rpms mysql
```
# 6. make a ISO start up flash disk  
## 6.1 under MacOS
如果是在Mac系统下，则 需要把下载的Ubuntu安装文件（.iso）  
转换成(.dmg)格式的文件,方便在Mac OS上面进行操作，转换命令
```sh
cd Downloads/
hdiutil convert -format UDRW -o ubuntu.dmg ubuntu-14.04.5-desktop-amd64.iso
```
hdiutil转换的文件后缀名为.dmg,所以需要把文件重命名为.iso，在安装的时候系统才能够更好的识别
```sh
mv ubuntu.dmg ubuntu.iso
```

打开终端，输入  

```sh
diskutil list
```

记录下U盘的地址, 然后卸载U盘命令  
```sh
diskutil unmountDisk [硬碟位置]
```

## 6.2  Create the installation medium in linux
Either you can burn the image onto CD/DVD, you use usb stick for the installation.  
Under linux, you can use the dd for that:
```sh
dd if=<source iso> of=<target device> bs=4M; sync
```
Make sure that the device does not include partition number, so example from my machine:
```sh
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

```shell
java.lang.AssertionError: org.xml.sax.SAXParseException;
systemId: jar:file:/path/to/glassfish/modules/jaxb-osgi.jar!/com/sun/tools/xjc/reader/xmlschema/bindinfo/binding.xsd;
lineNumber: 52; columnNumber: 88; schema_reference:
Failed to read schema document 'xjc.xsd',
because 'file' access is not allowed due to restriction set by the accessExternalSchema property.
```
Create a file named jaxp.properties (if it doesn't exist) under /path/to/jdk1.8.0/jre/lib and then write this line in it:
```shell
javax.xml.accessExternalSchema = all
```
# 8. use iphone as usb internet modem in ubuntu

```shell
sudo apt-get install ipheth-utils libimobiledevice-dev libimobiledevice-utils
```
# 9. setup atom in ubuntu 16.04
## 9.1 add source

```shell
sudo add-apt-repository ppa:webupd8team/atom  
sudo apt-get update  
sudo apt-get install atom
```
## 9.2 deb package

```shell
wget https://github.com/atom/atom/releases/download/v1.43.0/atom-amd64.deb
wget https://github.com/atom/atom/releases/download/v1.7.4/atom-amd64.deb
sudo dpkg -i atom-amd64.deb
```
# 10. pandoc

```shell
sudo apt-get install pandoc
sudo apt-get install texlive-lang-cjk texlive-latex-extra texlive-xetex
pandoc test.md -o test.docx
pandoc test.md -o test.pdf
```
# 11. TCP control info
## 11.1 TCP info in ubuntu

```shell
cd /proc/sys/net/ipv4
ls -al | grep tcp
```
## 11.2 use TCP congestion algorithm BBR
update kernel if necessary and config bbr as tcp congestion algorithm

```shell
wget --no-check-certificate \    'https://github.com/teddysun/across/raw/master/bbr.sh' \   && chmod +x bbr.sh && ./bbr.sh  
```
查看是否开启
```shell
sysctl net.ipv4.tcp_available_congestion_control
```
显示以下即已开启：
```shell
sysctl net.ipv4.tcp_available_congestion_control
net.ipv4.tcp_available_congestion_control = bbr cubic reno
```
```shell
sysctl net.ipv4.tcp_congestion_control
net.ipv4.tcp_congestion_control = bbr
```
查看BBR是否启动
```shell
lsmod | grep bbr
```
显示以下即启动成功：
```shell
lsmod | grep bbr
tcp_bbr 20480 14
```
## 11.3 停止BBR  

依次执行下面命令就可以了。   

```shell
sed -i '/net.core.default_qdisc/d' /etc/sysctl.conf
sed -i '/net.ipv4.tcp_congestion_control/d' /etc/sysctl.conf
sysctl -p       
reboot
```
# 12. mac terminal hostname

```shell
sudo scutil --set HostName my_host_name
```

# 13. some git things

## setup git in offline centos

```sh
wget https://mirrors.edge.kernel.org/pub/software/scm/git/git-2.25.1.tar.gz
scp *.tar.gz root@123.456.7.8:/data/app
cd /data/app
tar -zxf git-2.25.1.tar.gz
cd /data/app/git-2.25.1
./configure
make 
# 若出现缺少 zlib.h, 则执行
sudo yum install zlib-devel -y
make install
# git 若报 fatal: unable to find remote helper for 'http'， 则执行一下语句
yum install libcurl-devel
yum install curl-devel
```



## 13.1 git中文文件名变数字
修改配置
git config --global core.quotepath false
即可解决

## 13.2 use vim diff as git diff visual tool
```shell
sudo apt-get install vim
git config --global diff.tool vimdiff
git config --global difftool.prompt false   // 不再弹出 Launch  vimdiff ?
git config --global alias.d difftool        // 为输入方便，difftool输入实在太长， 用别名 d 来替代 difftool
git d your_file                             // enjoy your coding.
```

# 14. create ISO file in ubuntu
## 14.1  create ISO file from CD-ROM
```shell
sudo umount /dev/cdrom
dd if=/dev/cdrom of=file.iso bs=1024
```
## 14.2 add file or directory to ISO file
需要使用mkisofs这个工具,你想改的参数都可以修改，而且还有-gui这个参数。最简单的用法如下：
```shell
mkisofs -r -o file.iso your_folder_name/
```
生成一个MD5文件，执行
```shell
md5sum file.iso > file.iso.md5
```
## 14.3 burn ISO file to CD-ROM
 右键, write to disc...),点击这个选项

## 14.4 Create the installation medium
Either you can burn the image onto CD/DVD, you use usb stick for the installation.  
Under linux, you can use the dd for that:
```shell
dd if=<source iso> of=<target device> bs=4M; sync
```
Make sure that the device does not include partition number, so example from my machine:
```shell
dd if=~/Downloads/alpine-standard-3.10.2-x86_64.iso of=/dev/sdb bs=4M
```
The target device will be erased, so make sure you use something without any data you do not want to lose

# 15. convert GBK(gb18030 gbk) text file to readable file in ubuntu (UTF-8 format)

```shell
 iconv -f gbk -t utf8 gbk.txt > utf8.txt
```
# 16. network traffic monitoring/网络流量监控  
```shell
iftop -i interface
```
# 17. replace tab  

## 17.1 TAB替换为空格  
```shell
:set ts=4
:set expandtab
:%retab!
```

## 17.2 空格替换为TAB  

```shell
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
```shell
deb http://archive.kylinos.cn/yhkylin juniper main restricted universe multiverse
deb http://cz.archive.ubuntu.com/ubuntu trusty main
deb [arch=amd64] https://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu trusty stable
```

保存后， 更新源：

apt-get update


# 22. ubuntu wifi driver setup (wireless card driver)  

run `lspci | grep Wireless`  
看到无线网卡类型为 
```shell
Broadcom Inc. and subsidiaries BCM4360 802.11ac Wireless Network Adapter (rev 03)
```
执行
```shell
apt install firmware-b43-installer
apt-get install bcmwl-kernel-source
```

# 23. rar and unrar in ubuntu with password
```shell
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
`/etc/default/grub`
把原来的  
`GRUB_CMDLINE_LINUX_DEFAULT=”quiet splash”`
改成  
`GRUB_CMDLINE_LINUX_DEFAULT=”quiet splash text”`
然后再运行  
`sudo update-grub ` 即可。

如果想进入图形界面，输入命令：   
`sudo lightdm`

# 26. after install ubuntu on Mac and then delete ubuntu, efi boot is redundant.

在Mac安装ubuntu后开机默认进入Grub引导，删除ubuntu后Grub引导依旧存在，  
导致每次开机都要按住option才能进入Mac系统

打开终端，挂载EFI分区
```shell
mkdir /mnt
sudo mount -t msdos /dev/disk0s1 /mnt
```
查看当前EFI分区

```shell
ls /mnt/
ls /mnt/EFI/
cd /mnt/EFI/
ls
```

```shell
rd@mba: ls /mnt/
BOOTLOG     EFI     FSCK0000.REC
rd@mba: ls /mnt/EFI/
APPLE
rd@mba: cd /mnt/EFI/
rd@mab: ls
APPLE BOOT ubuntu
```
run
```shell
rm -rf ubuntu
sudo reboot
```
# 27. setup manpage

```shell
sudo apt-get update
sudo apt-get install manpages-posix
```
安装 C语言 库函数基本帮助文档:  
```shell
sudo apt-get install libc-dev
sudo apt-get install glibc-doc
sudo apt-get install manpages
sudo apt-get install manpages-zh
sudo apt-get install manpages-zh-dev
sudo apt-get install manpages-dev
```
安装 POSIX 函数帮助文档:  
```shell
sudo apt-get install manpages-posix
sudo apt-get install manpages-posix-dev
```
安装内核函数文档：
```shell
sudo apt-get install linux-doc
sudo apt-get install libcorelinux-dev
```
安装 C++ 帮助文档:
```shell
sudo apt-get install libstdc++-7-dev
sudo apt-get install libstdc++-7-doc
```
对于manpage可以直接一条命令：
```shell
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

```shell
ssh-keygen  
cd ~/.ssh/
scp id_pub.rsa user@192.168.0.2:/home/user/
```

on server
```shell
cat /home/user/id_pub.rsa >> ~/.ssh/authroized_keys
```
另外要注意请务必要将服务器上

- ~/.ssh权限设置为700
- ~/.ssh/authorized_keys的权限设置为600
   这是linux的安全要求，如果权限不对，自动登录将不会生效



hava fun!

# 30. config ubuntu wifi driver, chinese and grub timeout input method after installed
## 30.1 wifi
```shell
sudo apt-get --reinstall install bcmwl-kernel-source
```
## 30.2 zh languge pack
```shell
sudo apt-get install  language-pack-zh-han*
sudo apt install $(check-language-support)
sudo apt install ibus-pinyin
sudo apt install ibus-libpinyin
```
## 30.3 grub time out

```shell
sudo vim /etc/default/grub
```
change the value of GRUB_TIMEOUT=2
```shell
sudo update-grub
```
close bluetooth when sys boot
```shell
sudo gedit /etc/rc.local
rfkill block bluetooth
```
# 31. Fn key in ubuntu
make F1 work as F1, Fn+F1 work as something else.  

```shell
sudo vim /etc/modprobe.d/hid_apple.conf
options hid_apple fnmode=2
sudo update-initramfs -u
```
# 32. 查看动态库so文件所在的目录

run `ldconfig -p`

# 33. git clone with shallow history

```shell
git clone xxxx.git --depth 1
```

# 34. fix the `/dev/loop* 100%` problem

run `sudo apt autoremove --purge snapd `

# 35. ubuntu 16.04 remote access desktop of windows 7/10
setup rdesktop first,
```shell
sudo apt-get install rdesktop libgssglue1
```
then run

```shell
rdesktop -g800*600 -a 16  192.168.1.112  // in a 800*600 windows
rdesktop -f -a 16  192.168.1.112		// full screen
```
run `crtl+alt+enter` to exit remote desktop
如果看到报错`ERROR:CREDSSP..... CredSSP required by Server`,则需要在windows上
开启远程桌面(我的电脑->属性->远程设置->允许远程连接到此计算机)时，  
取消勾选`仅允许使用网络级别身份认证...`
# 36. connect wifi via terminal on Ubuntu

查看可用wifi，
```shell
nmcli dev wifi
```
配置wifi，

```shell
nmcli dev wifi connect essid（网络名称） password password（密码）
```
# 37. get random number
```shell
dd if=/dev/urandom bs=1 count=16 | xxd -ps
```


# 38. dns lookup
```shell
dig @114.114.114.114 registry-1.docker.io
```

# 39. network interface card up down

```shell
ifdown eth1  /  ifconfig eth1 down 　　　　禁用网卡

ifup eth1  / ifconfig eth1 up 　　　　　　 启用网卡
```
# 40. mvn install
```shell
mvn install:install-file -DgroupId=com.dm -DartifactId=dmjdbc7 -Dversion=1.7.0 -Dpackaging=jar -Dfile=Dm7JdbcDriver17.jar
```

# 41. set root password
```shell
sudo passwd
```
# 42. start sshd service
```shell
sudo /etc/init.d/ssh start
```
# 43. kylin 开启root登录
```shell
cd /usr/share/lightm/ightm.conf.d
vi 50-unity-greeter.conf
add
greeter-show-manual-login=true   
allow-guest=false 

```
save and reboot

# 44. set IP use command
```shell
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

```shell
sudo vim /etc/ssh/sshd_config 
修改或添加

X11Forwarding yes
X11DisplayOffset 10
X11UseLocalhost yes
```
restart sshd `sudo systemctl restart sshd.service`

## 45.2 on client

```shell
sudo vim /etc/ssh/ssh_config
修改或添加

ForwardAgent yes
ForwardX11 yes
ForwardX11Trusted yes
```
restart ssh `sudo systemctl restart ssh.service`

## 45.3 connect server with xhost
on client, run 
```shell
xhost +　　//允许服务器的的x11界面连接过来

ssh -X user@server_ip　　　　　　//-X参数表示转发X11数据， 把用户名称tsfh 以及服务器S的ip地址替换为你自己的
```

# 46. ubuntu firewall

```shell
sudo ufw status
sudo ufw disable
```

# 47. protobuf
```shell
protoc ./Message.proto --java_out=./
```
# 48. vim 空格和换行的删除和替换

```shell
%s/\s//g
%s/\r//g
%s/\n//g 
```

# 49. springboot support https
## 49.1 get key
```shell
keytool -genkey -alias tomcat -keyalg RSA -keystore ./server.keystore
# input password and other information
```
create pkcs12 certificate
```shell
keytool -genkey -v -alias test_me -keyalg RSA -storetype PKCS12 -keystore dlt.p12 -dname "CN=www.achng.cn,OU=helloworld,O=KLSZ,L=BJ,ST=BJ,C=CHINA" -storepass test@test -keypass test@test
```
converto pkcs12 to jks
```shell
keytool -importkeystore -srckeystore dlt.pkcs12 -srcstoretype PKCS12 -deststoretype JKS -destkeystore dlt.jks
```
convert jks to pkcs12
```shell
keytool -importkeystore -srckeystore keystore.jks -srcstoretype JKS -deststoretype PKCS12 -destkeystore keystore.p12
```
## 49.2 add config
```properties
server.ssl.protocol=TLS
server.ssl.key-store=classpath:server.keystore
server.ssl.key-alias=tomcat
server.ssl.enabled=true
server.ssl.key-store-password=123456
server.ssl.key-store-type=JKS
```
yaml 文件格式

```yaml
server:
  ssl:
    enabled: true
    key-store: classpath:server.keystore
    key-store-type: JKS
    key-password: 123456
    key-store-password: 123456
    protocol: TLS
```

其他不需要修改，启动即可， 与更改前 只需要将http 改成https 例如原来访问是http://localhost:8081/  
更改后：https://localhost:8081/   
需要注意的是：改完后不再支持http访问，因证书原因，浏览器会报证书风险，可无视


# 50. ssh yes

要避免输入yes需要在本机上执行

```sh
vi /etc/ssh/ssh_config
```
注意这里修改的不是`sshd_config`配置文件
将

```sh
# StrictHostKeyChecking ask`
```


修改为

```sh
StrictHostKeyChecking no
# for macOS
UserKnownHostsFile /dev/null
```

即可

# 51. 16 进制查看 java class 文件

```shell
vi *.class
:%!xxd
```

# 52. 统计文件行数（不含空行）
```sh
find ./ -name *.java | xargs cat | sed '/^$/d' | wc -l
```

# JQ

jq to parse json, https://stedolan.github.io/jq/download/

# git 目录拷贝

git管理的项目，从Linux上拷贝至windows 上，会显示所有的文件都被修改，这是因为不同操作系统使用的换行符是不一样的。Unix/Linux/Mac使用的是LF，但Windows一直使用CRLF[回车(CR, ASCII 13, \r) 换行(LF, ASCII 10, \n)]作为换行符。而git入库的代码采用的是LF格式，它考虑到了跨平台协作的场景，提供了“换行符自动转换”的功能：如果在Windows下安装git，在拉取文件时，会自动将LF换行符替换为CRLF；

解决办法是禁用git的自动换行功能, 添加如下三行配置即可

```shell
vi ./.git/config
[core]
	autocrlf=false
	filemode=false
	safecrlf=false
```

##  zkclient auth

```sh
./bin/zookeeper-shell.sh IP:port
addauth digest username:password  # 输入用户名和密码
ls /
```

# JSON



java

```xml
<dependency>
		<groupId>com.alibaba</groupId>
    <artifactId>fastjson</artifactId>
    <version>1.2.83</version>
</dependency>
```



```java
import com.alibaba.fastjson.JSONObject;
import com.alibaba.fastjson.serializer.SerializerFeature
public static void main(String[] args) {
    JSONObject obj = new JSONObject(true);
    obj.put("30_晚上", "有的吃吗");
    obj.put("01_你好", "wht?");
    obj.put("02_什么", "你听说过?");
    obj.put("20_没有", "这是什么?");
    String a = obj.toJSONString();
    System.out.println(a);
    JSONObject obj1 = JSONObject.parseObject(a);
    String b = JSONObject.toJSONString(obj1, SerializerFeature.MapSortField, SerializerFeature.PrettyFormat);
    System.out.println(b);
}
```


# Python 中调用 C 的 lib.so文件

## 定义 C 函数

头文件 dtparser.h

```c
char* prs_dt(char* s);
```

函数 dtparser.c 实现

```c
#include <stdio.h>
#include <string.h>
#include "dtparser.h"

char* prs_dt(char* s) {
    static char dt[1024];
    memset(dt,0,sizeof(dt));
    strcat(dt, "hello world, ");
    strcat(dt, s); 
    return dt; 
}
```

main.c

```c
#include <stdio.h>
#include "dtparser.h"
int main(int argc, char* argv[]) {
    printf("%s\n", prs_dt(argv[1]));
    return 0;
}
```

编译，生成动态共享库文件

```sh
gcc -o main main.c dtparser.c
gcc -o libdtparser.so -shared -fPIC dtparser.c
```

测试

```
./main i_am_c
hello world, i_am_c
```

## python 调用

test.py

```python
#!/usr/bin/python3

import ctypes
import sys

def prs_dt(dt):
    '''
    调用 C 的 lib.so文件中的函数
    :param dt: a hex string
    :return: a json string
    '''
    # 定义C函数的返回类型和参数类型
    prs_dt = ctypes.CDLL("./libdtparser.so").prs_dt
    prs_dt.restype = ctypes.c_char_p
    prs_dt.argtype = ctypes.c_char_p
    # 加载.so文件
    lib = ctypes.CDLL('./libdtparser.so')
    # 调用函数
    result = prs_dt(dt.encode("utf-8"))
    # 获取结果
    # print(result)
    result_string = result.decode("utf-8")
    return result_string

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("pls input dt")
    else:
        dt = prs_dt(sys.argv[1])
        print(dt)

```

添加执行权限

```sh
chmod +x test.py
```
执行

```
./test.py i_am_python
hello world, i_am_python
```



# Mac Eclipse

## 创建 gdb codesign

https://blog.csdn.net/u010157717/article/details/51394693

按入下步骤创建代码签名的证书：

1. 打开 Keychain Access 应用程序（/Applications/Utilities/Keychain Access.app）
2. 执行菜单 **钥匙串访问(Keychain Access)** -> **证书助理(Certificate Assistant)** -> **创建证书(Create a Certificate...)**
3. 填写如下信息：
   - 名称(Name)：gdb_codesign
   - 身份类型(Identity Type)：自签名根证书
   - 证书类型(Certificate Type)：代码签名
   - 钩选：**让我覆盖这些默认设置(Let me override default)**
4. 一路确定，直到**指定证书位置**的步骤，选择**系统(System)**
5. 点击“创建”，会提示用输入系统登录密码，创建完成
6. 在**钥匙串访问程序**中，选择左侧栏的**系统**和**我的证书**，找到你刚刚创建的**gdb_codesign**证书并双击打开证书信息(Get Info)窗口，展开**信任(Trust)**项，设置**使用此证书时：**为**始终信任**。
7. 关闭**证书信息**窗口，系统会再次要求输入系统登录密码。

## code sign eclise and gdb

### Signing GDB

You can use ***Activity Monitor\*** to restart taskgated. Open it and filter the list of processes by typing `***taskgated\***` in the search field in the toolbar. (If you cannot find it, make sure the menu item ***View
 All Processes\*** is checked.)

Terminate ***Taskgate\*** process . In a few seconds, it will be restarted by the system and should reappear in the list. Please wait for this to happen (it may take up to a minute or two, at worst).

Finally, in a Terminal window, run `***codesign -s gdbcert /usr/local/bin/gdb\***` (if you named your certificate differently, replace `gdbcert` with its name here). Once again, you will be prompted for you username and password. If the command does not produce any output, then GDB is successfully signed.

### Configuring Eclipse

The only thing left to do is to point Eclipse to the GDB executable. Open ***Eclipse\*** -> ***
 Preferences\*** from the main menu . In the tree of options listed in the sidebar, navigate to ***C/C++
 Debug
 GDB\***, and set the ***GDB debugger\*** field to `/usr/local/bin/gdb`.

## Eclipse 意外退出

执行

```sh
codesign --force --deep -s gdb_codesign /Applications/Eclipse.app/
```

重启电脑

## GDB 失败

首先在 Keychain Access 中创建证书，名称为 `gdb_code_sign`, 然后点击 证书-info，在trust 中选择 “永久信任”，然后执行

```
codesign -s gdb_codesign /usr/local/bin/gdb
```

重启电脑

