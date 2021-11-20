#include "config.h"
#include "semaphore.h"

#ifndef CONTEXT_H
#define CONTEXT_H
struct context
{
    struct config config;
    struct semaphore semaphore;
    int board[20];
    int ori_pce_pos[2];
    int cur_pce_pos[2];
    int pcs_brd[3][4][2];       //pieces border in axis x, [shape_idx].[rotate_idx].[whether_shet, left, right]
};
#endif
