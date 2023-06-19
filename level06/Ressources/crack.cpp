#include <iostream>
#include <string>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <limits.h>

int auth(char *s, int a2)
{
  int i; // [esp+14h] [ebp-14h]
  int v4; // [esp+18h] [ebp-10h]
  int v5; // [esp+1Ch] [ebp-Ch]

  s[strcspn(s, "\n")] = 0;
  v5 = strnlen(s, 32);
  if ( v5 <= 5 )
    return 1;
    v4 = (s[3] ^ 0x1337) + 6221293;
    for ( i = 0; i < v5; ++i )
    {
      if ( s[i] <= 31 )
        return 1;
      v4 += (v4 ^ (unsigned int)s[i]) % 0x539;
    }
    return a2 != v4;
}


int main()
{
	char	login[] = "saluttoi";

	for (int i = 0; i < INT32_MAX; i++)
	{
		// login = std::to_string(i);
		if (auth(login, i) == 0)
		{
			std::cout << "Serial found: " << i << std::endl;
			break;
		}
		// std::cout << "Trying: " << i << std::endl;
	}
	std::cout << "end of pgm" << std::endl;
	return (0);
}