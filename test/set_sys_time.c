/**
 * 需要root 用户权限执行 hwclock
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

void set_time(int year,int month,int day,int hour,int min,int sec)
{
    struct tm tptr;
    struct timeval tv;

    tptr.tm_year = year - 1900;
    tptr.tm_mon = month - 1;
    tptr.tm_mday = day;
    tptr.tm_hour = hour;
    tptr.tm_min = min;
    tptr.tm_sec = sec;

    tv.tv_sec = mktime(&tptr);
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);

}

void main()
{

    printf("before time set:\n");
    fflush(stdout);
    system("date");
    system("hwclock");

    set_time(2012,10,10,1,30,8);
    system("hwclock -w");

    printf("after time set:\n");
    fflush(stdout);
    system("date");
    system("hwclock");
}
