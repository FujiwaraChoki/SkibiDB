#include <stdint.h>

#include <iostream>
#include <fstream>
#include <string>

#include "termcolor.hpp"
#include "console/Console.cpp"

std::string getRootDir()
{
	/* Finds the path to the assets folder dynamically. */
	std::string ROOT_PATH = __FILE__;

	// Find the last occurrence of the path separator
	size_t found = ROOT_PATH.find_last_of("/\\");

	// Keep looping until \\SkibiDB is found
	while (ROOT_PATH.substr(found) != "/SkibiDB")
	{
		// Remove the last directory from the path
		ROOT_PATH = ROOT_PATH.substr(0, found);

		// Find the last occurrence of the path separator
		found = ROOT_PATH.find_last_of("/\\");
	}

	std::cout << termcolor::green << "[INFO] " << termcolor::reset << "Root directory: " << ROOT_PATH << std::endl;

	// Return the root path
	return ROOT_PATH;
}

void printAscii()
{
	/* Prints the Logo ASCII Art. */
	std::string asciiArtPath = getRootDir() + "/assets/art.txt";

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
