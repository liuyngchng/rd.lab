#ifndef PTTN_H
#define PTTN_H
/**
 * 根据url,返回不同的 HTTP 报文,与具体的需求有关，非通用类;
 * 这里提供的是一个demo,实际使用时可按照需求进行自定义
 * req, 请求报文;
 * resp, 响应报文
 **/
int dispatch(char *req, char *resp);
#endif
