# Unit one

##  配置开发环境 （config dev env）

下载JDK （Java Development Kit，即 Java 开发组件）， 配置环境变量（PATH 中添加 jdk_1.8/bin 目录，为了让控制台知道上哪儿去寻找命令 java 的可执行文件），在控制台（Windows cmd， Linux terminal ）运行

```powershell
java -version
openjdk version "17.0.3" 2022-04-19
```

看到自己安装的 java 的版本，至此，基本的开发环境已经OK。

##   运行 hello world （run hello world demo）

* **创建 java 源代码文件**

以下脚本默认运行在 `linux terminal `环境下， `windows `雷同。

```sh
mkdir workspace				# 创建自己的 java 工程目录
pwd							# 查看当前目录
touch hello.java			# 创建一个文件，文件名成为 hello.java
vi hello.java				# 打开 hello.java 文件
```

hello.java 内容如下:

```java
class hello {
    public static void main(String[] args) {
        System.out.println("hello world");
    }
}
```

注意：如果在`windwos` 下编辑文件， 则最好将 `hell.java` 保存为` UTF-8` 文件（`windows` 默认为 `ASCII` 编码格式），保持良好的编程习惯。

java 源代码文件的文件名（hello.java）称跟其内容中的 class 名称（hello）保持一致，这是默认规则。当然不一样可以吗？也可以，但初学者暂时按这个规则来，后续再讲不一样的处理方法。

* **编译为java 字节码文件**

运行

```sh
javac hello.java				# 编译源代码
ls								# 查看当前目录下的文件
hello.java hello.class			# 可以看到编译生成的字节码文件 hello.class
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

# 数据结构（data structure）

