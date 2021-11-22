#include <math.h>
#include "util.h"

int** get_matrix(int *b, int size_b, int col_n, int **c)
{

    for (int i=0; i<size_b; i++) {
        for (int j = 0; j < col_n; j++) {
            c[i][j] = (b[i] >> j) & 1;
        }
    }
    return c;
}
//get bin list of a matrix
int* get_bin(int **m, int row_n, int col_n, int *b)
{
    for (int i=0; i<row_n; i++) {
        int tmp = 0;
        for (int j=0; j<col_n; j++)
            tmp += m[i][j] * pow(2, j);
        b[i] =(int)tmp;
    }
    return b;
}
//shift mat2d for x
int *shift(int *bin_l, int bin_size, int x)
{
    for (int i=0; i< bin_size; i++)
        bin_l[i] <<= x;
    return bin_l;
}
