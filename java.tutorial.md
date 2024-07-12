# Unit 1 环境准备

##  配置开发环境 

### 安装 JDK

**（1）Windows 系统**

下载JDK （Java Development Kit，即 Java 开发组件）， 地址详见甲骨文(Oracle)官网 https://www.oracle.com/java/technologies/downloads/， 选择对应的操作系统（Windows、MacOS、Linux）和硬件架构（x86, aarch）的安装包。解压后安装。

**（2）Ubuntu 系统**

以ubuntu 22.04 LTS 为例，可直接在控制台终端(CTRL+ALT+T)中运行  

```sh
sudo apt-get install openjdk-17-jdk
```

输入管理员密码，即可。

###  配置环境变量

**（1）Windows 系统**

1）点击 “我的电脑”->"属性"->高级系统设置->环境变量->系统变量->新建

JAVA_HOME=java安装路径

2）在“我的电脑”->"属性"->高级系统设置->环境变量->系统变量-> 选择PATH ->编辑 ， 在PATH环境变量中添加一个value, %JAVA_HOME%\bin

3）验证。按下  Windows键+R，输入cmd，确定，输入 `java -version`，验证环境，若输出如下内容，则表示安装成功。

```powershell
java version 22****
Java (TM) SE Runtime Environment
```

**（2）Ubuntu 系统**

通过 sudo命令安装的JDK,无需添加环境变量。在控制台（CTRL+ALT+T）运行 `java -version`。若输出如下内容，则表示安装成功。

```sh
openjdk version "17.0.10" 2024-01-16
OpenJDK Runtime Environment (build 17.0.10+7-Ubuntu-122.04.1)
OpenJDK 64-Bit Server VM (build 17.0.10+7-Ubuntu-122.04.1, mixed mode, sharing)
```

至此，java运行时已安装完毕。

### 扩展阅读

为什么要安装JDK，不安装可以进行Java开发吗？   java属于高级语言，需要编译后生成一种叫`字节码（Byte-code）`的中间二进制流，通过`Java 虚拟机（JVM, java vritual machine）`的解释后，才能成为机器可是别的机器指令。JDK提供以下功能。

（1）源代码编译（%JAVA_HOME/bin/javac）。将java源代码编译为java 字节码，注意是字节码，一种字节流，这种字节流不是机器指令，机器无法识别。

（2）java程序运行时（%JAVA_HOME/bin/java）。提供java虚拟机，即可以运行java字节码的环境，通过java虚拟机的翻译，将java字节码翻译为机器硬件可是别的机器指令。

（3）函数库。提供常用的一些算法、数据结构、工具类的函数包，方便开发人员开箱即用，无需自己写源代码。

综合以上分析， （1）和（2）是必须的， （3）是可选的（如果开发人员想自己实现的花），可见JDK是必须安装的，否则无法进行java相关的的开发工作。

（4）字节码。字节码是一种跟硬件无关的二进制字节流，可以被java 虚拟机识别并转换为对应的机器可执行的机器指令。之所以这么做，是为了减少对不同硬件体系结构及其对应的硬件指令集的依赖。同一套源代码，生成同一套字节码，由`JVM`去适配不同的硬件指令集。

## 安装集成开发环境

### GUI IDE 安装

安装集成开发环境（IDE， Integrated Development Environment ），能够为开发人员提供源代码输入，代码补全，代码编译，运行时调试等便利的功能。本节以开源的Eclipse 为例， 下载地址  https://www.eclipse.org/downloads/packages/。

**（1）Windows**

下载 `Windows x86-64` 版本的 `Eclipse IDE for Enterprise Java and Web Developers`， https://www.eclipse.org/downloads/download.php?file=/technology/epp/downloads/release/2024-06/R/eclipse-jee-2024-06-R-win32-x86_64.zip。

下载完成后，解压缩，双击解压后文件夹中的 `eclipse.exe` 即可直接使用。

**（2）Ubuntu**

在 Ubuntu系统中，打开(CTRL+ALT+T)终端(Terminal)执行以下命令

```sh
sudo apt-get install eclipse
```

安装成功后, 在终端中执行

```sh
eclipse &
```

 即可打开图形化用户界面（GUI， Graphical User Interface）。

###  扩展阅读

是否可以不安装IDE,直接通过文本文档进行开发工作？ 事实上，如果只是进行一些小型的demo实验，或者复杂度较低的java 程序编写，可以不使用 IDE，直接打开一个 文本文档编辑器，写好源代码后，直接另存为 test.java（注意：文件编码格式需使用UTF-8）即可， 然后通过`java`相关的命令工具进行编译，运行。

但在实际的工程实践中，一般来说，都得使用一款自己熟悉的 IDE, IDE提供以下功能。

（1）源代码着色。通过源代码着色，提高代码的可读性。

（2）工程管理。对工程中所编写的源代码依赖的其他工具包进行自动管理，无需手动处理。

（3）源代码debug GUI。能够提供源代码 debug 的GUI，更加方便地进行代码的分析和问题查找。

目前市面上的IDE 有 Eclipse, Netbeans, Intelli IDEA，其中 Eclipse和netbeans 均为免费产品， Intelli IDEA 为收费产品。

（4）代码打包及版本管理。一般IDE 可以通过插件等其他形式提供代码打包及版本管理的图形化界面。

# Unit 2 Hello world

## 使用 IDE 创建工程并执行代码

以 eclipse-jee-2024-06-R-linux-gtk-x86_64 的 eclipse为例进行说明。打开Eclipse, 选择 `文件(File)`-> `新建(New) `->`工程(Project ...)` ->` Java` -> `Java Project`， 点击 `下一步(Next)`, 输入工程名称“helloworld”， 点击`下一步（Next）`，点击`完成(Finish)`， 如果弹出“`是否打开 java 视图(Open java perspective)`”，点击是即可。

双击IDE左侧区域 “`package Explorer`” 中的 “helloworld”， 出现2个下拉菜单： JRE System Library 和 src， 其中的 src就是源代码文件所在的目录。点击 src，右键点击新建(New) -> 包(package) ， 名称就是用默认的helloworld， 点击完成。此时在src下出现一个类似于小魔方的图标(田字形)，后面紧跟着helloworld,这就是刚创建的package。在田字形和helloworld上点击右键，点击新建(New)->类(class), 输入名称Hello,注意首字母大写，这是Java 类名称的规范写法，点击确认。此时出现了一个 Hello.java的打开文件，其中内容如下所示

```java
package helloworld;

public class Helllo {

}
```

在其中编写源代码， 如下所示。

```java
package helloworld;

public class Helllo {
	public static void main(String[] args) {
        System.out.println("hello world, i am here");
    }
}
```

点击菜单栏上的 运行(Run) -运行(Run CTRL+F11)，此时，会在IDE的最下方的 Console tab栏中，出现一行 “hello world, i am here”

##   通过命令行执行代码

* **创建 java 源代码文件**

以下脚本默认运行在 `linux terminal `环境下， `windows `类同。

```sh
mkdir workspace			# 创建自己的 java 工程目录， mkdir为linux命令， make a directory，创建文件夹
pwd						# 查看当前目录, pwd 为linux 命令， print working directory, 打印当前的工作目录
touch hello.java		# 创建一个文件，文件名成为 hello.java, touch 为linux命令， 若文件不存在则创建，若文件存在则更新访问时间
vi hello.java			# 打开 hello.java 文件， vi 为linux命令，类似于windows 下的文本编辑器，打开一个文件
```

Hello.java 内容如下所示。vi打开文件后，按键盘键“i”， 进入输入模式，输入一下内容

```java
class Hello {
    public static void main(String[] args) {
        System.out.println("hello world");
    }
}
```

注意：如果在`windows` 下编辑文件， 则最好将 `hell.java` 保存为` UTF-8` 文件（`windows` 默认为 `ASCII` 编码格式），保持良好的编程习惯。linux默认的文件编码格式为UTF-8，不存在这个问题。另外需要说明的是，linux下严格区分文件名称大小写，而windows中则不区分文件名称的大小写。

java 源代码文件的文件名（Hello.java）称跟其内容中的 class 名称（Hello）保持一致，这是默认规则。当然不一样可以吗？也可以，但初学者暂时按这个规则来，后续再讲不一样的处理方法。

* **编译为java 字节码文件**

运行

```sh
javac Hello.java						# 编译源代码
ls										# 查看当前目录下的文件
Hello.java Hello.class					# 可以看到编译生成的字节码文件 hello.class
```

`javac` 为 java 的编译器，作用是将 java源代码文件 hello.java 编译为 字节码文件(java规范定义的一种可以在任意平台上被 java 运行时翻译为机器二进制可执行文件的文件格式) `hello.class ` 

* **运行字节码文件**

  执行

```shell
java hello					# 运行字节码文件
hello world					# 可以看到控制台的输出
```

java 命令在 执行后形成的进程在Java 中叫做 Java 虚拟机（Java Virtual Machine，简称 JVM）。之所以叫虚拟机， 是因为 Java跨平台，Java 规范定义了一套运行字节码文件的规则，类似于操作系统执行可执行文件，所以叫做java 虚拟机。

注意，此处命令为`java`，后面的参数为 hello，即 hello.class的前缀， JVM 默认会找到当前目录下的 hello.class 文件，读取，加载至内存，然后解析，找到其中的入口 （main函数），然后执行。

# 几个基础概念

## 源文件

java 中的源文件是以.java为后缀的编码为UTF-8的纯文本文档。源文件的名称一般以大写字母开头，例如Animal.java, Cat.java, Car.java。

## 类 class

Java 中的 类 （class）， 表示对物理世界中的一个名词（例如，MyCat）、动词（例如，DrinkWater）。类在源文件中体现为以class开头，包含在{}中的部分源代码。通过java编译后，会生成一个与源文件同名但以.class为后缀名的类文件，例如 Animal.class, Cat.class, Car.class。`.class`文件一般称之为类文件，是一个类的字节码文件。

## 包 package

package 在linux、windows中体现为一堆嵌套的文件夹， 在 Java IDE 中以“.”连接的一串字符串，例如 com.alibaba.taobao.ui.user.login。那么从包名可以看出来这个是alibaba的淘宝这个应用的UI（用户界面）中的user（用户）的登录相关的而java类。

package 在 Java 中用于将某个功能模块涉及到的多个类文件放在一起，以方便管理。如果把一个源代码项目比喻成是一个衣柜，那么每个package可以理解为不同的衣橱，有的专门放外套，有的专门放鞋子，有的专门放裤子。

package的概念在很多编程语言中（C、python）都有这个概念，主要为了方便源代码的管理，不然好几百个源代码文件放在一个文件夹下，放眼望去，太多了，无论是管理还是查找都不方便。
