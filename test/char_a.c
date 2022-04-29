#include <stdio.h>
#include <stdlib.h>

void set_data(char ** s, int size)
{
    s[0] = "h";
    s[1] = "lo";
    s[2] = "what?";
    s[3] = "the end";
}

int main()
{
    int size = 4;
    char **res = (char **)malloc((size +1) * sizeof(char *));
    set_data(res, size);
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", res[i]);
    }
    free(res);
    char *res1[size];
    res1[0] = "hello";
    res1[1] = "who";
    res1[2] = "are";
    res1[3] = "you";
    for (int i = 0; i < size; i++)
    {
        printf("res1[%d]=%s\n",i ,res1[i]);
    }
//    free(res1);

}
