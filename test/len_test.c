#include <stdio.h>
#include <string.h>
int main()
{
 	char buf_init[] = "rsp_frm_srv";
    int a = sizeof(buf_init);
    int b = strlen(buf_init);
	printf("str=%s\n", buf_init);
    printf("sizeof=%d, strlen=%d\n", a, b);
}
