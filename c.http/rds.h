#ifndef RDS_H
#define RDS_H

/**
 * connect to a redis server
 */
int rds_con(const char* ip, const int port, const char *psw);

/**
 * get a string value of the key
 */
int rds_get(const char *key, char *value);


/**
 * set a string value of the key
 */
int rds_set(const char *key, const char *value);

#endif
