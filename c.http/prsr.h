#ifndef PRSR_H
#define PRSR_H
/**
 * 解析特定结构的数据，与具体的需求有关，非通用方法
 * s, source data;
 * t, 结果保存在 t 中;
 * n, 最多返回n个字节的数据
 **/
char *const prsdt(const char *s, char *const t, const int n);
char *const prs_up_rpt_dt(const char *s, char *const t, const int n);
char *const prs_dt_u(const char *s, char *const t, const int n);
char *const prs_up_rpt_dt_obj(const char *id, const char *s, char *const t, const int n);
char *const prs_evt_ext(const char *s, char *const t, const int n);
char *const prs_at_ext(const char *s, char *const t, const int n);

#endif
