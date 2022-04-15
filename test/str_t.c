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


void split(char *s, char **ms, int size)
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

/*
headerTransfer-Encoding: chunked\r\n\r\n
[size]\r\n[data]\r\n
[size]\r\n[data]\r\n
[0]\r\n\r\n
*/
int test()
{
//	char s[] = "HTTP/1.1 200 OK\r\nDate: Tue, 09 Nov 2021 00:22:02 GMT\r\nContent-Type: application/json;charset=UTF-8\r\nTransfer-Encoding: chunked\r\n\r\n16F4\r\n{\"pageSize\":25,\"pageNo\":0,\"total\":23,\"totalPage\":0}\r\n0\r\n\r\n";
    char s[] = "HTTP/1.1 200 OK\r\nDate: Fri, 12 Nov 2021 00:21:06 GMT\r\nContent-Type: application/json;charset=UTF-8\r\nTransfer-Encoding: chunked\r\n\r\n16F4\r\n{\"pageSize\":25,\"pageNo\":0,\"total\":23,\"totalPage\":0,\"rows\":[{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2020-10-29 19:46:23.509\",\"target_ip\":\"192.168.0.2\",\"id\":1,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2020-10-29 19:46:33.981\",\"target_ip\":\"192.168.0.2\",\"id\":2,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2020-10-30 08:24:04.821\",\"target_ip\":\"192.168.0.2\",\"id\":3,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2020-10-30 08:26:00.053\",\"target_ip\":\"192.168.0.2\",\"id\":4,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2020-10-30 11:16:42.308\",\"target_ip\":\"192.168.0.2\",\"id\":5,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2020-10-30 11:17:22.641\",\"target_ip\":\"192.168.0.2\",\"id\":6,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2020-10-30 13:40:00.189\",\"target_ip\":\"192.168.0.2\",\"id\":7,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-01-22 09:28:08.681\",\"target_ip\":\"192.168.0.2\",\"id\":8,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-04-13 14:26:37.179\",\"target_ip\":\"192.168.0.2\",\"id\":9,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-04-13 14:27:17.731\",\"target_ip\":\"192.168.0.2\",\"id\":10,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-04-13 14:27:31.612\",\"target_ip\":\"192.168.0.2\",\"id\":11,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-04-13 14:29:15.613\",\"target_ip\":\"192.168.0.2\",\"id\":12,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-04-13 14:29:25.919\",\"target_ip\":\"192.168.0.2\",\"id\":13,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:37:55.572\",\"target_ip\":\"192.168.0.2\",\"id\":14,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:38:07.423\",\"target_ip\":\"192.168.0.2\",\"id\":15,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:38:12.207\",\"target_ip\":\"192.168.0.2\",\"id\":16,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:38:13.128\",\"target_ip\":\"192.168.0.2\",\"id\":17,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:38:13.959\",\"target_ip\":\"192.168.0.2\",\"id\":18,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:39:03.312\",\"target_ip\":\"192.168.0.2\",\"id\":19,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:39:20.958\",\"target_ip\":\"192.168.0.2\",\"id\":20,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:39:21.758\",\"target_ip\":\"192.168.0.2\",\"id\":21,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:39:22.430\",\"target_ip\":\"192.168.0.2\",\"id\":22,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:39:23.286\",\"target_ip\":\"192.168.0.2\",\"id\":23,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:39:25.102\",\"target_ip\":\"192.168.0.2\",\"id\":24,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"},{\"task_name\":\"abc.pdf\",\"data_size\":\"100\",\"complete_per\":\"100%\",\"create_time\":\"2021-10-29 08:39:25.901\",\"target_ip\":\"192.168.0.2\",\"id\":25,\"task_type\":\"2\",\"finish_time\":\"2020-11-11 08:00:39.000\",\"origin_ip\":\"192.168.0.1\",\"status\":\"3\"}]}\r\n0\r\n\r\n";
    printf("size=%ld, strlen=%ld\n",sizeof(s), strlen(s));
    char *bd = strstr(s, "\r\n\r\n");
    printf("bd=%s\n", bd);
    char *ed = strstr(bd + 4, "\r\n\r\n");
    printf("ed=%s\n", ed);
    char *s1[2];
    s1[0] = "hello\0";
    s1[1] = "who are you?";
    printf("size=%ld, strlen=%ld\n",sizeof(s1), strlen(s1));

    int size = 3;
    char **res = (char **)malloc((size +1) * sizeof(char *));
    split(s, res, size);
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", res[i]);
    }

    char* sub1= strstr(res[2], "{");

    int len = num_s(res[1]);
    free(res);
	printf("len=%d\n", len);
	printf("*****\n");
	//get_len(s);
}

void set_value(char **str)
{
    str[0] = "hello\0";
    printf("str=%s\n",str[0]);
}

void get_value(char *str)
{
    printf("str=%s\n",str);
}

int set_value_test()
{
//    test();
    char **str = malloc(2 * sizeof(char*));
    printf("v1 =%s\n", str[0]);
    set_value(str);
    printf("v2 =%s\n", str[0]);
    free(str);
}

int main()
{
    char str[20] = {"hello, world!"};
    get_value(str);
}
