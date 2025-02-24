#include <stdio.h>

int source =1;

int* copy_addr(int **a)
{
	*a = &source;
}

int main()
{
	int b = 2;
	int **p;
	printf("source addr=%p\n", &source);

	int *b1 = &b;
	p = &b1;
	copy_addr(p);
	printf("copy addr=%p\n", *p);
}
