#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <string>

#include "Tokenizer.hpp"

class Console
{
public:
    // Constructor
    Console();

    // Destructor
    ~Console();

    // Starts the console
    void start();

    // Stops the console
    void stop();

    // Get the running state of the console
    bool isRunning() const;

    // Set the running state of the console
    void setRunning(bool state);

    // Get the current command
    std::string getCurrentCommand() const;

    // Set the current command
    void setCurrentCommand(const std::string &command);

    // Take user input
    template <typename T>
    T input(const std::string &message);

    Tokenizer tokenizer;

private:
    bool running;
    std::string currentCommand;
};

#endif // CONSOLE_HPP
