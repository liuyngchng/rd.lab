# 开发者视角

​       UUID 是蓝牙从设备（slave）连接蓝牙主设备（master）的必须的参数，相当于服务端开发中的端口号，代表设备中的某个服务。

   （1）只要 <font color='red'>主从设备协商好指定的UUID</font> 即可，是否需要通过蓝牙联盟分配不重要。（需开发实践进一步验证）

   （2）通过蓝牙 SDP 协议（Service Discovery protocol），从设备从主设备获取服务清单，只要 <font color='red'>主从设备按照蓝牙协议的默认约定</font>，使用某一指定类型的服务，即可实现通信，无需蓝牙联盟分配。（需开发实践进一步验证）

# 利益相关者（商业）视角

​     从蓝牙联盟申请一个 UUID ，说明公司是一个蓝牙生态的深度参与者，具有一定商业价值。

​     另外，使用蓝牙标识需要得到蓝牙联盟的授权。

# 标准制订视角

​      从蓝牙联盟申请一个 UUID 表示组织机构是蓝牙相关标准的参与方。个人理解这个标准制订可能是蓝牙标准范畴内的。

# 一般蓝牙使用者视角

无需申请，只要实现业务功能即可。

# 大公司（跨国公司等）

​    通过申请蓝牙 UUID， 对外界宣布自己的独有 UUID。对其周边蓝牙设备的开发，以及蓝牙设备之间的互联互通，具有正面意义。

否则，可能会出现各个蓝牙设备之间无法互相通信。

# 蓝牙规范

## 蓝牙核心规范

​     蓝牙核心规范（Bluetooth Core Specification）5.3版（V5.3）详见 https://www.bluetooth.com/specifications/specs/core-specification-5-3/。

## UUID 简介

​     在文档的第三卷（Vol 3: Host）的Part B 服务发现协议规范（SERVICE DISCOVERY PROTOCOL (SDP) SPECIFICATION）中的

2.5节 服务搜索（SEARCHING FOR SERVICES）中对于UUID的介绍如下。

```
A UUID is a universally unique identifier that is expected to be unique across all space and all time (more precisely, the probability of independently-generated UUIDs being the same is negligible). UUIDs can be independently created in a distributed fashion. No central registry of assigned UUIDs is required. A UUID is a 128-bit value.
To reduce the burden of storing and transferring 128-bit UUID values, a range of UUID values has been pre-allocated for assignment to often-used, registered purposes. The first UUID in this pre-allocated range is known as the Bluetooth_Base_UUID and has the value 00000000-0000-1000-8000- 00805F9B34FB. UUID values in the pre-allocated range have aliases that are represented as 16-bit or 32-bit values. These aliases are often called 16-bit and 32-bit UUIDs, but each actually represents a 128-bit UUID value.
```

翻译如下

```
   UUID 是一个能够在所有时间和空间全局唯一的 ID（或者更精确地描述，独立产生相同的UUID的可能性基本上可以忽略）。UUID可以已一种分布式的方式独立创建。不需要管理中心去分配 UUID。UUID是一个128位的值（二进制的128位，译者注）。
   为了去除存储和转换128位二进制 UUID 值的负担，某个范围的 UUID 值被预先生成并分配用于日常使用和注册。第一个被预先生成的 UUID 被称为蓝牙基准 UUID （Bluetooth_Base_UUID），其值为 00000000-0000-1000-8000- 00805F9B34FB （16进制形式，译者注）。在预分配范围内的的 UUID 值通过一个 16 位 或 32 位（2进制位数，译者注）的值作为别名，这些别名通常称为 16 位或 32 位UUID，但是实际上每个别名都代表的是一个 128 位（2进制位数）的 UUID 值。
```

## UUID 别名和真实值的转换

​       蓝牙核心规范在2.5节 服务搜索（SEARCHING FOR SERVICES）中对于UUID的转换说明如下。

```
The full 128-bit value of a 16-bit or 32-bit UUID may be computed by a simple arithmetic operation.
128_bit_value = 16_bit_value * 2^96 + Bluetooth_Base_UUID 
128_bit_value = 32_bit_value * 2^96 + Bluetooth_Base_UUID
A 16-bit UUID may be converted to 32-bit UUID format by zero-extending the 16-bit value to 32-bits. An equivalent method is to add the 16-bit UUID value to a zero-valued 32-bit UUID.
```

翻译如下

```
16 位或 32 位别名UUID的真实 128 位 UUID，可以通过简单的算术运算计算出来，公式如下
128 位 UUID = 16 位别名值 * 2 的 96 次方 + 蓝牙基准 UUID
```

​      以下为个人理解的实际操作，以蓝牙官方文档（https://btprodspecificationrefs.blob.core.windows.net/assigned-values/16-bit%20UUID%20Numbers%20Document.pdf）中预分配的串口协议的 UUID 别名 0x1101 为例进行计算示范。

```
 0x1101 * 2 ^ 96 + 00000000-0000-1000-8000- 00805F9B34FB
```

乘 2^96 ，表示2进制左移 96 位；2^96 = 16^24， 即 16 进制左移 24位。

```sh
#bit 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
                                                                                      0x 1  1  0  1
             0x   1  1  0  1                                                    <-- 左移 24位 
# 蓝牙基准 UUID
   0x 0  0  0  0  0  0  0  0- 0  0  0  0- 1  0  0  0- 8  0  0  0- 0  0 8  0  5  F  9  B  3  4  F  B
# 相加后
   0x 0  0  0  0  1  1  0  1- 0  0  0  0- 1  0  0  0- 8  0  0  0- 0  0 8  0  5  F  9  B  3  4  F  B
```

于是，串口服务（SerialPort）UUID别名为 0x1101 对应的真实128位 UUID 的16 进制表示数值为

```
0x00001101- 0000- 1000- 8000- 00805F9B34FB
```

# UUID 分配(Requesting Assigned Numbers)

##  分配简介

​       蓝牙 UUID 由蓝牙技术联盟（*Bluetooth* Technology Special Interest Group，简称BLE SIG， 直译为蓝牙技术特别兴趣小组）进行授权。

​       自定义蓝牙服务时要申请UUID,每个 UUID 2,500 美元，详见 https://support.bluetooth.com/hc/en-us/articles/360058202612-Bluetooth-SIG-Membership-Fee-Changes-Effective-1-January-2022。

已分配的 UUID 详见 https://www.bluetooth.com/specifications/assigned-numbers/。

​      蓝牙官网关于公司身份 UUID 分配的介绍，详见 https://support.bluetooth.com/hc/en-us/articles/360062030092-Requesting-Assigned-Numbers。

​    UUID 分配，分为以下几类， 

* 公司身份 （Company Identifiers）UUID 

* 蓝牙技术联盟成员UUID （16-bit UUIDs for Members）

* 标准研发组织 UUID （16-bit UUIDs for Standards Development Organizations(SDOs)）

* 客户自定义 UUID （Custom Characteristic UUIDs）

* 创建唯一的128位 UUID （Create a unique 128-bit UUID）

## 公司身份 （Company Identifiers）UUID 

​     蓝牙官网描述如下

```
Company identifiers are unique numbers that the Bluetooth SIG assigns to member companies that request them. Each Bluetooth SIG member that is assigned a Company Identifier may use the assigned value for any or all of the following: 

(1)  LMP_CompID (refer to the Bluetooth® Core Specification) 
(2) Company Identifier Code used in Manufacturer Specific Data type used for EIR and Advertising Data Types (refer to CSSv1 or later) 
(3) Company ID for vendor-specific codecs (refer to Vol. 2, Part E, of the Bluetooth Core Specification, v4.1 or later) 
(4) As the lower 16 bits of the Vendor ID for designating Vendor Specific A2DP Codecs (refer to the A2DP v1.3 or later) 
(5) VendorID Attribute in Device ID service record (when VendorIDSourceAttribute equals 0x0001, refer to Device ID Profile) 
(6) 802.11_PAL_Company_Identifier (refer to Bluetooth Core Specification v3.0 + HS or later) 
(7) TCS Company ID (refer to Telephony Control Protocol [WITHDRAWN]) 
```

翻译如下

```
公司身份标识符是由蓝牙技术联盟（BLE SIG）向发起申请身份标识符的成员公司的一串数字。每一个被赋予一个公司身份ID的蓝牙技术联盟成员可以使用这串数字进行以下活动：
（1）LMP_CompID， 参考蓝牙核心规范（LMP，Link Manager Protocol，是用来控制和协商两个设备连接行为的协议，涉及的方面包括逻辑传输连接的建立和控制，以及对于物理链路的控制等等，它是两个设备的LMP模块之前的交流，其消息是传输在ACL-C 的逻辑链路上，详见蓝牙核心规范 Figure 2.1: Bluetooth core system architecture， 译者注）；
（2）公司身份编码可用于在 EIR（Extended Inquiry Response（译者注），参考 CSSv1 或更高版本） 和广播数据类型中使用的制造商规格数据；
（3）公司身份 ID 可用于发行商相关的编解码（参考蓝牙核心规范的第2卷 E 部分，版本为v4.1或更高版本）；
（4）作为发行商 ID 的低 16 位，作为发行商规范 A2DP 编解码设计内容的一部分（参考 A2DP v1.3版或更高版本）；
（5）作为设备 ID 记录中的发行商ID（当发行商ID源属性（VendorIDSourceAttribute）等于0x0001时，指的是 设备 ID 全貌）；
（6）作为8.2.11 PAL 公司身份标识符（参考蓝牙核心规范 v3.0 + HS 版本或更高版）；
（7）TCS（Telephony Control Specification， 蓝牙电话控制规范） 公司 ID （参考电话控制协议（已撤回））。

```

##  蓝牙技术联盟成员UUID

官网介绍如下, 明确提出了付款

```
To request a 16-bit UUID (Universally Unique Identifier) for Members, submit the following to  Bluetooth Support: 

(1) Company name:
(2) Company contact:
(3) Company contact email address:​ 
(4) Company address:
(5) The number of 16-bit UUIDs requested:
(6) For each UUID requested, provide:  
    1) Custom service name:
    2) Detailed custom service description:
        a) that explains the purpose of the service while also describing at least one use case that illustrates how the service may be used  
SIG staff will review the application and provide instructions on how to accept the Terms of Use and how to complete the payment requirements. When the signed  Terms of Use and full payment are received, the 16-bit UUID(s) will be published and maintained in the 16-bit UUID Numbers Document. SIG staff will notify the member when the 16-bit UUID(s) have been allocated.
SLA: Once the 16-bit UUID payment has been received by the SIG, your UUID will be published to our Assigned Numbers page within 10 business days. 
Note: Bluetooth SIG can not determine how long the 16-bit UUID payment process will take. The length of the transaction process depends on the member company's banking system. 
```

翻译如下

```
申请一个 16 位的蓝牙技术联盟 UUID，需要提交以下材料至蓝牙技术联盟：
（1）公司名称；
（2）公司联系方式；
（3）公司联系 e-mail；
（4）公司地址；
（5）请求的 16 位 UUID；
（6）若申请多个 UUID，对于申请的每一个 UUID，需要提供以下材料
    1）自定义服务名称；
    2）自定义服务的详细描述，在详细描述中需要解释自定义服务的用途，同时至少需要提供一个使用场景来说明这个服务是如何被使用的。
蓝牙技术联盟的工作人员将审核这些申请，同时提供一个如何接受使用法律条款以及完成相应支付要求的指导说明。当申请人签署了使用法律条款，并且蓝牙技术联盟收到了全额支付的款项后，16 位的 UUID 将在公开渠道公布和并在 16 位 UUID 的文档中维护。 在 16 位 UUID 分配完成后，蓝牙技术联盟的工作人员将会通知申请人，
```

## 标准研发组织 UUID

官网介绍，

```
How to request 16-bit UUIDs for SDOs
For guidance on the process and documentation associated with new assignments, Standards Development Organizations requesting the assignment of a 16-bit UUID should submit a request to Bluetooth Support.  
When submitting a request, select:
Category: Assigned Numbers
Subcategory: 16 bit UUIDs for SDOs
```

翻译如下。

```
标准研发组织（SDO）如何申请 16 位的 UUID？
   为了指导 UUID 分配的过程以及相关的文档， 标准研发组织请求请求 16 位 UUID 应向蓝牙技术联盟支持工作组（蓝牙在线服务网站）提交申请。
   当提交申请时，应选择 “申请号码” 大类，以及 “标准研发组织 16 位 UUID” 子类。
```

## 客户自定义 UUID 

官网介绍

```
If a proprietary characteristic requires a UUID, a member may generate their own 128-bit UUID. 

16-bit UUIDs are only to be used to represent services. 16-bit UUIDs cannot be used to generate characteristic UUIDs.  

KB0010305
```

翻译如下。

```
如果在某些特定条件下需要一个 UUID， 客户可以自行生成自己的 128 位 UUID。
16位的 UUID 只是用于代表服务，但不能用于生成特定的 UUID。
```

## 创建唯一的 128 位 UUID

官网介绍

```
(1) The International Telecommunication Union (ITU) ITU-T Rec. X.667 document describes the structure of a valid unique 128-bit UUID. To download a free copy of this document, visit the ITU website: ITU-T Rec. X.667（http://www.itu.int/ITU-T/studygroups/com17/oid/X.667-E.pdf）
(2) To generate a 128-bit UUID, visit the UUIDs page of the ITU website（https://www.itu.int/en/ITU-T/asn1/Pages/UUID/uuids.aspx） and select Get a new UUID.
    Note: The information contained in this section is used as a reference only, and a user may use different standards and documents that describe the structure and creation of UUIDs.

KB0010168
```

翻译如下

```
（1）在国际电信联盟（ITU）的 ITU-T Rec. X.667 文件描述了一个有效的全局唯一 128 位 UUID的组成。可以通过访问 ITU 官网（http://www.itu.int/ITU-T/studygroups/com17/oid/X.667-E.pdf）免费下载一份文件拷贝。
（2）如果想很方便地生成UUID，可以访问 ITU 官网，在UUID页面（https://www.itu.int/en/ITU-T/asn1/Pages/UUID/uuids.aspx）选择 “获取一个新的 UUID”。
    注：以上方法仅供参考，用户可以自由选择创建 UUID 的不同的标准和相应的文件。
```

