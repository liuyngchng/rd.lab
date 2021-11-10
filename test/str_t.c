#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

int num_s(char *hex_str)
{
	unsigned long int l = strlen(hex_str);
//	printf("strlen(chunk_length) = %ld\n", l);
	int sum = 0;
	for (int j = 0; j < l; j++)
	{
		int n ;
		char c =hex_str[j];
		if isdigit(c)
			n = c - 48;
		else
			n = c - 55;

//		printf("%d\n", n);
		sum += n * pow(16, l - 1 - j);
	}
//	printf("sum = %d\n", sum);
	return sum;
}


void get_body(char *s, char **ms, int size)
{
    char* sub_str = strstr(s, "\r\n\r\n");
    ms[0] = "ignored_header";
	char *str, *token;
    char *saveptr;
    int i;
    for (i = 0, str = sub_str;; i++,str = NULL)
    {
        token = strtok_r(str, "\r\n", &saveptr);
        if (token == NULL)
            break;
        if (i == 0)
        {
            ms[1] = token;  //length
        }
        else if (i == 1)
        {
            ms[2] = token;  //body
        }
    }
}

int main()
{
	char s[] = "HTTP/1.1 200 OK\r\nDate: Tue, 09 Nov 2021 00:22:02 GMT\r\nContent-Type: application/json;charset=UTF-8\r\nTransfer-Encoding: chunked\r\n\r\n16F4\r\n{\"pageSize\":25,\"pageNo\":0,\"total\":23,\"totalPage\":0}\r\n0\r\n\r\n";
    int size = 3;
    char **res = (char **)malloc((size +1) * sizeof(char *));
    get_body(s, res, size);
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", res[i]);
    }

    int len = num_s(res[1]);
    free(res);
	printf("len=%d\n", len);
	printf("*****\n");
	//get_len(s);


}
