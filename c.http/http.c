#include <stdio.h>
#include <string.h>
#include "peer.h"
#include "util.h"

// HTTP/1.1 200 
// Set-Cookie: JSESSIONID=FCC697ED16FE18D3FA21F951BF39E0AE; Path=/industry; HttpOnly
// Content-Type: application/json;charset=UTF-8
// Transfer-Encoding: chunked
// Date: Thu, 23 Nov 2023 06:00:57 GMT

// 206
// {"code":200,"message":"test"}




/* 
GET / HTTP/1.1
Host: localhost:8083
User-Agent: curl/7.68.0
Accept: text/html
Content-Type:application/json; 
*/


/* 
POST /v2/api/?login HTTP/1.1
Accept:text/html
Accept-Encoding: gzip, deflate, br
Host: passport.baidu.com

username=admin&password=admin 
*/
char *const req(const char *ip, const int port, const char *method,
	const char *path, const char *body,
	char *const resp, const int n, const int bodyline) {
    char req[1024] = {0};
    char header[512]={0};
    char debug[1024]={0};
    sprintf(debug, 
        "curl -X%s --noproxy '*' 'http://%s:%d%s' -H 'Content-Type: application/json;charset=UTF-8' -d'%s'",
        method, ip, port, path, body    
    );
    sprintf(header, 
        "%s %s HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Content-Type: application/json\r\n",
        method, path, ip, port
    );
    if (strncmp(method,"GET", 3) ==0){
        sprintf(req, 
            "%s"
            "Content-Length:0\r\n\r\n",
            header
        );
    } else {
        sprintf(req, 
            "%s"
            "Content-Length:%ld\r\n\r\n"
            "%s",
            header, strlen(body), body
        );
    }
    printf("[%s][%s-%d]%s\n", gettime(),filename(__FILE__), __LINE__, debug);
    char raw_resp[4096] = {0};
    writemsg(ip, port, req, raw_resp);
    // printf("[%s][%s-%d]raw_resp\n%s\n", gettime(),filename(__FILE__), __LINE__, raw_resp);
    char raw_body[4096] = {0};
    getbody(raw_resp, raw_body, sizeof(raw_body));
    // printf("[%s][%s-%d]raw_body\n%s\n", gettime(),filename(__FILE__), __LINE__, raw_body);
    getln(raw_body, resp, n, bodyline);
    // printf("[%s][%s-%d]%d,%d,resp\n%s\n", gettime(),filename(__FILE__), __LINE__, n, bodyline, resp);
    return resp;
}
