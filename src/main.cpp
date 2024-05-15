#include <stdint.h>

#include <iostream>
#include <fstream>
#include <string>

#include "termcolor.hpp"
#include "console/Console.cpp"

std::string getRootDir()
{
    std::string ROOT_PATH = __FILE__;
    size_t found = ROOT_PATH.find_last_of("/\\");

	if (found != std::string::npos)
	{
		ROOT_PATH = ROOT_PATH.substr(0, found);
	}

	// Check if ends with src, if yes, remove it (level up), then return
	if (ROOT_PATH.substr(ROOT_PATH.length() - 3) == "src")
	{
		ROOT_PATH = ROOT_PATH.substr(0, ROOT_PATH.length() - 3);
	}

    return ROOT_PATH;
}

void printAscii()
{
	/* Prints the Logo ASCII Art. */
	std::string asciiArtPath = getRootDir() + "/assets/art.txt";

	std::cout << asciiArtPath << std::endl;

	// Read from assets/art.txt
	std::ifstream file(asciiArtPath);

	// Print the ASCII Art
	std::string line;
	while (std::getline(file, line))
	{
		std::cout << termcolor::on_blue << line << termcolor::reset << std::endl;
	}

	// Close the file
	file.close();

	std::cout << std::endl
			  << std::endl;
	std::cout << termcolor::cyan << "[INFO] " << termcolor::reset << "Root directory: " << getRootDir() << std::endl;
}

int main()
{
	printAscii();

	// Create a new console
	Console console;

	// Run the console
	console.start();

	// Return status code
	return EXIT_SUCCESS;
}
