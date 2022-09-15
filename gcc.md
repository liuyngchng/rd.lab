<h1>`GCC` Introduction</h1>

# 1. cmd
```
gcc main.c -o main      # comile and link source file in one step, output a executable bin file
gcc -c main.c           # produce object file from source, default file name is main.o, you can use -o get a customized name
gcc main.o              # link a object file to produce a execute bin file
```
# 2. `GCC` pipeline
gcc process step
- precompile source file -> precompiled file
- precompiled file -> assembly file
- assembly file -> object file
- link object file -> executable file

# 3. `GCC` options

GCC 选项区分大小写

| option | note |
| :-- | :-- |
| -E | 预处理指定的源文件，不进行编译,例如宏展开、文件包含、删除部分代码等 |
| -S | 编译指定的源文件，生成汇编源程序，但是不进行汇编 |
| -c |  编译、汇编指定的源(c源程序,汇编源程序)文件，生成目标文件,但是不进行链接 |
| -o [file1] [file2] | 将文件 file2(c源程序，汇编源程序,目标文件) 编译成可执行文件 file1 |
| -I directory | 指定 include 包含文件的搜索目录 |
| -g | 生成调试信息，该程序可以被调试器调试 |
| -save-temps | GCC 会正常地编译和链接，但是会把预处理器输出、汇编语言和对象文件全部存储在当前目录下 (.i 预处理文件, .s 汇编源文件,.o 目标文件)|

# 4. `GCC` 预处理选项

- `-E -C`   
  头文件较大或源文件包括了多个头文件时，预处理器输出可能会庞杂难读。使用-C选项可删除源文件和头文件中的注释   
- `-Dname[=definition]`  
在处理源文件之前，先定义宏 name。宏 name 必须是在源文件和头文件中都没有被定义过的。将该选项搭配源代码中的#ifdef name命令使用，可以实现条件式编译。如果没有指定一个替换的值，该宏被定义为值 1
- `-Uname`  
如果在命令行或 GCC 默认设置中定义过宏 name，则“取消”name 的定义。`-D`和`-U`选项会依据在命令行中出现的先后顺序进行处理  
- `-Idirectory[:directory[...]]`  
当通过 #include 命令把所需的头文件包括进源代码中时，除系统标准 include 目录之外，指定其他的目录对这些头文件进行搜索
- `-iquote directory[:directory[...]] `
这是在最近 GCC 版本中新增的选项，它为在 #include 命令中采用引号而非尖括号指定的头文件指定搜索目录  
- `-isystem directory[:directory[...]]`  
该选项在标准系统 include 目录以外为系统头文件指定搜索目录，且它指定的目录优先于标准系统 include 目录被搜索。在目录说明开头位置的等号，被视作系统根目录的占位符，可以使用`--sysroot` 或 `-isysroot` 选项来修改它
- `-isysroot directory`  
该选项指定搜索头文件时的系统根目录。例如，如果编译器通常在 /usr/include 目录及其子目录下搜索系统头文件，则该选项将引导到 directory/usr/include 及其子目录下进行搜索
- `-I-`  
在较新版本的 GCC 中，该选项被`-iquote`替代。在旧版本中，该选项用于将命令行的所有`-Idirectory`选项分割为两组。所有在`-I-`左边加上`-I`选项的目录，被视为等同于采用`-iquote`选项；这指的是，它们只对 `#include` 命令中采用引号的头文件名进行搜索。    
所有在`-I-`右边加上`-I`选项的目录，将对所有`#include`命令中的头文件名进行搜索，无论文件名是在引号还是尖括号中。  
而且，如果命令行中出现了`-I-`，那么包括源文件本身的目录不再自动作为搜索头文件的目录

# 5. Include Header  

对于include目录而言，通常的搜索顺序是：  
(1) 包含指定源文件的目录（对于在 #include 命令中以引号包括的文件名）。  
(2) 采用-iquote选项指定的目录，依照出现在命令行中的顺序进行搜索。只对 #include 命令中采用引号的头文件名进行搜索。  
(3) 采用-I选型指定的目录，依照出现在命令行中的顺序进行搜索。  
(4) 采用环境变量 CPATH 指定的目录。  
(5) 采用-isystem选项指定的目录，依照出现在命令行中的顺序进行搜索。  
(6) 采用环境变量 C_INCLUDE_PATH(gcc)\CPLUS_INCLUDE_PATH(g++) 指定的目录。  
(7) 系统默认的 include 目录  
# 6. `GCC` -S
- 编译器的核心任务是把C程序翻译成机器的汇编语言（`assembly language`）。汇编语言是人类可以阅读的编程语言，也是相当接近实际机器码的语言。由此导致每种 `CPU` 架构都有不同的汇编语言。  
- 实际上，`GCC` 是一个适合多种 `CPU` 架构的编译器，不会把C程序语句直接翻译成目标机器的汇编语言，而是在输入语言和输出汇编语言之间，利用一个中间语言，称为 `RegisterTransfer Language`（简称 `RTL`，寄存器传输语言）。借助于这个抽象层，在任何背景下，编译器可以选择最经济的方式对给定的操作编码。
- 在交互文件中针对目标机器的抽象描述，为编译器重新定向到新架构提供了一个结构化的方式。但是，从 `GCC` 用户角度来看，我们可以忽略这个中间步骤。
- 通常情况下，`GCC` 把汇编语言输出存储到临时文件中，并且在汇编器执行完后立刻删除它们。但是可以使用`-S`选项，让编译程序在生成汇编语言输出之后立刻停止。
- 如果没有指定输出文件名，那么采用`-S`选项的 `GCC` 编译过程会为每个被编译的输入文件生成以`.s`作为后缀的汇编语言文件。如下例所示：
```
$ gcc -S circle.c
```
编译器预处理 circle.c，将其翻译成汇编语言，并将结果存储在 circle.s 文件中。
如果想把 C 语言变量的名称作为汇编语言语句中的注释，可以加上`-fverbose-asm`选项：
```
$ gcc -S -fverbose-asm circle.c
```  

# 7. `GCC` -l
## 7.1 链接器  
- 链接器把多个二进制的目标文件（`object file`）链接成一个单独的可执行文件。在链接过程中，它必须把符号（变量名、函数名等一些列标识符）用对应的数据的内存地址（变量地址、函数地址等）替代，以完成程序中多个模块的外部引用。
- 链接器也必须将程序中所用到的所有`C`标准库函数加入其中。对于链接器而言，链接库不过是一个具有许多目标文件的集合，它们在一个文件中以方便处理。
- 当把程序链接到一个链接库时，只会链接程序所用到的函数的目标文件。在已编译的目标文件之外，如果创建自己的链接库，可以使用 ar 命令。
- 标准库的大部分函数通常放在文件 libc.a 中（文件名后缀.a代表“archive”），或者放在用于共享的动态链接文件 libc.so 中（文件名后缀.so代表“share object”，译为“共享对象”）。这些链接库一般位于 /lib/ 或 /usr/lib/，或者位于 GCC 默认搜索的其他目录。
- 当使用 `GCC` 编译和链接程序时，`GCC` 默认会链接 libc.a 或者 libc.so，但是对于其他的库（例如非标准库、第三方库等），就需要手动添加。

## 7.2 demo
`GCC` 的`-l`选项可以让我们手动添加链接库。下面我们编写一个数学程序 main.c，并使用到了 cos() 函数，它位于 <math.h> 头文件
```
#include <stdio.h>      /* printf */
#include <math.h>       /* cos */
#define PI 3.14159265
int main ()
{
    double param, result;
    param = 60.0;
    result = cos ( param * PI / 180.0 );
    printf ("The cosine of %f degrees is %f.\n", param, result );
    return 0;
}
```
为了编译这个 main.c，必须使用-l选项，以链接数学库：
```
$ gcc main.c -o main.out -lm
```
数学库的文件名是 `libm.a`。前缀`lib`和后缀`.a`是标准的，`m`是基本名称，`GCC` 会在`-l`选项后紧跟着的基本名称的基础上自动添加这些前缀、后缀，本例中，基本名称为 `m`。
```
在支持动态链接的系统上，GCC 自动使用在 Darwin 上的共享链接库 libm.so 或 libm.dylib。
```
## 7.3 自定义的链接库  
`GCC` 会自动在标准库目录中搜索文件，例如 `/usr/lib`，如果想链接其他目录中的库，就得特别指明。有三种方式可以链接在 GCC 搜索路径以外的链接库  
- 把链接库作为一般的目标文件  
GCC 指定该链接库的完整路径与文件名。
例如，如果链接库名为 libm.a，并且位于 /usr/lib 目录，那么下面的命令会让 GCC 编译 main.c，然后将 libm.a 链接到 main.o
```
$gcc main.c -o main.out /usr/lib/libm.a
```
- 使用`-L`选项
为 `GCC` 增加另一个搜索链接库的目录
```
$ gcc main.c -o main.out -L/usr/lib -lm
```
可以使用多个`-L`选项，或者在一个`-L`选项内使用冒号分割的路径列表
- `LIBRARYPATH`  
把包括所需链接库的目录加到环境变量 `LIBRARY_PATH` 中

生成自定义的archieve文件
```
gcc -c a.c
gcc -c b.c
gcc -c c.c
ar -fr abc.a a.o b.o c.o

```
# 8. Shared Object File  
## 8.1 Output `.so` File
Linux 下动态链接库（`shared object file`，共享对象文件）的文件后缀为`.so`，  
它是一种特殊的目标文件（`object file`），可以在程序运行时被加载（链接）进来。  
使用动态链接库的优点是：程序的可执行文件更小，便于程序的模块化以及更新，  
同时，有效内存的使用效率更高  
创建一个动态链接库，可以使用 `GCC` 的`-shared`选项。  
输入文件可以是源文件、汇编文件或者目标文件  
还得结合`-fPIC`选项。`-fPIC` 选项作用于编译阶段，告诉编译器产生与位置无关代码（`Position-Independent Code`）；  
这样一来，产生的代码中就没有绝对地址了，全部使用相对地址，所以代码可以被加载器加载到内存的任意位置，都可以正确的执行。  
这正是共享库所要求的，共享库被加载时，在内存的位置不是固定的。  
从源文件生成动态链接库
```
$gcc -fPIC -shared func.c -o libfunc.so
```
从目标文件生成动态链接库
```
gcc -fPIC -c func.c -o func.o
gcc -shared func.o -o libfunc.so
```
`-fPIC` 选项作用于编译阶段，在生成目标文件时就得使用该选项，以生成位置无关的代码  

## 8.2 Compile with `.so` file  

如果希望将一个动态链接库链接到可执行文件，那么需要在命令行中列出动态链接库的名称，具体方式和普通的源文件、目标文件一样  

```
gcc main.c libfunc.so -o app.out
```
还可以这样加载  
```
gcc main.c -L. -lfunc -o app.out
```

还可以在编译时链接库环境变量 `LIBRARY_PATH` 中添加so文件的路径，然后执行

```
gcc main.c -lfunc -o app.out
```

编译器会自动在 `LIBRARY_PATH`中查找动态链接文件libfunc.so  

## 8.3 Run with `.so` file  

编译完之后，必须要确保程序在运行时可以找到这个动态链接库，可以采用以下几种方法中的一种   
* 你可以将链接库放到标准目录下，例如 /usr/lib   
* 为运行时设置一个合适的环境变量，例如 LD_LIBRARY_PATH。  
```
export LD_LIBRARY_PATH='xxxx'
```
* 不同系统，具有不同的加载链接库的方法, ubuntu 和 centos下，  
  执行 `sudo vim /etc/ld.so.conf`,  或者  
  ```
  cd /etc/ld.so.conf.d
  sudo vim customized.conf
  ```
  将.so 文件的路经添加进去,然后执行 `sudo /sbin/ldconfig` 使系统配置生效  
 ldconfig creates the necessary links and cache to the most recent   
shared libraries found in the directories specified on the command   
line, in the file /etc/ld.so.conf, and in the trusted directories   
(/lib and /usr/lib).  
* 设置 LD_LIBRARY_PATH
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/
```  
## 8.4 check shared objects (shared libraries) dependency

```
ldd execuable file
```

# 9. cross-compilation(交叉编译)
交叉编译，相对于原生编译(native compilation)来说，是指在某个主机平台上（比如x86上）用交叉编译器编译出可在其他平台上（比如ARM上）运行的代码的过程。
To cross-compile is to build on one platform a binary that will run on another platform. When speaking of cross-compilation, it is important to distinguish between the build platform on which the compilation is performed, and the host platform on which the resulting executable is expected to run. The following configure options are used to specify each of them
```
--build=build
The system on which the package is built.

--host=host
The system where built programs and libraries will run.
```
## 9.1 命名规则
交叉编译工具链的命名规则为：arch [-vendor] [-os] [-(gnu)eabi]
```
arch - 体系架构，如ARM，MIPS
verdor - 工具链提供商
os - 目标操作系统
eabi - 嵌入式应用二进制接口
```
根据对操作系统的支持与否，ARM GCC可分为支持和不支持操作系统，如
```
arm-none-eabi：这个是没有操作系统的，自然不可能支持那些跟操作系统关系密切的函数，比如fork(2)。他使用的是newlib这个专用于嵌
       入式系统的C库。
arm-none-linux-eabi：用于Linux的，使用Glibc
```
## 9.2 命名实例
```
arm-none-eabi-gcc
```
（ARM architecture，no vendor，not target an operating system，complies with the ARM EABI）  
用于编译 ARM 架构的裸机系统（包括 ARM Linux 的 boot、kernel，不适用编译 Linux 应用 Application），  
一般适合 ARM7、Cortex-M 和 Cortex-R 内核的芯片使用，所以不支持那些跟操作系统关系密切的函数，  
比如fork(2)，他使用的是 newlib 这个专用于嵌入式系统的C库。  
```
arm-none-linux-gnueabi-gcc
```
(ARM architecture, no vendor, creates binaries that run on the Linux operating system, and uses the GNU EABI)  
主要用于基于ARM架构的Linux系统，可用于编译 ARM 架构的 u-boot、Linux内核、linux应用等。  
arm-none-linux-gnueabi基于GCC，使用Glibc库，经过 Codesourcery 公司优化过推出的编译器。  
arm-none-linux-gnueabi-xxx 交叉编译工具的浮点运算非常优秀。一般ARM9、ARM11、Cortex-A 内  
核，带有 Linux 操作系统的会用到  
```
arm-eabi-gcc
```
Android ARM 编译器。
```
armcc
```
ARM 公司推出的编译工具，功能和 arm-none-eabi 类似，可以编译裸机程序（u-boot、kernel），  
但是不能编译 Linux 应用程序。  
armcc一般和ARM开发工具一起，Keil MDK、ADS、RVDS和DS-5中  
的编译器都是armcc，所以 armcc 编译器都是收费的。
```
arm-none-uclinuxeabi-gcc
```
```
 arm-none-symbianelf-gcc
```
arm-none-uclinuxeabi 用于uCLinux，使用Glibc。  
arm-none-symbianelf 用于symbian，没用过，不知道C库是什么   
## 9.3 缩写说明
```
ABI
```
二进制应用程序接口(Application Binary Interface (ABI) for the ARM Architecture)。  
在计算机中，应用二进制接口描述了应用程序（或者其他类型）和操作系统之间或其他应用程序的低级接口。  
```
EABI
```
嵌入式ABI。嵌入式应用二进制接口指定了文件格式、数据类型、寄存器使用、堆积组织优化和在一个嵌入式软件中的参数的标准约定。  
开发者使用自己的汇编语言也可以使用 EABI 作为与兼容的编译器生成的汇编语言的接口。  
两者主要区别是，ABI是计算机上的，EABI是嵌入式平台上（如ARM，MIPS等）。  
## 9.4 其他
`arm-linux-gnueabi-gcc` 和 `arm-linux-gnueabihf-gcc`  
两个交叉编译器分别适用于`armel`和 `armhf` 两个不同的架构，`armel` 和 `armhf` 这两种架构在对待浮点运算采取了不同的策略  
（有 fpu 的 arm 才能支持这两种浮点运算策略）。  
其实这两个交叉编译器只不过是 `gcc` 的选项 `-mfloat-abi` 的默认值不同。  
`gcc` 的选项 `-mfloat-abi` 有三种值 `soft`、`softfp`、`hard`
（其中后两者都要求`arm` 里有 `fpu` 浮点运算单元，`soft` 与后两者是兼容的，但 `softfp` 和 `hard` 两种模式互不兼容）：  
`soft`： 不用fpu进行浮点计算，即使有fpu浮点运算单元也不用，而是使用软件模式。  
`softfp`： `armel`架构（对应的编译器为 `arm-linux-gnueabi-gcc` ）采用的默认值，用`fpu`计算，  
但是传参数用普通寄存器传，这样中断的时候，只需要保存普通寄存器，中断负荷小，但是参数需要转换成浮点的再计算。  
`hard`： `armhf`架构（对应的编译器 `arm-linux-gnueabihf-gcc` ）采用的默认值，  
用`fpu`计算，传参数也用`fpu`中的浮点寄存器传，省去了转换，性能最好，但是中断负荷高。

#  10. 创建静态库 (静态编译）  

使用ar命令（archive）可以很容易地创建属于自己的静态库。
ar命令一般对.o的目标文件进行操作，目标文件可以由gcc -c命令得到。  
下面就以一个具体的例子来说明一下。

## 10.1 源程序文件  

首先，我们有如下两个源程序文件：  

```
ls
main.c  print.c test.h
pg print.c

#include <stdio.h>

int print()
{
    printf("Hello world\n");
    return 0;
}

pg main.c                                                        
#include "test.h"

int main()
{
    print();
    return 0;
}

pg test.h
int print();
```
## 10.2 生成目标文件    

先通过gcc -c命令将其编译成.o文件:

```
gcc -c *.c                                                        
ls
main.c  main.o  print.c  print.o  test.h
```

我们可以看到两个.o的目标文件已经成功生成。  
这时候，如果我们使用以下命令，是可以直接编译成功的：  

```
gcc -o test *.o
ls
main.c  main.o  print.c  print.o  test.h test
./test
Hello world
```  
## 10.3 创建归档文件  

但是这里由于我们是要创建静态库，所以可以使用ar命令来创建一个归档文件:

```
ar crv libtest.a print.o
a - test2.o
ls
libtest.a  main.c  main.o  print.c  print.o  test  test.h
```
可以看到，静态库libtest.a已经成功创建，  
这时，还需要使用ranlib命令来生成一个内容表，这一步在Unix系统中必不可少，  
但在Linux中，当使用的是GUN开发工具时，这一步可以省略。  
以上步骤完成后，即可以使用下面的命令来编译程序：  

```
ranlib libtest.a
```
## 10.4 链接静态库文件  
```
gcc -o testa main.o -L./ -ltest
ls
libtest.a  main.c  main.o  print.c  print.o  test  testa  test.h
./testa
Hello world
```

通过以上案例，可以发现得到的效果其实是一样的。  
当然，也可以使用以下命令，得到相同的效果（这里因为没有链接头文件，会报一个错，但是结果没有影响）：  

```
gcc -o testb main.c -L./ -ltest  
ls
libtest.a  main.c  main.o  print.c  print.o  test  testa  testb  test.h
 ./testb
Hello world
```

