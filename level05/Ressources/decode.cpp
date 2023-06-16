#include <string>
#include <iostream>

int main (int argc, char **argv)
{
	(void)argc, (void)argv;
	char s[100]; // [esp+28h] [ebp-70h] BYREF
	unsigned int i; // [esp+8Ch] [ebp-Ch]

	i = 0;
	fgets(s, 100, stdin);
	std::cout << "Before FOR" << std::endl;
	for ( i = 0; i < strlen(s); ++i )
	{
		if ( s[i] > 64 && s[i] <= 90 )
		s[i] ^= 0x20u; // 0x20 = 32
	}
	std::cout << "After FOR" << std::endl;
	std::cout << s << std::endl;
	exit(0);

}