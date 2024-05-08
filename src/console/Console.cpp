#include <string>
#include <iostream>

#include "Console.hpp"
#include "termcolor.hpp"
#include "Tokenizer.cpp"

// Constructor
Console::Console()
{
    // Set the console to running
    setRunning(true);

    this->tokenizer = Tokenizer();
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

        // Print the current command
        this->tokenizer.tokenize(getCurrentCommand());

        // Check if the command is "exit"
        if (strncmp(getCurrentCommand().c_str(), "exit", 4) == 0)
        {
            // Stop the console
            stop();
        }
        else
        {
            while (this->tokenizer.hasMoreTokens())
            {
                std::string token = this->tokenizer.nextToken();
                std::cout << "Console::start: " << token << std::endl;
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
