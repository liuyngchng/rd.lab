#ifndef TLSPR_H
#define TLSPR_H
/**
 * work as a TLS request client.
 * ip, connect ip;
 * port, connect port;
 * msg, send msg(payload);
 * resp, response data
 * n, sizeof resp
 */
char *const tlsreq(const char *ip, const int port, const char *msg, char *const resp, int n);

#endif
