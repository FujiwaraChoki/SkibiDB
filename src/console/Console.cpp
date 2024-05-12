#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "syntax.hpp"
#include "Console.hpp"
#include "termcolor.hpp"
#include "utilities.hpp"
#include "Tokenizer.cpp"
#include "FileManager.hpp"

Console::Console()
{
    // Set the console to running
    setRunning(true);

    this->syntax = getSyntax();
    this->fileManager = new FileManager();
    // Assign the pointer directly
    this->skibiDB = this->fileManager->getSkibiDB();
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

        // Check if the command is "exit"
        if (strcmp(getCurrentCommand().c_str(), "exit") == 0)
        {
            // Stop the console
            stop();
        }
        else
        {
            // Tokenize the command
            std::string foundSyntax;

            // Loop through syntax template statements (SQL)
            for (const auto &syntax : this->syntax)
            {
                // TODO: Fix this, as it doens't work!!

                // See if cleaned syntax is in token
                std::string cleaned = cleanSyntax(syntax);

                // Set the found syntax
                foundSyntax = syntax;
            }

            // Execute the syntax
            if (!foundSyntax.empty())
            {
                // Tokenize the command
                Tokenizer tokenizer(getCurrentCommand());

                // Get the tokens
                std::vector<std::string> tokens = tokenizer.tokenize();

                for (int i = 0; i < tokens.size(); i++)
                {
                    std::string token = tokens[i];
                    // Starts with CREATE
                    if ((strcmp("CREATE", token.c_str()) == 0) && (strcmp("TABLE", tokens[i + 1].c_str()) == 0))
                    {
                        std::string tableName = tokens[i + 2];

                        // Vector of map of attributes
                        std::vector<std::map<std::string, std::string>> attributes;

                        // Remove all parantheses
                        std::string cleanCommand = getCurrentCommand();
                        cleanCommand.erase(std::remove(cleanCommand.begin(), cleanCommand.end(), '('), cleanCommand.end());
                        cleanCommand.erase(std::remove(cleanCommand.begin(), cleanCommand.end(), ')'), cleanCommand.end());

                        // Attributes string is everything after tokens[i + 2]
                        std::vector<std::string> tokensAfterTableName(tokens.begin() + i + 3, tokens.end());

                        // Join the tokens
                        std::string attrStr = join(tokensAfterTableName, " ");

                        // Remove all ( and )
                        attrStr.erase(std::remove(attrStr.begin(), attrStr.end(), '('), attrStr.end());
                        attrStr.erase(std::remove(attrStr.begin(), attrStr.end(), ')'), attrStr.end());

                        std::cout << attrStr << std::endl;

                        // Tokenize the attributes
                        Tokenizer attrTokenizer(attrStr);
                        std::vector<std::string> attrTokens = attrTokenizer.tokenize();

                        for (int j = 0; j < attrTokens.size(); j += 2)
                        {
                            if (j + 1 < attrTokens.size()) // Ensure there are enough tokens remaining
                            {
                                // Set the attribute name
                                std::string name = attrTokens[j];

                                // Set the attribute type
                                std::string type = attrTokens[j + 1];

                                // Add the attribute to the list
                                std::map<std::string, std::string> attribute;
                                attribute["name"] = toLowerCase(name);
                                attribute["type"] = toLowerCase(type);

                                // Add the attribute to the list of attributes
                                attributes.push_back(attribute);
                            }
                        }

                        // Create the table
                        this->skibiDB->addTable(tableName, attributes);

                        // Save the database
                        this->fileManager->save();
                    }
                    else if ((strcmp("SHOW", token.c_str()) == 0) && (strcmp("TABLES", tokens[i + 1].c_str()) == 0))
                    {
                        // Show all tables
                        std::vector<Table> tables = this->skibiDB->getTables();

                        for (Table table : tables)
                        {
                            std::cout << termcolor::on_bright_grey << "=> " << table.getTableName() << termcolor::reset << std::endl;
                        }
                    }
                }
            }
            else
            {
                std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid statement." << std::endl;
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
    std::string inputString;
    std::cout << termcolor::green << message << termcolor::reset;
    std::getline(std::cin, inputString);

    return inputString;
}
