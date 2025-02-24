# 1. gcc -g
```
gcc -g main.c -o main
```
-g　选项生成 gdb　需要的一些信息

# 2. start debug
```
gdb main        # start debug executable main
break function  # set break point for a function
run main        # start run executable main
run args        # debug运行时提供控制台参数
info locals     # view local variables
p variable_name # print a variable value
set args        # input console arguments for main(int argc ,char *[]args)
```
control
```
c    # run to next break point
next        # step over
step        # step into
```
# 3. doc
启动GDB后，首先就是要设置断点，程序中断后才能调试。在gdb中，断点通常有三种形式：

## 3.1 断点（BreakPoint）

在代码的指定位置中断.设置断点的命令是break，它通常有如下方式：

```
break <function>          # 在进入指定函数时停住
break <linenum>           # 在指定行号停住。
break +/-offset           # 在当前行号的前面或后面的offset行
break filename:linenum    # 在源文件filename的linenum行处
break ... if <condition>  # ...可以是上述的参数，condition表示条件，在条件成立时停住。比如在循环境体中，可以设置break if i=100，表示当i为100时停住
```

查看信息

```
info breakpoints n        # 命令查看当前断点信息,
delete                    # 删除所有断点
delete breakpoint n       # 删除某个断点
disable breakpoint n      # 禁用某个断点
enable breakpoint n       # 使能某个断点
save breakpoints bp       # 保存断点信息至文件
gdb     xxxx -x  bp       # 调试时，加载文件中的断点信息
```

## 3.2 观察点（WatchPoint）

在变量读、写或变化时中断，这类方式常用来定位 bug  

```
watch <expr>    变量发生变化时中断
rwatch <expr>    变量被读时中断
awatch <expr>     变量值被读或被写时中断
```

可以通过info watchpoints [n]命令查看当前观察点信息

## 3.3 多线程

```
info thread     # 查看线程信息
thread n        # 进入线程编号为n的线程，进行debug
```

## 3.4 捕捉点（CatchPoint）

捕捉点用来补捉程序运行时的一些事件。如：载入共享库（动态链接库）、C++的异常等。通常也是用来定位bug。

捕捉点的命令格式是：catch <event>，event可以是下面的内容

  throw     C++抛出的异常时中断
  catch     C++捕捉到的异常时中断
  exec    调用系统调用exec时（只在某些操作系统下有用）
  fork    调用系统调用fork时（只在某些操作系统下有用）
  vfork    调用系统调用vfork时（只在某些操作系统下有用）
  load 或 load <libname>     载入共享库时（只在某些操作系统下有用）
  unload 或 unload <libname>    卸载共享库时（只在某些操作系统下有用）

另外，还有一个tcatch <event>，功能类似，不过他只设置一次捕捉点，当程序停住以后，应点被自动删除。

捕捉点信息的查看方式和代码断点的命令是一样的，这里就不多介绍了。

在特定线程中中断

你可以定义你的断点是否在所有的线程上，或是在某个特定的线程。GDB很容易帮你完成这一工作。

    break <linespec> thread <threadno>
    break <linespec> thread <threadno> if ...

linespec指定了断点设置在的源程序的行号。threadno指定了线程的ID，注意，这个ID是GDB分配的，你可以通过"info threads"命令来查看正在运行程序中的线程信息。如果你不指定thread <threadno>则表示你的断点设在所有线程上面。你还可以为某线程指定断点条件。如：

     (gdb) break frik.c:13 thread 28 if bartab > lim

当你的程序被GDB停住时，所有的运行线程都会被停住。这方便你你查看运行程序的总体情况。而在你恢复程序运行时，所有的线程也会被恢复运行。那怕是主进程在被单步调试时。

恢复程序运行和单步调试

在gdb中，和调试步进相关的命令主要有如下几条：

    continue    继续运行程序直到下一个断点（类似于VS里的F5）
    next        逐过程步进，不会进入子函数（类似VS里的F10）
    setp        逐语句步进，会进入子函数（类似VS里的F11）
    until        运行至当前语句块结束
    finish    运行至函数结束并跳出，并打印函数的返回值（类似VS的Shift+F11）

PS：这些命令大部分可以简写为第一个字母，在日常使用过程中，往往只会输入第一个字符即可执行该命令，我标红的即是通常的使用方式。这几条命令使用非常频繁，并且可以带一些附加参数以实现高级功能，需要熟练掌握。
