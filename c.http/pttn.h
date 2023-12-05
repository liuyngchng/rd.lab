#ifndef PTTN_H
#define PTTN_H
/**
 * 根据url,返回不同的 HTTP 报文;
 * req, 请求报文;
 * resp, 响应报文
 **/
int dispatch(char *req, char *resp);
#endif
