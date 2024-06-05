#ifndef UTIL_H
#define UTIL_H
#include <string.h>

/**
 * get the filename from a full file path.
 */
//windows:
// #define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
//linux :
#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x

/**
 * 获取 HTTP request method
 * s, source string;
 * t, target string;
 * n, 最多获取 n 个字节;
 **/
char *const getmethod(const char *s, char *const t, const int n);

/**
 * 获取 http request uri
 * s, source string;
 * t, target string;
 * n, 最多获取 n 个字节;
 **/
char *const geturi(const char *s, char *const t, const int n);

/**
 * 从字符 s 中读取第 l(从0开始)行，最多读取 n 个字符,
 * s, source string;
 * t, target string, 数据保存在数组 t 中;
 * n, 最多获取 n 个字节;
 * l, 从字符 s 中第 l(从0开始)行开始读取
 */
char *const getln(const char *s, char *const t, const int n, const int l);

/**
 * 获取 http response body;
 * s, source str;
 * t, target str;
 * n, 返回的字符串最大长度
 **/
char *const getbody(const char *s, char *const t, int n);

/**
 * 获取当前时间
 * buf, 存储时间字符串的地址,格式为YYYY-MM-DD HH:mm:ss
 * n, 最多返回n个字符
 **/
char *const gettimestr(char *const buf, int n);

/**
 * 获取当前时间格式化字符串
 * 返回 YYYY-MM-DD HH:mm:ss SSS 格式的时间字符串
 **/
char *gettime();

/**
 * 获取当前时间格式化字符串
 * 返回 YYYY-MM-DD HH:mm:ss 格式的时间字符串
 **/
char *gettime1();

/**
 * 获取当前时间格式化字符串
 * 返回 YYMMDDHHmmss 格式的时间字符串
 **/
char *gettime2();

/**
 * 获取当前 ms 时间戳
 **/
unsigned long gettimestamp();

/**
 * 获取 JSON 字符串某个 key 的值
 * s, JSON 字符串;
 * t, 返回的JSON key对应的value;
 * jsonk, JSON key;
 * n, 最多返回n个字节的内容
 **/
char *const getjsonv(const char *s, char *const t, const char *jsonk, int n);

/**
 * 将数字 s 转换为 4 个字符的 hex字符串
 * s, 0～65535之内的数字
 * t, hex字符串
 */
char *const getshorthex(const unsigned short s, char *const t);
/**
 * 将一个字节的数字 s 转换为 2 个字符的 hex字符串
 * s, 0～255之内的数字
 * t, hex字符串
 */
char *const getcharhex(const char s, char *const t);

/**
 * 将一个字符数组转换为16进制hex字符串
 */
char *const getchararrayhex(const char *s, int n, char *const t);

/**
 * 读取文件内容
 * f, file name
 * t, 输出的文件内容
 * n, 最多读取的字节数
 */
char *const readf(const char *f, char *const t, int n);

char *const char2hex(const unsigned char* intput, int input_len, char *const output);

#endif

