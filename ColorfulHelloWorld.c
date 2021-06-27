#include <stdio.h>

int main()
{
	for (int i = 0; i < 256; i++)
	{
		printf("\033[38;5;%dmHello World!\n", i);
	}
	printf("\033[38;5;0m");
	return 0;
}
