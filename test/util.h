#ifndef UTIL_H
#define UTIL_H
// get a 2d matrix from a 1d matrix
int** get_matrix(int *b, int size_b, int col_n, int **c);
//get bin list of a matrix
int* get_bin(int **m, int row_n, int col_n, int *b);
//shift mat2d for x
int *shift(int *bin_l, int bin_size, int x);
#endif
