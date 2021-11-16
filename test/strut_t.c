#include <stdio.h>

struct test {
	int a;
	char *b;
	float c;
};

int main()
{
    struct test test;
    test.a =1;
    test.b = "hello";
    test.c = 10.2f;
    printf("test.c = %f\n", test.c);

    struct test t1;
    t1 = (struct test){2, "hello2", 20.2f};
    printf("t1.b = %s\n", t1.b);

    struct test ts[2];
    ts[0] = (struct test){0, "ts0", 20.2f};
    ts[1] = (struct test){0, "ts1", 20.2f};
    printf("ts[0].b = %s\n", ts[0].b);
    printf("ts[1].b = %s\n", ts[1].b);


    struct test *ts3;
    ts3 = &ts;
    printf("ts3->b = %s\n", ts3->b);
    printf("(ts3+1)->b = %s\n", (ts3+1)->b);
    return 0;
}
