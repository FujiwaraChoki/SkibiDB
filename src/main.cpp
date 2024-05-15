#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

#include "termcolor.hpp"
#include "console/Console.cpp"

void printAscii()
{
    std::vector<std::string> ascii = {
        "                                                                               ",
        "       .--.--.         ,-.                                ,---,        ,---,.      ",
        "      /  /    '.   ,--/ /|   ,--,     ,---,     ,--,    .'  .' `\\    ,'  .'  \\     ",
        "     |  :  /`. / ,--. :/ | ,--.'|   ,---.'|   ,--.'|  ,---.'     \\ ,---.' .' |     ",
        "     ;  |  |--`  :  : ' /  |  |,    |   | :   |  |,   |   |  .`\\  ||   |  |: |     ",
        "     |  :  ;_    |  '  /   `--'_    :   : :   `--'_   :   : |  '  |:   :  :  /     ",
        "      \\  \\    `. '  |  :   ,' ,'|   :     |,-.,' ,'|  |   ' '  ;  ::   |    ;      ",
        "       `----.   \\|  |   \\  '  | |   |   : '  |'  | |  '   | ;  .  ||   :     \\     ",
        "       __ \\  \\  |'  : |. \\ |  | :   |   |  / :|  | :  |   | :  |  '|   |   . |     ",
        "      /  /`--'  /|  | ' \\ \\'  : |__ '   : |: |'  : |__'   : | /  ; '   :  '; |     ",
        "     '--'.     / '  : |--' |  | '.'||   | '/ :|  | '.'|   | '` ,/  |   |  | ;      ",
        "       `--'---'  ;  |,'    ;  :    ;|   :    |;  :    ;   :  .'    |   :   /       ",
        "                 '--'      |  ,   / /    \\  / |  ,   /|   ,.'      |   | ,'        ",
        "                            ---`-'  `-'----'   ---`-' '---'        `----'          ",
        "                                                                               ",
    };

    // Find the length of the longest line
    size_t max_length = 0;
    for (const auto &line : ascii)
    {
        if (line.length() > max_length)
        {
            max_length = line.length();
        }
    }

    // Print each line with blue background and padding to ensure rectangle shape
    for (const auto &line : ascii)
    {
        std::cout << termcolor::on_blue;
        std::cout << line;
        // Pad the line with spaces if it's shorter than the max length
        if (line.length() < max_length)
        {
            std::cout << std::string(max_length - line.length(), ' ');
        }
        std::cout << termcolor::reset << std::endl;
    }

    std::cout << std::endl;
}

int main()
{
    printAscii();
    try
    {
        // Create a new console
        Console console;

        // Run the console
        console.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << termcolor::red << e.what() << termcolor::reset << std::endl;
    }

    // Return status code
    return 0;
}
