#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#include "syntax.hpp"
#include "Console.hpp"
#include "termcolor.hpp"
#include "utilities.hpp"
#include "Tokenizer.cpp"
#include "FileManager.hpp"

// Constructor
Console::Console()
{
    // Set the console to running
    setRunning(true);

    this->syntax = getSyntaxKeywords();
    this->fileManager = FileManager();
}

// Destructor
Console::~Console()
{
    // Stop the console
    stop();
}

// Starts the console
void Console::start()
{
    // Loop until the console is stopped
    while (isRunning())
    {
        // Take user input
        std::string command = input<std::string>("> ");

        // Set the current command
        setCurrentCommand(command);

        std::cout << getCurrentCommand() << std::endl;

        // Check if the command is "exit"
        if (strncmp(getCurrentCommand().c_str(), "exit", 4) == 0)
        {
            // Stop the console
            stop();
        }
        else
        {
            // Tokenize the command
            this->tokenizer = Tokenizer(getCurrentCommand());

            // Loop until there are no more tokens
            while (this->tokenizer.hasMoreTokens())
            {
                // Get the next token
                std::string token = this->tokenizer.nextToken();

                std::cout << toUpperCase(token) << std::endl;

                int count = std::count(this->syntax.begin(), this->syntax.end(), toUpperCase(token));

                if (count > 0)
                {
                    // Command starts with keyword
                    std::cout << termcolor::green << "Keyword: " << token << termcolor::reset << std::endl;
                }
                else
                {
                    // Not a keyword
                    std::cout << termcolor::red << "Not a keyword: " << token << termcolor::reset << std::endl;
                }
            }
        }
    }
}

// Stops the console
void Console::stop()
{
    // Set the console to not running
    setRunning(false);
}

// Get the running state of the console
bool Console::isRunning() const
{
    return running;
}

// Set the running state of the console
void Console::setRunning(bool state)
{
    running = state;
}

// Get the current command
std::string Console::getCurrentCommand() const
{
    return currentCommand;
}

// Set the current command
void Console::setCurrentCommand(const std::string &command)
{
    currentCommand = command;
}

// Take user input
template <typename T>
T Console::input(const std::string &message)
{
    T value;
    std::cout << termcolor::green << message << termcolor::reset;
    std::getline(std::cin, value);

    return value;
}
