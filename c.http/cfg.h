#ifndef CFG_H
#define CFG_H
/**
 * 从默认配置文件./cfg.ini中读取配置项
 * f, 文件名称
 */
void initcfg();

/**
 * 从指定文件full path fname 中读取配置
 */
void initcfg1(const char *fname);
/**
 * 从文件f中读取配置项
 * f, 文件名称
 */
int getcfg(const char *f);

#endif
