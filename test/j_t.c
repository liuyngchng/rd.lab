#include <stdio.h>
#include "cJSON.h"

void test_j()
{
	printf("start\n");
    char *str ="{\"name\": \"Awesome 4K\",\"r\": {\"width\": 1280,\"height\": 720,\"wow\":\"wowValue\"},\"test\":[{\"hi\":\"what\"}]}";
	printf("%s\n",str);
	cJSON *j = cJSON_Parse(str);
	cJSON *name = cJSON_GetObjectItem(j, "name");
	printf("root.name=%s\n", name->valuestring);
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

int main()
{
    test_j();
}
