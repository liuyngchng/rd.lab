#ifndef PEER_H
#define PEER_H
/**
 * 连接 ip:port 表示的 host,发送报文 req, 接收返回的报文保存至 resp
 **/
int writemsg(const char *ip, const int port, const char *req, char *const resp);

#endif
