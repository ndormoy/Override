#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	(void)argc;
	char dest[40]; // [esp+14h] [ebp-2Ch] BYREF
	int v5; // [esp+3Ch] [ebp-4h]
	if (argc != 2)
		return 1;
	v5 = atoi(argv[1]);
	printf("Dest = %d\n", v5);

  return 0;
}