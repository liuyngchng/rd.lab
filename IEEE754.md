# 国际标准

​     详见《ISO/IEC 60559:2020 Information technology — Microprocessor Systems — Floating-Point arithmetic》，

​     相关标准 《*IEEE Standard for Binary Floating Point Arithmetic (ANSI/IEEE Std 754-1985)*》

# 双精度浮点数



本节以大端（BigEndian）格式进行说明 IEEE754 中双精度浮点数的二进制表示。

## 大端和小端

* 大端（BigEndian）： byte[2] = 0X1234， 则byte[0] = 0X12, byte[1] = 0x34; 符合人类的阅读习惯；
* 小端（LittileEndian）： byte[2] = 0X1234， 则byte[1] = 0X12, byte[0] = 0x34; 符合机器的字节流；

##  符号说明

bit：二进制的位

byte：字节，一个字节为8位（bit）

func：功能

sign：双精度浮点数的符号位，一下简称s

e_sign：双进度浮点数的，以下简称es

##  科学计数法

​       把一个数表示成a与10的n次幂相乘的形式（1≤|a|<10，a不为分数形式，n为整数），这种记数法叫做**科学记数法**

例如 双精度浮点数331.46875，十进制的科学技术法表示将数字小数点左移动，直到数字在区间[1,10)中：
$$
\begin{align}
331.46875 &= 3.3146875 \times 10^2 \\
&= 3.3146875 E 2 \\
&= [(3\times 10^0).(3 \times 10^{-1}+ 1 \times 10{-2}+ 4 \times 10^{-3}+ 6 \times 10^{-4}+ 8 \times 10^{-5}+ 7 \times 10^{-6}+ 5 \times 10^{-7})]\times10^2\\
&=(3\times 10^0 + 3 \times 10^{-1}+ 1 \times 10{-2}+ 4 \times 10^{-3}+ 6 \times 10^{-4}+ 8 \times 10^{-5}+ 7 \times 10^{-6}+ 5 \times 10^{-7}) E2\tag1
\end{align}
$$
则 3.3146875 称为底数（mantissa）， 2称为指数（exponent）


##  浮点数二进制表示法

   双精度浮点数共8个字节，64 位，其中 1 位为符号位，11 位位指数（exponent，一下简称exp）位，其余的52位为底数位（mantissa，以下简称m），以BigEndian格式的 字节如表1所示，其中：

* all_bit: 64位的序号
* byte 当前属于第几个字节
* func：当前位的功能
* byte_bit：当前字节内当前位的序号
* data：所储存的数据

<center>表1 BigEndian 表示的浮点数二进制表示法示意图</center>

| all_bit      | 0      | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      | 10     | 11     | 12     | 13     | 14     | 15     |
| ------------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
| **byte**     | 0      | -      | -      | -      | -      | -      | -      | -      | 1      | -      | -      | -      | -      | -      | -      | -      |
| **func**     | s      | es     | exp    |        | -      | -      | -      | -      | -      | -      | -      | -      | m      | -      | -      | -      |
| **byte_bit** | 7      | 6      | 5      | 4      | 3      | 2      | 1      | 0      | 7      | 6      | 5      | 4      | 3      | 2      | 1      | 0      |
| data         | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 1      | 0      | 0      | 0      | 0      | 0      | 0      |
|              |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
| **all_bit**  | **16** | **17** | **18** | **19** | **20** | **21** | **22** | **23** | **24** | **25** | **26** | **27** | **28** | **29** | **30** | **31** |
| **byte**     | 2      | -      | -      | -      | -      | -      | -      | -      | 3      | -      | -      | -      | -      | -      | -      | -      |
| **func**     | m      | -      |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
| **byte_bit** | 7      | 6      | 5      | 4      | 3      | 2      | 1      | 0      | 7      | 6      | 5      | 4      | 3      | 2      | 1      | 0      |
| data         | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      |
|              |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
| **all_bit**  | **32** |        |        |        |        |        |        |        |        |        |        |        |        |        |        | **47** |
| **byte**     | 4      | -      | -      | -      | -      | -      | -      | -      | 5      | -      | -      | -      | -      | -      | -      | -      |
| **func**     |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
| **byte_bit** | 7      | 6      | 5      | 4      | 3      | 2      | 1      | 0      | 7      | 6      | 5      | 4      | 3      | 2      | 1      | 0      |
| **data**     | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      |
|              |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
| **all_bit**  | 48     |        |        |        |        |        |        |        |        |        |        |        |        |        |        | 63     |
| **byte**     | 6      | -      | -      | -      | -      | -      | -      | -      | 7      | -      | -      | -      | -      | -      | -      | -      |
| **func**     |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
| **byte_bit** | 7      | 6      | 5      | 4      | 3      | 2      | 1      | 0      | 7      | 6      | 5      | 4      | 3      | 2      | 1      | 0      |
| data         | 0      | 0      | 0      | 0      | 1      | 0      | 0      | 1      | 0      | 1      | 0      | 0      | 1      | 1      | 1      | 1      |

## 十进制到二进制的换算

  牵扯到不同的进制换算，为了清晰起见，每个数字添加头部表示，以示区分。 

以0x（0X）开头的数为16进制， 以 0b（0B）开头的数为2进制，以0D开头的数位10进制数    

​    依然以1.3节的 331.46875 为例，进行说明。
$$
\begin{align}
0D331.459 &= 3.3146875 \times 10^2 \\
&= 3.3146875 E 2\tag2
\end{align}
$$
十进制31换算为2进制，
$$
\begin{align}
0D331&=256+ 64+11 =1\times16^2+4\times16^1+ 11 = 0X14B \\
&= 0B000101001010\tag3
\end{align}
$$
十进制 0.46875 换算为 2 进制
$$
\begin{align}
0D0.459&=0D[0\times 2^{0}].[0\times 2^{-1} + 1\times2^{-2} + 1\times 2^{-3} +1\times2^{-4}+1\times2^{-8}]\\
&=0B0.01111\tag4
\end{align}
$$
于是将公式（3）和（4）相加，得到公式（5）：
$$
\begin{align}
0D331.459 &= 0D331 +0D0.459 \\
&= 0B000101001010 +0B0.01111\\&= 0B000101001010.01111\\&= 0B0001.0100101001111\times2^8
\\&= 0B1.0100101001111E1000\tag5
\end{align}
$$
由于二进制的特殊性，小数点前面的数永远位1，因为能够落在区间[1, 2)的数字只有1，因此在后续表示底数的时候，直接忽略掉小数点前的数字，即共识（5）中在计算底数和指数时，计：

```sh
（1）底数 mantissa=0100101001111					# 注意，此处忽略了小数点前的1
（2）指数 exponent=1000
```

## 双精度（64bit）浮点数二进制表示法

如表1所示， 

* 0B1.0100101001111E1000 为正数，所以 s=0，即all_bit[0] = 0; 
* 指数 exponent=100 为正数， es=0，即all_bit[1]=0；
*  all_bit[2-11]=2<sup>10</sup>- 0B0000000100。为什么这里突然冒出个2<sup>10</sup>，后面再解释。
* all_bit[12-63]， 共计52位，表示的数字为 底数 mantissa=0100101001111	

# 代码实现

## Java

根据浮点数定义，自行实现的解析方法如下所示。

```java
public static double getDouble(byte[] b) {
    final int EXP_LEN = 11;
    final int sign = (0b10000000 & b[0]) == 0 ? 1 : -1;
    final int exponent = (0b011111110000 & (b[0] << 4)) + 0b00001111 & (b[1] >> 4);
    final int pow = (int)(exponent - (Math.pow(2, EXP_LEN -1) -1));
    double mantissa = 1;
    // 由于double 不支持右移操作，所以只能一步一步计算
    mantissa += (0b00001111 &b[1]) / Math.pow(2, 4);
    for (int i = 2; i < 8; i++) {
        mantissa += (0xff & b[i]) / Math.pow(2, (i- 1)* 8 + 4);
    }
    System.out.println(String.format("sign=%d, exponent=%d, pow=%d, mantissa=%1.20f", sign, exponent, pow, mantissa));
    return sign * mantissa * Math.pow(2, pow);
}
```

 JDK 本身提供了native 实现的类方法, hex为16进制的字符数组表示形式。

```java
public static double parseDouble(String hex) {
    return Double.longBitsToDouble(Long.parseLong(hex, 16));
}
```

##  C

```c
#include <stdio.h>
#include <string.h>
/**
 * 输出一个字节的2进制字符串
 */
void print_bin(char c) {
	for (int i = 0; i < 8; i++) {
		printf("%d", ((c<<i) & 0b10000000) >> 7);
	}
}

int hexcharToInt(char c)
{
	if (c >= '0' && c <= '9') return (c - '0');
	if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
	if (c >= 'a' && c <= 'f') return (c - 'a' + 10);
	return 0;
}
//
void hex2bin(unsigned char *bin, const char *hex) {
	int i = 0;
	for (i = 0; i < strlen(hex); i += 2) {
		bin[i / 2] = (char)((hexcharToInt(hex[i]) << 4) | hexcharToInt(hex[i + 1]));
        printf("bin[%d]=%02X\n", i/2, bin[i/2]);
	}
}

int main()
{
	unsigned long x = 0x40E743CD1BF68000;
	printf("unsigned long x=%lX\n", x);
	double *a = (double*)&x;
	printf("double a=%f, sizeof(a)=%lu\n", *a, sizeof(a));
	for (int i = sizeof(a) - 1; i >= 0; i--) {
		char *p = (char*)a;
		unsigned char c = *(p+i);
		printf("byte%d=", i);
		print_bin(c);
		printf(" 0X%02X\n", c);
	}
	printf("\n");
	unsigned char b=0xFF;
	printf("unsigned char b = %d, 0X%X, 0B", b, b);
	print_bin(b);
	printf("\n");
	const char *y = "40E743CD1BF68000";
	printf("literal hex str: char *y = %s, strlen(y)=%lu\n", y, strlen(y));
	for(int i = 0; i< strlen(y); i++) {
		printf("byte%d=%c\n", i, *(y+i));
	}
	unsigned char z[8];
    printf("copcy hex str char *y to bin str char *z\n");
	hex2bin(z, y);
	size_t size = strlen((const char*)z);
	printf("strlen(z)=%lu\n", size);
	printf("bin char str: unsigned char *z=");
	for (int i = 0; i < 8; i++) {
		printf("%02X", *(z+i));
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		unsigned char c = *(z+i);
		printf("byte%d=", i);
		print_bin(c);
		printf(" 0X%02X\n", c);
	}
	double *c = (double*)z;
	printf("double c=%f\n", *c);
	unsigned char dz[8];
	for(int i = 0; i < 8; i++) {
		*(dz+i) = *(z + 7 -i);
	}
	printf("bin char str in small end: unsigned char *dz=\n");
	for (int i = 7; i >=0; i--) {
		printf("%02X", *(dz+i));
	}
    printf("\ndouble value of *dz=%f\n", *(double *)dz);
}
```

