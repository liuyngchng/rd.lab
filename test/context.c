#include "context.h"
#include "util.h"
#include "cJSON.h"
#include <stddef.h>

static struct context context;

static char *mtx_data;

void init(int **matrix, char *data)
{
    get_bin(matrix, 20, 10, context.board);
    mtx_data = data;

}

cJSON *get_pce_info()
{
    cJSON *j = cJSON_Parse(mtx_data);
    const char *key1 = "spirit_this";
    const char *key2 = "mat2d";
    cJSON *pce_info = cJSON_GetObjectItem(j, key1);
    cJSON *mat2d = cJSON_DetachItemFromObject(pce_info, key2);
//    cJSON_DeleteItemFromObject(pce_info, "mat2d");
    char *mat2d_str = cJSON_Print(mat2d);
    char *mat2d_int;
    get_bin()
    cJSON *itemAdded = cJSON_Parse("{\"key\":\"value\"}");
	cJSON_AddItemToObjectCS(pce_info, key2, itemAdded);
    cJSON_Delete(mat2d);
    cJSON_Delete(j);


}
