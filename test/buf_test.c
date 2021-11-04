#include <stdio.h>
#include <string.h>

int main()
{
	char buf[10];
    memset(buf,0,sizeof(buf));
	printf("buf[0]=%c\n", buf[0]);
	printf("int(buf[0])=%d\n", buf[0]);
	printf("%s\n", buf);
	buf[0]='0';
	buf[1]='\0';
	printf("buf[1]=%c\n", buf[1]);
	printf("int(buf[1])=%d\n", buf[1]);
	buf[2]='\4';
	printf("buf[2]=%c\n", buf[2]);
	printf("int(buf[2])=%d\n", buf[2]);
	printf("%s\n", buf);
	char c = 48;
	printf("c=%c\n",c);
	printf("d=%d\n",c);
	
}
