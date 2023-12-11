#ifndef CFG_H
#define CFG_H
/**
 * 从默认配置文件./cfg.ini中读取配置项
 * f, 文件名称
 */
void initcfg();
/**
 * 从文件f中读取配置项
 * f, 文件名称
 */
int getcfg(const char *f);

#define __SSL1_0__		"1.0"
#endif
