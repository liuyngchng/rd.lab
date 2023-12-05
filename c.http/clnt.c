#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"
#include "util.h"


int clientreq() {
    char resp[8096] = {0};
    // myhttpreq(resp);
    char *ip = "127.0.0.1";
    int port = 8083;
    char *method="POST";
    char *path="/";
    char *bd = "123";
    // char reqeust[1024]={0};
    // req(resp);
    req(ip, port, method, path, bd, resp, sizeof(resp), 0);
    printf("[%s][%s-%d]resp body\n%s\n",gettime(), filename(__FILE__), __LINE__, resp);
    return 0;
}

int main(int argc, char* argv[]) {
    clientreq();
}