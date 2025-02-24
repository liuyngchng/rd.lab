#ifndef PEER_H
#define PEER_H
/**
 * (1)连接 ip:port 表示的 socket
 * (2)发送报文 req;
 * (3)接收返回的报文保存至 resp
 * (4)关闭连接
 **/
int writemsg(const char *ip, const int port, const char *req, char *const resp);

#endif
