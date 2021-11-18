#include "eipd.h"

double evl(int *brd, int *pce_x, int pos_y, int col, int col_max, int rw_n)
{
    rvs(brd);
    double scr = 0;
    int ld_h = get_ld_h(pce_x, 4, pos_y, rw_n);
    int rw_rm = get_rw_rm(brd, col_max, rw_n);
    int rw_t = get_rw_t(brd, col, rw_n);
    int col_t = get_col_t(brd, col, rw_n, col_max);
    int hl = gt_hl(brd, col, rw_n);
    int wl = gt_wl(brd, col, rw_n);
    scr += (ld_h * -4.500158825082766);
    scr += rw_rm * 3.4181268101392694;
    scr += rw_t * -3.2178882868487753;
    scr += col_t * -9.348695305445199;
    scr += hl * -7.899265427351652;
    scr += wl * -3.3855972247263626;
    return scr;
}

int *rvs(int *brd)
{
    return brd;
}

int get_ld_h(int *pce_x, int pce_x_size, int pos_y, int rw_n)
{
    int p_h = 0;
    int c = 0;
    for (int i; i< pce_x_size; i++) {
        if (pce_x[i])
            continue;
        c += 1;
        p_h += ((rw_n - 1) - (pos_y + i));
    }
    return p_h/c;
}

int get_rw_n(int *brd)
{
    return get_len(brd);
}

int get_len(int * array)
{
    return 0;
}

int get_rw_rm(int *brd, int col_max, int rw_n)
{
    int c = 0;
    for(int i = 0; i < rw_n; i++) {
        if (brd[i] == col_max) {
            c += 1;
            pop_i(brd, i);
            insert_i(brd, rw_n - 1, 0);
        } else if (brd[i] == 0) {
            break;
        }
    }
    return c;
}

int *pop_i(int *array, int i)
{
    return array;
}

int *insert_i(int *array, int i, int i_v)
{
    return array;
}

int get_rw_t(int *brd, int col, int rw_n)
{
    int t = 0;
    int last_bit = 1;
    int bit = 0;
    for (int i = 0; i < rw_n; i++) {
        int row = brd[i];
        if (!row) {
            t += 2;
            last_bit = 1;
            continue;
        }
        for (int j = 0; j < col; j++) {
            bit = (row >> j) & 1;
            if (bit != last_bit) {
                t += 1;
                last_bit = bit;
            }
        }
        if (!bit)
            t += 1;
        last_bit = 1;
    }
    return t;
}

int get_col_t(int *brd, int col, int rw_n, int col_max)
{
    int t = 0;
    int xor = brd[0] ^ col_max;
    for (int j = 0; j <col; j++) {
        if ((xor >> j) & 1) {
            t += 1;
        }
    }
    for (int i = 1; i < rw_n; i++) {
        int xor = brd[i] ^ brd[i - 1];
        for (int j = 0; j < col; j++) {
            if ((xor >> j) & 1) {
                t += 1;
            }
        }
    }
    return t;
}

int get_col_t1(int *brd, int col, int rw_n, int col_max)
{
    int t = 0;
    int last_bit = 1;
    for (int i = 0; i < col; i++) {
        for (int j; j <rw_n; j++) {
            int row = brd[j];
            int bit = (row >> i) & 1;
            if (bit != last_bit) {
                t += 1;
            }
            last_bit = bit;
        }
        last_bit = 1;
    }
    return t;
}

int gt_hl(int *brd, int col, int rw_n)
{
    int hl = 0;
    int rw_hl = 0b0000000000;
    int p_rw = brd[rw_n - 1];
    for (int i = rw_n - 2; i > 0; i--) {
        int c_rw = brd[i];
        rw_hl = ~c_rw & (p_rw | rw_hl);
        for (int j = 0; j < col; j++) {
            hl += ((rw_hl >> j) & 1);
        }
        p_rw = brd[i];
    }
    return hl;
}

int gt_wl(int *brd, int col, int rw_n)
{
    int wl_s = 0;
    for (int i = 1; i < col -1; i++) {
        for (int j = rw_n -1; j > 0; j--) {
            if ((!(brd[j] >> i) & 1) && (brd[j] >> (i - 1) & 1)
                && ((brd[j] >> (i + 1)) & 1)) {
                wl_s += 1;
                for (int k = j -1; j > 0; j--) {
                    if (!((brd[k] >> i) & 1))
                        wl_s += 1;
                    else
                        break;
                }
            }
        }
    }
    for (int j = rw_n - 1; j > 0; j--) {
        if ((!(brd[j] >> 0) & 1) && ((brd[j] >> 1) & 1)) {
            wl_s += 1;
            for (int k = j - 1; k > 0; k--) {
                if (!((brd[k] >> 0) & 1))
                    wl_s += 1;
                else
                    break;
            }
        }
    }
    for (int j = rw_n - 1; j > 0; j--) {
        if ((!((brd[j] >> (col - 1)) & 1)) && (brd[j] >> (col - 2) & 1)) {
            wl_s += 1;
            for (int k = j - 1; k > 0; k--) {
                if (!(brd[k] >> (col - 1) & 1))
                    wl_s += 1;
                else
                    break;
            }
        }
    }
    return wl_s;
}

int main()
{
    return 0;
}
