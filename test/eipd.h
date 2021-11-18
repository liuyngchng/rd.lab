#ifndef EIPD_H
#define EIPD_H

int get_len(int *a);
double evl(int *brd, int *pce_x, int pos_y, int col, int col_max, int rw_n);
int *rvs(int *brd);
int get_ld_h(int *pce_x, int pce_x_size, int pos_y, int rw_n);
int get_rw_n(int *brd);
int get_len(int *a);
int get_rw_rm(int *brd, int col_max, int rw_n);
int *pop_i(int *a, int i);
int *insert_i(int *a, int i, int i_v);
int get_rw_t(int *brd, int col, int rw_n);
int get_col_t(int *brd, int col, int rw_n, int col_max);
int get_col_t1(int *brd, int col, int rw_n, int col_max);
int gt_hl(int *brd, int col, int rw_n);
int gt_wl(int *brd, int col, int rw_n);
#endif
