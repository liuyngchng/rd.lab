/**
 * a HTTP client
 * @author whoami@hotmail.com
 * @since 2022-06-01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"
#include "const.h"
#include "cfg.h"
#include "utl.h"

extern char _SRV_IP_[_CFG_LEN_];
extern int _SRV_PORT_;

int clreq() {
    char resp[8096] = {0};
    char *path="/";
    char *bd = "123";
    req(_SRV_IP_, _SRV_PORT_, _POST_, path, bd, resp, sizeof(resp), 0);
    printf("[%s][%s-%d]resp body\n%s\n", gettime(),
    	filename(__FILE__), __LINE__, resp);
    return 0;
}

int main(int argc, char* argv[]) {
	initcfg();
	clreq();
}
