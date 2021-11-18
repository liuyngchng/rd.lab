#include <stdio.h>
#include "config.h"
#include "semaphore.h"

struct test {
	int a;
	char *b;
	float c;
	int d[2][2];
};

int test()
{
    struct config cfg = get_config();
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
    struct test t4 =(struct test){2, "hello2", 20.2f, 0,1,2,3};
    printf("t4.d[1][1] = %d\n", t4.d[1][1]);

    struct test s_array[2];
    s_array[0] = (struct test){2, "s_array[0]", 20.2f, 0,1,2,3};
    printf("s_array[0].d[1][1] = %d\n", s_array[0].d[1][1]);
    return 0;
}


int main()
{
    struct semaphore smph = (struct semaphore){1, 0, 0, 0};
    printf("smph.roate=%d\n", smph.rotate);
}
