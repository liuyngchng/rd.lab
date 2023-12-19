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
int main() {
	char *ip = "127.0.0.1";
    int port = 6379;
    redisContext *c = redisConnect(ip, port);
    if (c == NULL || c->err) {
        if (c) {
            printf("Error: %s\n", c->errstr);
            return;
        } else {
            printf("Can't allocate redis context\n");
        }
    }
    printf("connected to %s:%d\n", ip, port);
    redisReply *reply;

    cmd = "get mykey";
    reply = (redisReply *)redisCommand(c, cmd);
    printf("cmd result %s\n", reply->str);
    freeReplyObject(reply);

    redisFree(c);
}
```

编译

```sh
gcc test.c -lhiredis -L ./hiredisdir -I ./hiredisdir
```







