#include <stdio.h>
#include "cJSON.h"
#include <math.h>

void get_str_a(int *a, int size, char *a_str);

void test_j()
{
	printf("start\n");
    char *str ="{\"name\": \"Awesome 4K\",\"r\": {\"width\": 1280,\"height\": 720,\"wow\":\"wowValue\"},\"test\":[{\"hi\":\"what\"}]}";
	printf("%s\n",str);
	cJSON *j = cJSON_Parse(str);
	char *str1 = cJSON_Print(j);
	printf("string_json=%s\n", str1);

	const char *itemAdd = "addKey";
	cJSON *itemAdded = cJSON_Parse("{\"key\":\"value\"}");
	cJSON_AddItemToObjectCS(j, itemAdd, itemAdded);
	char *str2 = cJSON_Print(j);
	printf("string_json_addItem=%s\n", str2);
	cJSON *name = cJSON_GetObjectItem(j, "name");
	printf("root.name=%s\n", name->valuestring);

    //replace
	const char *replaceKey = "name";
	cJSON *item_replace = cJSON_Parse("{\"name\": \"New Awesome 4K\"");
	cJSON_ReplaceItemViaPointer(j, name, item_replace);
	char *str3 = cJSON_Print(j);
	printf("string_json_replace=%s\n", str3);

	//detach
	char *name_detach = cJSON_DetachItemFromObject(j, "name");
	printf("name_json_detach=%s\n", cJSON_Print(name_detach));
	printf("string_json_delete=%s\n", cJSON_Print(j));

    //delete
	cJSON_DeleteItemFromObject(j, "name");
	printf("string_json_delete=%s\n", cJSON_Print(j));

	cJSON *r = cJSON_GetObjectItem(j, "r");
	cJSON *wow = cJSON_GetObjectItem(r, "wow");
    printf("root.r.wow=%s\n", wow->valuestring);
    cJSON *h = cJSON_GetObjectItem(r, "height");
    printf("root.r.height=%d\n", h->valueint);
	cJSON *test = cJSON_GetObjectItem(j, "test");
	cJSON *test_1 = cJSON_GetArrayItem(test, 0);
	cJSON *hi = cJSON_GetObjectItem(test_1, "hi");
    printf("root.test[0].hi=%s\n", hi->valuestring);

    printf("cascade:root.test[0].hi=%s\n",
        cJSON_GetObjectItem(
            cJSON_GetArrayItem(
                cJSON_GetObjectItem(
                    cJSON_Parse(str),
                    "test"
                ),
                0
            ),
            "hi"
        )->valuestring
    );
	printf("the end\n");
	cJSON_Delete(j);
}

void test_int_array()
{
    int a[7] = {1,2,3,4,5,6,7};
    int size = sizeof(a)/ sizeof(a[0]);
    cJSON *int_j = cJSON_CreateIntArray(a, size);
    char *str = cJSON_Print(int_j);
	printf("int_json_str=%s\n", str);

	int b[4][4] = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};
    cJSON *int_j1 = cJSON_CreateIntArray(b, 16);
    char *str1 = cJSON_Print(int_j1);
	printf("int_json_str=%s\n", str1);
	cJSON_Delete(int_j);


}

void get_int_array(char *str_a, int *a, int size)
{
    cJSON *json = cJSON_Parse(str_a);
	char *str1 = cJSON_Print(json);
	printf("string_json=%s\n", str1);
	int size1 = cJSON_GetArraySize(json);
	printf("sizeof str=%d\n", size1);

	cJSON *a0 = cJSON_GetArrayItem(json, 0);
	printf("a0_json=%s\n", cJSON_Print(a0));
	int size2 = cJSON_GetArraySize(a0);
	printf("sizeof a0=%d\n", size2);
	for (int i = 0; i < cJSON_GetArraySize(json); i++) {
        int sum = 0;
//        int size = cJSON_GetArraySize(cJSON_GetArrayItem(json, 0));
        for (int j = 0; j < size; j++) {
            sum += (int)cJSON_GetNumberValue(
                cJSON_GetArrayItem(cJSON_GetArrayItem(json, i), j)
            ) * pow(2, j);
        }
        a[i] = sum;
	}
	cJSON_Delete(json);
}

void get_str_a(int *a, int size, char *a_str)
{
    cJSON *int_j = cJSON_CreateIntArray(a, size);
    a_str = cJSON_Print(int_j);
}

void test_int_array2d()
{
    char *str = "[\
                    [0, 1, 0, 0],\
                    [1, 1, 1, 0],\
                    [0, 0, 0, 0],\
                    [0, 0, 0, 0]\
                ]";
    int a[4];
    int size = 4;
    get_int_array(str, a, size);
    for(int i = 0; i< 4; i++) {
        printf("a[%d] = %d\n", i, a[i]);
    }
    char *str_a;
    get_str_a(a, size, str_a);
    printf("str_a=%s\n", str_a);
}


int main()
{
    test_int_array2d();
//    test_int_array();
//    test_j();
}
