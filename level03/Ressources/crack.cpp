#include <iostream>
#include <string>
#include <limits>

std::string xorWithString(const std::string& str, int xorNumber) {
	std::string result;
	
	for (char c : str) {
		// Perform XOR operation on ASCII value of each character
		char xoredChar = c ^ xorNumber;
		
		// Append the result character to the output string
		result += xoredChar;
	}
	
	return result;
}

int main() {
	std::string inputString = "Q}|u`sfg~sf{}|a3";
	unsigned long xorNumber = 0;
	
	while (xorNumber != ULONG_MAX)
	{
		std::string xoredString = xorWithString(inputString, xorNumber);
		if (!xoredString.compare("Congratulations!"))
		{
			std::cout << "XOR number is: " << xorNumber << std::endl;
			return (0);
		}
		xorNumber++;
	}
	return 0;
}

// int decrypt(char a1) {
//     unsigned int i;
//     unsigned int v3;
//     char v4[29];

//     strcpy(v4, "Q}|u`sfg~sf{}|a3");
//     v3 = strlen(v4);
//     for (i = 0; i < v3; ++i)
//         v4[i] ^= a1;
//     if (!strcmp(v4, "Congratulations!"))
// 	{
//         return 0;
// 	}
//     else
//         return 1;
// }

// int main() {
//     for (int a1 = 0; a1 <= 255; ++a1) {
//         if (decrypt(static_cast<char>(a1)) == 0) {
//             std::cout << "Found correct value for a1: " << a1 << std::endl;
//             break;
//         }
//     }

//     return 0;
// }