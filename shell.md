# tcpdump

```sh
sudo tcpdump -A -i any  host localhost and  port 9200
```

# echo

```sh
echo -e "\033[41;30m红底黑字\033[0m"
echo -e "\033[30m 黑色字 \033[0m"
echo -e "\033[31m 红色字 \033[0m"
echo -e "\033[32m 绿色字 \033[0m"
echo -e "\033[33m 黄色字 \033[0m"
echo -e "\033[46;30m 天蓝底黑字 \033[0m"
echo -e "\033[4;31m 下划线红字 \033[0m"
echo -e "\033[5;34m 红字在闪烁 \033[0m"
#需要注意的是闪烁文本在任何一个终端模拟器上都不显示，只在控制台
```

# 在shell中潜入可执行文件

可执行文件的源代码 foo.c 内容如下所示

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	const char* envtagUSER="USER";
	printf("Your Username: %s. This is detected by the program %s./n", getenv(envtagUSER), argv[0]);
	return 0;
}
```

编译 

```sh
gcc -Os foo.c -o foo
```

编写脚本文件 dofoo.sh

```sh
#!/bin/sh

skip=11
prog="/tmp/$0.exe"
tail -n +$skip "$0" > $prog
chmod u+x $prog
$prog
rm $prog
exit
#-----------------------------------------------------------
#      Start of executable file
#-----------------------------------------------------------;
```

这代码将脚本dofoo.sh 中11行以后的东西导入到了/tmp/dofoo.exe这个文件中，添加x权限，执行，然后删除该文件

现在把foo追加到dofoo.sh的末尾：

```sh
cat foo >> dofoo.sh    # 注意，追加，不是覆盖
```
