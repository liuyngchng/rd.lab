#include <cstddef>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

long int get_time()
{
   	struct timeval tv;
   	gettimeofday(&tv, NULL);
   	long int timestamp = tv.tv_sec * 1000000 + tv.tv_usec;
	return timestamp;
}

int main()
{
	long int t = get_time();
	printf("%ldus\n", t);
    sleep(1);
	printf("%ldus\n", get_time());
}
