/**
 * constant used in project
 * @author whoami@hotmail.com
 * @since 2022-06-01
 */
#ifndef CONST_H
#define CONST_H

#define _BACKLOG_       10
#define _CFG_LEN_		256
#define _POST_			"POST"
#define _CMN_RESP_FMT_	"{\"uri\":\"%s\",\"timestamp\":\"%s\",\"status\":200}"
#define _CRUL_FMT_		"[%s][%s-%d][%lu]rcv_msg, curl -X%s -s --noproxy '*' 'http://%s:%d%s' -H 'Content-Type:application/json;charset=UTF-8' -d'%s'\n"
#define _HTTP_MSG_FMT_	"HTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\n%s"


#define _URI1_			"/a"
#define _URI2_			"/b"
#define _URI3_			"/c"
#define _URI4_			"/d"
#define _URI5_			"/e"

#endif
