#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int main(int argc, char **argv)
// {
// 	(void)argc;
// 	char dest[40]; // [esp+14h] [ebp-2Ch] BYREF
// 	int v5; // [esp+3Ch] [ebp-4h]
// 	if (argc != 2)
// 		return 1;
// 	v5 = atoi(argv[1]);
// 	printf("Dest = %d\n", v5);

//   return 0;
// }

#include <stdbool.h>

unsigned int str_to_uint(const char* str)
{
    unsigned int result = 0;
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            result = result * 10 + (str[i] - '0');
            i++;
        }
        else
        {
            // Invalid character encountered
            break;
        }
    }

    return result;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
    const char* str = argv[1];
    unsigned int value = str_to_uint(str);
    printf("%u\n", value);
    return 0;
}