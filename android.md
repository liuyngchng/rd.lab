# 1. **dev**

官网下载安装，在启动的时候会自动下载Android SDK 

```sh
~/Android/Sdk
```

也可以自己手动下载cmdlinetools 手动下载 Android SDK ， 然后在 Android Studio 中的菜单[tools] -> [SDK Manager] 指定 SDK 的位置



# 2. package

**（1）生成密钥**

发布release 版本apk，apk包必须要有密钥签名，可通过如下命令生成密钥

```sh
keytool -genkeypair -v  \
	-keystore rd-app-release.jks  \
	-keyalg RSA -keysize 2048 \
	-validity 10000 -alias rd-app \
	-storepass "your_password"  \
	-keypass "your_password"  \
	-dname "CN=rdApk, OU=rdAndroid, O=rdForkedApkTeam, L=Beijing, ST=Beijing, C=CN"
```

**（2）配置 local.properties**

```sh
# Android SDK目录
sdk.dir=/home/rd/Android/Sdk
# 签名配置，文件路径，使用绝对路径
KEYSTORE_FILE=/home/rd/AndroidStudioProjects/rd_app/rd-app-release.jks
KEYSTORE_PASSWORD=your_actual_password
KEY_ALIAS=apkclaw
KEY_PASSWORD=your_actual_password
```

**（3）开始编译**

```sh
# 先 clean 之前的编译文件，然后生成release 包
./gradlew clean assembleRelease
```

如果需要配置代理下载相关依赖，可以在文件 gradle.properties 中进行配置

```sh
vi gradle.properties
# 添加代理配置
systemProp.http.proxyHost=proxy.your_host.domain
systemProp.http.proxyPort=8080
systemProp.https.proxyHost=proxy.your_host.domain
systemProp.https.proxyPort=8080
```

最终会在目录下生成打包好的 apk 文件。

```sh
app/build/outputs/apk/release/
```

**（4）查看已经连接的设备**

```sh
# usb 连接手机后，查看已经连接的设备
adb devices 
List of devices attached
JEEECN0100000712        device
```

有时候，这里会报 no permissions

```sh
rd@rd-t14:~/Android/Sdk/platform-tools$ ./adb devices
List of devices attached
JEEECN0100000712	no permissions (missing udev rules? user is in the plugdev group); see [http://developer.android.com/tools/device.html]

rd@rd-t14:~/Android/Sdk/platf
```

进行如下操作

```sh
lsusb
Bus 003 Device 014: ID 0e8d:201c MediaTek Inc. Jelly 2E

# 这里 0e8d 就是厂商 ID，201c 是产品 ID, 后续会用到
```

创建并编辑 udev 规则文件，使用以下命令创建或编辑规则文件（通常命名为 `51-android.rules`）：

```bash
sudo vim /etc/udev/rules.d/51-android.rules
```

写入规则，在打开的文件中，粘贴下面这行内容，并将 `0e8d` 和 `201c` 替换成你刚才记录下的设备 ID：

```sh
SUBSYSTEM=="usb", ATTR{idVendor}=="18d1", ATTR{idProduct}=="4ee7", MODE="0666", GROUP="plugdev"
```

应用新规则：保存并退出编辑器后，执行以下命令使新规则生效，并重启 ADB 服务：

```sh
sudo udevadm control --reload-rules
sudo udevadm trigger
sudo adb kill-server
sudo adb start-server
```

验证结果：重新拔插 USB 数据线，然后再次运行 `adb devices` 命令。此时，如果一切正常，设备状态应该从 `no permissions` 变为 `device`，表示已成功连接并获得授权。

```sh
adb devices
```

**（5）安装 apk 包到手机**

```sh
# 如果安装时，报没有签名，可以使用 apksigner 对已经打完的包进行签名
# 需要 Android SDK build-tools
~/Android/Sdk/build-tools/37.0.0/apksigner sign --ks ~/termux-release.jks --ks-key-alias termux termux-app_apt-android-7-release_arm64-v8a.apk

# 验证签名
~/Android/Sdk/build-tools/30.0.3/apksigner verify termux-app_apt-android-7-release_arm64-v8a.apk

# 安装
adb install termux-app_apt-android-7-release_arm64-v8a.apk
```



