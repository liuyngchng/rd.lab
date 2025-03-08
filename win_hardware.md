# 获取信息

##  查看内存信息

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


##   查看内存卡槽数量及支持的最大容量

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



##  看已使用的插槽

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

# 机器信息

##  Lenovo T14

### 内存



（1）插槽数量：2个

（2）插槽使用情况：目前使用了1个插槽

（3）目前内存大小： 16GB  主频3200MHz

（4）内存型号：SK hynix

（5）内存串号：HMAB2GS6AMR6N-XN

### 硬盘

M.2接口2280 NVMe协议 PCI-E 4.0x4

硬盘型号

```sh
ubuntu
cat /sys/block/nvme0n1/device/model 
SKHynix_HFS512GDE9X081N
```

