#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	(void)argc;
	int a = atoi(argv[1]);
	int b = a;
	if (b >> 0x18 != 0x7f) {
		printf ("Value = %d | is Good", a);
	}
	else
		printf("Value =  %d | is Bad", a); 
}