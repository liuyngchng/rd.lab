# help

```sh
redis-cli -h
```



# cmd

```sh
redis-cli  -h 11.10.36.4 -p 6379 -a '12345$lGifRT'  keys "*"
```

# hiredis

获取源代码并编译

```sh
git clone git@github.com:redis/hiredis.git
cd hiredis
make
```

使用`hiredis` 的库 test.c

```c
#include "hiredis.h"
int main() {
	char *ip = "127.0.0.1";
    int port = 6379;
    redisContext *c = redisConnect(ip, port);
    if (c == NULL || c->err) {
        if (c) {
            printf("err: %s\n", c->errstr);
            return -1;
        } else {
            printf("can't allocate redis context\n");
        }
    }
    const char *psword = "your_psword";
    redisReply *reply;
    // auth psword
	reply = redisCommand(c, "AUTH %s", psword);
    if (reply->type == REDIS_REPLY_ERROR) {
        printf("auth_failed, cause: %s\n", reply->str);
        freeReplyObject(reply);
        redisFree(c);
        return -1;
    }
    freeReplyObject(reply);
    printf("connected to %s:%d, auth %s\n", ip, port, psword);
	// exec cmd
    char *cmd = "keys *";
    reply = (redisReply *)redisCommand(c, cmd);
    printf("cmd result %s\n", reply->str);
    freeReplyObject(reply);

    redisFree(c);
    return 0;
}
```

编译

```sh
gcc test.c -lhiredis -L ./hiredisdir -I ./hiredisdir
```

运行

```sh
# 添加动态库加载路径
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${lib_hiredis_so_dir}
# 使配置更改生效
sudo ldconfig
# 进入ELF 可执行文件的目录
cd ${elf_dir}
# 构建软链
ln -s libhiredis.so libhiredis.so.1.2.1-dev
# 查看是否缺少其他动态库
ldd ./a.out
# 执行可执行文件
./a.out
```







