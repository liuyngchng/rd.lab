#ifndef DB_H
#define DB_H
/**
 *connect to a db with host as hst, user as usr, password as pwd,
 * dbname as db, port as prt
 */
int db_con(const char *hst, const char *usr, const char *pwd,
	const char *db, const int prt);

/**
 * exec select sql
 */
int db_select(const char *sql, char *const dt);

int db_insert(const char *sql);

int db_update(const char *sql);

int db_delete(const char *sql);

#endif
