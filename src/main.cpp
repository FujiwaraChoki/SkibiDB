#include <iostream>
#include <fstream>
#include <string>

void printAscii()
{
	/*
	Prints the Skibidi Toilet ASCII Art.
	*/
	// Read from assets/art.txt
	std::ifstream file("assets/art.txt");

	// Print the ASCII Art
	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
	}

	// Close the file
	file.close();
}

int main()
{
	std::cout << "Hello World!" << std::endl;

	return 0;
}
