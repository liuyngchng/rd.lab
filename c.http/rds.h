#ifndef RDS_H
#define RDS_H

/**
 * connect to a redis server
 */
int rds_con(const char* ip, const int port, const char *psw);

/**
 * get a string value of the key
 * return the value string length as a integer
 */
int rds_gt(const char *key, char *value);


/**
 * set a string value of the key
 */
int rds_st(const char *key, const char *value);

#endif
