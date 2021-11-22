#include "context.h"
#include "util.h"
#include "cJSON.h"
<<<<<<< HEAD
#include <stddef.h>

static struct context context;

static char *ctx;

void init(int **matrix, char *data)
{
    get_bin(matrix, 20, 10, context.board);
    ctx = data;

}

cJSON *get_pce_info(char *data)
{

}
