# 1. cmd

## 1.1 查看端口占用

```powershell
# 获取进程ID
netstat -ano |findstr 9222
# 根据进程 ID 查看进程信息
tasklist | findstr "1912"
```



## 1.2 chrome启动debug

```powershell
start "" "C:\Program Files\Google\Chrome\Application\chrome.exe" --user-data-dir="C:\tagui\src\chrome\tagui_user_profile" --remote-debugging-port=9222 about:blank --window-size=1366,842


"C:\Program Files\Google\Chrome\Application\chrome.exe" --user-data-dir="C:\tagui\src\chrome\tagui_user_profile" --remote-debugging-port=9222 about:blank --window-size=1366,842
```

## 1.3 查询命令使用方式

```powershell
cmd /?
```

## 1.4 查询命令列表

```
help
```

# 2. OpenSSH

## 2.1 启用

对于windows10， 设置->应用->管理可选功能->OpenSSH 服务器， OpenSSH客户端

## 2.2 修改默认shell

将默认的shell 从cmd 修改为 powershell

Windows 初始默认 Shell 是 cmd，Windows 还包括了 PowerShell 和 Bash。

先确认 OpenSSH 安装文件夹位于系统路径上，对于 windows，默认安装位置是：`C:Windows\System32\openssh`。

cmd 中 `path`和 PowerShell 中 `$env:path`可以显示当前路径。

在注册表中，将 PowerShell 的完整路径添加到 `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\OpenSSH`中的 `DefaultShell`字符串值中。

先确保注册表中存在 `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\OpenSSH`这个路径，然后可以在 PowerShell 中用以下命令将默认 shell 设为 PowerShell：

```powershell
New-ItemProperty -Path "HKLM:\SOFTWARE\OpenSSH" -Name DefaultShell -Value "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" -PropertyType String -Force
```

# 3. 获取信息

##  3.1 查看内存信息

windows 控制台（win+R，输入cmd）中执行

```
wmic memorychip list full
```

看到结果

```powershell
BankLabel=BANK 0                             # 插槽名称
Capacity=17179869184                         # 容量
DataWidth=64                                 # 64位
Description=物理内存
DeviceLocator=Controller0-ChannelA
FormFactor=12
HotSwappable=
InstallDate=
InterleaveDataDepth=
InterleavePosition=
Manufacturer=SK Hynix                        # 品牌
MemoryType=0
Model=
Name=物理内存
OtherIdentifyingInfo=
PartNumber=HMAB2GS6AMR6N-XN                  # 部件号码
PositionInRow=
PoweredOn=
Removable=
Replaceable=
SerialNumber=00000000
SKU=
Speed=3200                                   # 主频 单位，MHz
Status=
Tag=Physical Memory 0
TotalWidth=64
TypeDetail=128
Version=
```


##   3.2 查看内存卡槽数量及支持的最大容量

执行

```powershell
wmic Memphysical get MaxCapacity, MemoryDevices
```


看到

```powershell
# 所有卡槽支持的最大容量（单位KB）   插槽数量
MaxCapacity                    MemoryDevices
 67108864                         2 
```



##  3.3 看已使用的插槽

执行

```
wmic MemoryChip get BankLabel, DeviceLocator, Capacity
```


看到

```
# BankLabel  出现几行说明使用了几个插槽
# Capacity 表示当前插槽中的内存大小（单位：Byte）
BankLabel  Capacity     DeviceLocator
BANK 0     17179869184  Controller0-ChannelA
```

# 4. 机器信息

##  4.1 Lenovo T14

### 4.1.1 内存



（1）插槽数量：2个

（2）插槽使用情况：目前使用了1个插槽

（3）目前内存大小： 16GB  主频3200MHz

（4）内存型号：SK hynix

（5）内存串号：HMAB2GS6AMR6N-XN

### 4.1.2 硬盘

M.2接口2280 NVMe协议 PCI-E 4.0x4

硬盘型号

```sh
ubuntu
cat /sys/block/nvme0n1/device/model 
SKHynix_HFS512GDE9X081N
```

# 5. USB SSD Windows Setup

操作起来其实和你用U盘装系统给电脑硬盘差不多，但有一个关键区别。这里以最推荐的免费工具 **Rufus** 为例，为你梳理一下步骤。

## 5.1 准备

一定要使用USB 3.0或更高版本的**移动固态硬盘（SSD）**。普通U盘速度太慢，会导致系统卡顿，严重影响使用体验 。容量建议至少64GB 。从微软官网下载Windows系统镜像（ISO文件）。下载最新版Rufus软件。

## 5.2 操作

将移动硬盘通过USB接口连接到电脑，打开Rufus软件，它就会自动识别出你的移动硬盘，在"设备"栏，**再次确认**选择的是你的移动硬盘，千万选错，否则会清空其他硬盘的数据。

点击"选择"，找到并选中你下载好的Windows ISO文件。在"映像选项"这个下拉菜单中，不要选默认的"标准Windows安装"，而是要手动选择 **"Windows To Go"** （WTG）**关键**。其他选项（如分区类型）保持默认即可，然后点击"开始"。

注意事项：

1） 如果Rufus 弹出： rufus检测到您正在使用1809版本的iso文件创建 Windows to go驱动器。由于微软的程序故障，本驱动器启动windows时将出现蓝屏崩溃，可除非您手动将 WppRecorder.sys 文件替换为1803镜像中的版本， 请在微软官网下载最新版本的Windows 10, 老版本的 Windows 10 存在一些bug。

2）如果Rufus 弹出：Rufus 检测到您选择的iso包含一个已经吊销的UEFI引导加载器，当在最新的UEFI 系统上起用安全启动时，将产生一个 安全违规界面。忽略即可，只要启动电脑关闭 Secure Boot 选项不受影响。

## 5.3 启动

把移动硬盘插到目标电脑上，开机时按特定键（通常是F2、F12、Del等）进入BIOS/UEFI设置，将USB移动硬盘设为第一启动项，保存退出后，电脑就会从你的移动硬盘启动了 。

- **驱动自适应**：当你第一次在新电脑上启动时，Windows会自动检测并安装这台电脑所需的驱动程序（如网卡、显卡），这个过程可能需要一点时间，通常重启一次即可 。

- **系统性能**：系统的流畅度主要取决于你的移动硬盘速度和你当前使用的电脑配置 。

- **数据安全**：默认情况下，WTG系统会**自动将电脑的内置硬盘设置为脱机状态**，也就是说你从移动硬盘启动后，是看不到那台电脑自己硬盘里的文件的，这可以防止不小心把数据遗留在别人的电脑上，或者自己的系统被意外修改 。

## 5.3 异常情况

**（1）内置硬盘是双系统。**如果你的电脑内置硬盘已经安装了Ubuntu + Windows 的双系统则通过 WTG 外置硬盘启动后，拔掉 USB SSD启动， 系统会直接进入内置硬盘的 Windows， 而不是之前的 Ubuntu 引导界面，此时，在Windows 中 执行以下命令即可。

```sh
# 设置Windows启动管理器（Boot Manager）应该去加载哪个引导文件
bcdedit /set {bootmgr} path \EFI\ubuntu\shimx64.efi
# 允许Windows启动管理器在屏幕上显示菜单
bcdedit /set {bootmgr} displaybootmenu yes
```

此时，重启会重新弹出 Ubuntu 的引导界面。

内置硬盘是单一系统的，不管是Windows ，还是 Ubuntu，WTG不会对原系统造成任何影响。
