# 国际标准

​     详见《ISO/IEC 60559:2020 Information technology — Microprocessor Systems — Floating-Point arithmetic》，

​     相关标准 《*IEEE Standard for Binary Floating Point Arithmetic (ANSI/IEEE Std 754-1985)*》

# 双精度浮点数



本节以`大端`（`BigEndian`）格式进行说明 IEEE754 中双精度浮点数的二进制表示。

## 大端和小端

* `大端`（`BigEndian`）： byte[2] = 0X1234， 则byte[0] = 0X12, byte[1] = 0x34; 符合人类的阅读习惯；

* `小端`（`LittileEndian`）： byte[2] = 0X1234， 则byte[1] = 0X12, byte[0] = 0x34; 符合机器的字节流；

  可以通过以下程序来获取当前操作系统的大小端

  ```c
  /**
   * 运行结果为 1: little-endian
   * 					0: big-endian
   */
  # include <stdio.h>
  int main()
  {
      int a = 0x0102;
      char *p = (char *)&a;
      printf("byte[0]=%d, addr=%p\n", *p, p);
      printf("byte[1]=%d, addr=%p\n", *(p+1), p+1);
  }  
  ```

  

##  符号说明

`bit`：二进制的位；

`byte`：字节，一个字节为8位（bit）；

`func`：功能；

`sign`：双精度浮点数的符号位，以下简称`s`

`e_sign`：双进度浮点数的科学表示法的指数的符号位，以下简称`es`。

##  科学计数法

###   熟悉的十进制   

​	将一个数表示成 a 与 10 的 n 次幂相乘的形式（其中 1≤|a|<10，a不能为分数形式，n为整数），这种记数法叫做**`科学记数法`**。

例如 浮点数 331.46875，如果表示为十进制的科学技术法，则需要将数字的小数点向左移动，直到数字处于区间[1,10)：
$$
\begin{align}
331.46875 &= 3.3146875 \times 10^2 \\
&= 3.3146875 E 2 \\
&= [(3\times 10^0).(3 \times 10^{-1}+ 1 \times 10{-2}+ 4 \times 10^{-3}+ 6 \times 10^{-4}+ 8 \times 10^{-5}+ 7 \times 10^{-6}+ 5 \times 10^{-7})]\times10^2\\
&=(3\times 10^0 + 3 \times 10^{-1}+ 1 \times 10{-2}+ 4 \times 10^{-3}+ 6 \times 10^{-4}+ 8 \times 10^{-5}+ 7 \times 10^{-6}+ 5 \times 10^{-7}) E2\tag1
\end{align}
$$
则 3.3146875 称为底数（`mantissa`，以下简称为 `m` ）， 2称为指数（`exponent`，以下简称 `exp`）

###  扩展到二进制

​	二进制的 **`科学记数法`**， 即将一个数表示成 a 与 2 的 n 次幂相乘的形式（其中 1≤|a|<2，a不能为分数形式，n为整数）。


##  浮点数二进制表示法

   计算机存储的双精度浮点数共8个字节(`byte`)，64 位(`bit`)。64位中， 其中 1 位为符号位，11 位为指数（`exp`）位（这11位中有一位为符号位`es`），其余的52位为底数位（`m`），以`BigEndian`格式的 字节序列如表1所示，其中：

* `all_bit`: 64位的序号
* `byte` 当前属于第几个字节
* `func`：当前位的功能
* `byte_bit`：当前字节内当前位的序号
* `data`：所储存的数据

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

## 十进制科学计数法到二进制科学计数法的转换

​       考虑到不同的进制换算，为了表述清楚起见，每个数字添加头部表示以示区分。 例如以`0x`（`0X`）开头的数为16进制， 以 `0b`（`0B`）开头的数为2进制，以`0D`开头的数为10进制数。    

​       依然以2.3节的 331.46875 为例，进行说明，十进制通过科学记数法表示如公式（2）所示。
$$
\begin{align}
0D331.459 &= 3.3146875 \times 10^2 \\
&= 3.3146875 E 2\tag2
\end{align}
$$
接下来，进行十进制到二进制的转换， 首先讲十进制数据的整数部分331，换算为2进制，如公式（3）所示。
$$
\begin{align}
0D331&=256+ 64+11 =1\times16^2+4\times16^1+ 11 = 0X14B \\
&= 0B000101001010\tag3
\end{align}
$$
再将十进制数据的小数部分 0.46875 换算为 2 进制小数，如公式（4）所示。
$$
\begin{align}
0D0.459&=0D[0\times 2^{0}].[0\times 2^{-1} + 1\times2^{-2} + 1\times 2^{-3} +1\times2^{-4}+1\times2^{-8}]\\
&=0B0.01111\tag4
\end{align}
$$
将公式（3）和（4）相加，得到公式（5）：
$$
\begin{align}
0D331.459 &= 0D331 +0D0.459 \\
&= 0B000101001010 +0B0.01111\\&= 0B000101001010.01111\\&= 0B0001.0100101001111\times2^8
\\&= 0B1.0100101001111E1000\tag5
\end{align}
$$
从2.3.2节可以看出， 由于二进制的特殊性，小数点前面的数永远为1，因为能够落在区间[1, 2)的数字只有1，因此在后续表示底数的时候，直接忽略掉小数点前的数字，即公式（5）中在计算底数和指数时，计：

```sh
（1）底数 mantissa=0100101001111					# 注意，此处忽略了小数点前的1
（2）指数 exponent=1000
```

## 双精度（64bit）浮点数二进制表示法

如表1所示， 

* 0B1.0100101001111E1000 为正数，所以 `s`=0，即all_bit[0] = 0; 
* 指数 `exp`=100 为正数， `es`=0，即`all_bit`[1]=0；
*  `all_bit`[2-11]=2<sup>10</sup>- 0B0000000100。为什么这里突然冒出个2<sup>10</sup>，后面再解释。
* `all_bit`[12-63]， 共计52位，表示的数字为 底数 `m`=0100101001111	

# 代码实现

## Java

​         根据浮点数定义，自行实现的解析方法如代码段 3-1 所示。

<center>Code Snippet 3-1</center>

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

​         JDK 本身提供了native 实现的类方法，如代码段 3-2 所示。

<center>Code Snippet 3-2</center>

```java
//hex为16进制的字符数组表示形式,格式为大端
public static double parseDouble(String hex) {
    return Double.longBitsToDouble(Long.parseLong(hex, 16));
}
```

##  C

​         C语言的实现如代码段3-3所示。

<center>Code Snippet 3-3</center>

```c
#include <stdio.h>
void print_bin(char c){
	for(int i =0; i < 8; i++){
		printf("%d", ((c<<i)& 0b10000000) >> 7);
	}
}

int main()
{
	unsigned long x = 0x40E743CD1BF68000;
	double *a = (double*)&x;
	printf("%f, sizeof(double)=%lu\n", *a, sizeof(double));
	for(int i=sizeof(double)-1; i>=0;i--){
		char *p = (char*)a;
		char c = *(p+i);
    // 可以看到二进制的表示，进一步验证 IEEE754 的计算机存储方法
		print_bin(c);
    printf("\n");
    char* y = "40E743CD1BF68000";
	}
}
```

