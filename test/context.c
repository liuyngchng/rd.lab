#include "context.h"
#include "util.h"
#include "cJSON.h"
#include <stddef.h>

static struct context context;

void init(int **data)
{
    get_bin(data, 20, 10, context.board);

}



void update()
{

}
