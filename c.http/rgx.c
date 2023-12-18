/**
 * regexp util
 * @author whoami@hotmail.com
 * @since 2022-06-01
 */
#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

#include "utl.h"

int match(const char *s, const char *p, char *const t, const int n) {
    regex_t oregex;   // 编译后的结构体
    regmatch_t pmatch[3];	//匹配到的结果
    int ret = 0;
    if ((ret = regcomp(&oregex, p, REG_EXTENDED | REG_NOSUB)) == 0) {// 编译
        //成功编译
        if ((ret = regexec(&oregex, s, sizeof(pmatch), pmatch, 0)) == 0) {// 执行匹配不保存匹配的返回值
            printf("[%s][%s-%d]%s matches %s\n",gettime(), filename(__FILE__), __LINE__, s, p);
//            for(int i=0; i< sizeof(pmatch); i++){
//            	printf("eo=%d, so=%d\n", pmatch[i].rm_eo, pmatch[i].rm_so);
//            }
            regfree(&oregex);
            return 0;
        }
    }
    char err[1024] = {0};   // 保存错误信息的数组
    size_t err_len = 0;

    err_len = regerror(ret, &oregex, err, sizeof(err)); // 如果出错得到错误信息
    err_len = err_len < sizeof(err) ? err_len : sizeof(err) - 1;
    err[err_len] = '\0';
    printf("[%s][%s-%d]regex error, %s\n", gettime(), filename(__FILE__), __LINE__, err);
    regfree(&oregex);
    return 1;
}
