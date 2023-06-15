#include <iostream>
#include <string>
#include <algorithm>

std::string reverseBlocks(const std::string& input, int blockSize) {
    std::string reversed;
    std::string temp;

    for (unsigned int i = 0; i < input.length(); i += blockSize) {
        std::string block = input.substr(i, blockSize);
        std::reverse(block.begin(), block.end());

        for (int j = 0; j < blockSize; j += 2) {
            std::string twoChar = block.substr(j, 2);
            reversed += twoChar;
        }
    }

    return reversed;
}

std::string hexToAscii(const std::string& hexString) {
    std::string asciiString;

    for (std::size_t i = 0; i < hexString.length(); i += 2) {
        std::string byteString = hexString.substr(i, 2);
        char byte = static_cast<char>(std::strtol(byteString.c_str(), nullptr, 16));
        asciiString += byte;
    }

    return asciiString;
}

int main(int argc, char* argv[]) {

	(void)argc;
    std::string hexString = argv[1];
    std::string asciiString = hexToAscii(hexString);

    std::cout << "Hexadecimal: " << hexString << std::endl;
    std::cout << "ASCII: " << asciiString << std::endl;
	std::cout << "Password: " << reverseBlocks(asciiString, 8) << std::endl;

    return 0;
}