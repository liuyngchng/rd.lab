# 1. intro
[ish.app](https://ish.app)
iSH是一个使用usermode x86模拟器将Linux shell引入IOS设备的工具，基于Alpine Linux，  
该程序占用空间小，具备一定的安全性且易于上手。不过目前iSH还处于测试阶段，部分功能还不完善。
[csdn_url](https://blog.csdn.net/xyzAriel/article/details/105497710)

# 2. setup ish on ios
在苹果应用市场App Store搜索TestFlight，获取-安装。安装好后打开，如下图所示。

复制链接： https://testflight.apple.com/join/97i7KM8O ，在Safari浏览器打开，浏览器会自动跳转打开TestFlight，在弹出的对话框中点击“开始测试”（Start Testing），如上图所示，就可以收到加入iSH测试版的邀请了。

# 3. cmd
apk update                    更新源
apk info				    列出当前已安装的软件包
apk search <query>            搜索可安装包
apk add <package>             安装软件包
apk upgrade                   升级软件包
apk list [--installed]        列出所有的软件包信息
apk del <package>             删除软件包
chmod                         修改文件权限
chown                         修改文件归属

# 4. config

配置iSH

（1）更新国内源
Alpine的源文件为：/etc/apk/repositories，默认的源地址为：http://dl-cdn.alpinelinux.org/，可以将其更新为国内源。
编辑源文件，输入命令
```
vi /etc/apk/repositories
```
1
采用国内阿里云的源，将其文件内容修改为：

```
# http://dl-cdn.alpinelinux.org/alpine/v3.11/main/
# http://dl-cdn.alpinelinux.org/alpine/v3.11/community/
https://mirrors.aliyun.com/alpine/v3.11/main/
https://mirrors.aliyun.com/alpine/v3.11/community/
```
apk update
apk upgrade
