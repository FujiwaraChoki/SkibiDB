#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

#include "syntax.hpp"
#include "Console.hpp"
#include "termcolor.hpp"
#include "utilities.hpp"
#include "Tokenizer.cpp"
#include "Attribute.hpp"
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
        std::string command = input<std::string>("\n> ");

        // Set the current command
        setCurrentCommand(command);

        // Check if the command is "exit"
        if (strcmp(getCurrentCommand().c_str(), "exit") == 0)
        {
            // Ask the user if they want to save the database
            std::string save = input<std::string>("Save database? (y/n): ");

            // If the user wants to save the database
            if (strcmp(save.c_str(), "y") == 0)
            {
                // Save the database
                this->fileManager->save();
            }

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
                                attribute["name"] = name;
                                attribute["type"] = type;

                                // Add the attribute to the list of attributes
                                attributes.push_back(attribute);
                            }
                        }

                        std::map<std::string, std::string> attr1;
                        attr1["name"] = "__id__";
                        attr1["type"] = "string";
                        attributes.push_back(attr1);

                        std::map<std::string, std::string> attr2;
                        attr2["name"] = "__created_at__";
                        attr2["type"] = "string";
                        attributes.push_back(attr2);

                        std::map<std::string, std::string> attr3;
                        attr3["name"] = "__row__";
                        attr3["type"] = "int";
                        attributes.push_back(attr3);

                        // Create the table
                        this->skibiDB->addTable(tableName, attributes);

                        std::cout << termcolor::magenta << "[NOTE] " << termcolor::reset;
                        std::cout << termcolor::italic << "It is recommended to save the database after creating a table." << termcolor::reset << std::endl;
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
                    else if (strcmp("INSERT", token.c_str()) == 0)
                    {
                        std::string tableName = tokens[i + 2];

                        // Get all tokens until VALUES word appears
                        std::vector<std::string> tokensBeforeValues;

                        for (int j = i + 3; j < tokens.size(); j++)
                        {
                            if (strcmp("VALUES", tokens[j].c_str()) == 0)
                            {
                                break;
                            }

                            tokensBeforeValues.push_back(tokens[j]);
                        }

                        // Join the tokens
                        std::string attributes = join(tokensBeforeValues, " ");

                        // Remove all parentheses and commas from attributes
                        attributes.erase(std::remove(attributes.begin(), attributes.end(), '('), attributes.end());
                        attributes.erase(std::remove(attributes.begin(), attributes.end(), ')'), attributes.end());
                        attributes.erase(std::remove(attributes.begin(), attributes.end(), ','), attributes.end());

                        std::vector<std::string> values;

                        // Extract the values
                        std::string valuesString;
                        for (int j = i + 5; j < tokens.size(); j++)
                        {
                            if (strcmp("VALUES", tokens[j].c_str()) == 0)
                            {
                                // Move to the next token
                                continue;
                            }

                            valuesString = tokens[j];
                            break;
                        }

                        // Split valuesString by commas
                        std::istringstream iss(valuesString);
                        std::string value;
                        while (std::getline(iss, value, ','))
                        {
                            // Remove leading and trailing whitespaces
                            value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
                            value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
                            if (isNumber(value))
                            {
                                // check if double or int
                                if (value.find('.') != std::string::npos)
                                {
                                    // double
                                    value = "double:" + value;
                                }
                                else
                                {
                                    // int
                                    value = "int:" + value;
                                }
                            }

                            // Remove double quotes
                            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());

                            // Remove ( and )
                            value.erase(std::remove(value.begin(), value.end(), '('), value.end());
                            value.erase(std::remove(value.begin(), value.end(), ')'), value.end());

                            values.push_back(value);
                        }

                        // Tokenize the attributes
                        Tokenizer attrTokenizer(attributes);
                        std::vector<std::string> tokAttributes = attrTokenizer.tokenize();

                        // Insert the row
                        Table &table = this->skibiDB->getTable(tableName);

                        // Add the row to the table
                        table.addRow(tokAttributes, values);
                    }
                    else if (strcmp("SAVE", toUpperCase(token).c_str()) == 0)
                    {
                        this->fileManager->save();

                        std::cout << termcolor::green << "[SUCCESS] " << termcolor::reset << "Database saved." << std::endl;
                    }
                    else if (strcmp("HELP", token.c_str()) == 0)
                    {
                        std::cout << termcolor::cyan << "[HELP] " << termcolor::reset << "Commands:" << std::endl;
                        std::cout << termcolor::cyan << "[HELP] " << termcolor::reset << "CREATE TABLE <table_name> (<attribute_name> <attribute_type>, ...)" << std::endl;
                        std::cout << termcolor::cyan << "[HELP] " << termcolor::reset << "SHOW TABLES" << std::endl;
                        std::cout << termcolor::cyan << "[HELP] " << termcolor::reset << "INSERT INTO <table_name> (<attribute_name>, ...) VALUES (<value>, ...)" << std::endl;
                        std::cout << termcolor::cyan << "[HELP] " << termcolor::reset << "SELECT <column_name>, ... FROM <table_name> [WHERE <condition>]" << std::endl;
                        std::cout << termcolor::cyan << "[HELP] " << termcolor::reset << "SAVE" << std::endl;
                        std::cout << termcolor::cyan << "[HELP] " << termcolor::reset << "EXIT" << std::endl;
                    }
                    else if (strcmp("SELECT", token.c_str()) == 0)
                    {
                        if (tokens.size() >= 6 && strcmp("COUNT", tokens[i + 1].c_str()) == 0 && strcmp("COLUMNS", tokens[i + 2].c_str()) == 0 && strcmp("FROM", tokens[i + 4].c_str()) == 0)
                        {
                            std::string tableName = tokens[i + 5];

                            // Retrieve the table from the database
                            Table &table = this->skibiDB->getTable(tableName);

                            // Retrieve the column names from the table
                            std::vector<std::string> columnNames = table.getColumnNames();

                            // Count the number of columns
                            int columnCount = columnNames.size();

                            // Print the column count
                            std::cout << "Number of columns in table " << tableName << ": " << columnCount << std::endl;
                        }
                        else if (tokens.size() >= 4 && strcmp("*", tokens[i + 1].c_str()) == 0 && strcmp("FROM", tokens[i + 2].c_str()) == 0)
                        {
                            std::string tableName = tokens[i + 3];

                            // Retrieve the table from the database
                            Table &table = this->skibiDB->getTable(tableName);

                            // Retrieve the rows from the table
                            std::vector<std::map<std::string, std::string>> rows = table.getRows();

                            // Print the rows
                            for (const auto &row : rows)
                            {
                                for (const auto &column : row)
                                {
                                    std::cout << column.first << ": " << column.second << " | ";
                                }
                                std::cout << std::endl;
                            }
                        }
                        else
                        {
                            std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid SELECT syntax." << std::endl;
                        }
                    }

                    else if (strcmp("DELETE", token.c_str()) == 0)
                    {
                        std::string tableName = tokens[i + 2];

                        if (strcmp("FROM", tokens[i + 1].c_str()) == 0)
                        {
                            // Delete a row
                            std::vector<std::string> whereClause;

                            for (int j = i + 3; j < tokens.size(); j++)
                            {
                                if (strcmp("WHERE", tokens[j].c_str()) == 0)
                                {
                                    // Skip WHERE
                                    j++;
                                    while (j < tokens.size())
                                    {
                                        whereClause.push_back(tokens[j]);
                                        j++;
                                    }
                                    break;
                                }
                            }

                            // Get the table
                            Table &table = this->skibiDB->getTable(tableName);

                            // Delete the row
                            table.deleteRow(whereClause);

                            std::cout << termcolor::green << "[SUCCESS] " << termcolor::reset << "Row deleted." << std::endl;
                        }
                    }
                    else if (strcmp("UPDATE", token.c_str()) == 0)
                    {
                        std::string tableName = tokens[i + 2];
                        if (tableName.empty())
                        {
                            std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Table name is empty." << std::endl;
                            exit(1);
                        }

                        // Get this table
                        Table &table = this->skibiDB->getTable(tableName);

                        // Get the set clause
                        std::vector<std::string> setClause;
                        for (int j = i + 3; j < tokens.size(); j++)
                        {
                            if (strcmp("SET", tokens[j].c_str()) == 0)
                            {
                                // Skip SET
                                j++;
                                while (j < tokens.size())
                                {
                                    setClause.push_back(tokens[j]);
                                    j++;
                                }
                                break;
                            }
                        }

                        // Get the where clause
                        std::vector<std::string> whereClause;
                        for (int j = i + 3; j < tokens.size(); j++)
                        {
                            if (strcmp("WHERE", tokens[j].c_str()) == 0)
                            {
                                // Skip WHERE
                                j++;
                                while (j < tokens.size())
                                {
                                    whereClause.push_back(tokens[j]);
                                    j++;
                                }
                                break;
                            }
                        }

                        // Update the row
                        //table.updateRow(setClause, whereClause);

                    }

                    else if (strcmp("DROP", token.c_str()) == 0)
                    {
                        // Remove table
                        std::string tableName = tokens[i + 2];

                        if (tableName.empty())
                        {
                            std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Table name is empty." << std::endl;
                            exit(1);
                        }

                        // Delete table
                        this->skibiDB->removeTable(tableName);

                        std::cout << termcolor::magenta << "[NOTE] " << termcolor::reset;
                        std::cout << termcolor::italic << "It is recommended to save the database after deleting a table." << termcolor::reset << std::endl;
                    }
                    else if ("ALTER", token.c_str() == 0)
                    {
                        // Alter table
                        std::string tableName = tokens[i + 2];
                        std::string action = tokens[i + 3];

                        if (tableName.empty())
                        {
                            std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Table name is empty." << std::endl;
                            exit(1);
                        }

                        // Get the table
                        Table &table = this->skibiDB->getTable(tableName);

                        if (strcmp("ADD", action.c_str()) == 0)
                        {
                            // Check if the attribute already exists
                            for (const auto &attr : table.getAttributes())
                            {
                                if (strcmp(toLowerCase(attr.getAttributeName()).c_str(), toLowerCase(tokens[i + 4]).c_str()) == 0)
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Attribute already exists." << std::endl;
                                    exit(1);
                                }
                            }

                            // Get the attribute name
                            std::string attributeName = tokens[i + 4];

                            // Get the attribute type
                            std::string attributeType = tokens[i + 5];

                            // Add the attribute to the table
                            table.addAttribute(Attribute(attributeName, attributeType));

                            std::cout << termcolor::magenta << "[NOTE] " << termcolor::reset;
                            std::cout << termcolor::italic << "It is recommended to save the database after altering a table." << termcolor::reset << std::endl;
                        }
                        else if (strcmp("REMOVE", action.c_str()) == 0)
                        {
                            // Remove an attribute from the table
                            // ALTER TABLE <table_name> REMOVE <attribute_name>

                            // Get the attribute name
                            std::string attributeName = tokens[i + 4];

                            // Remove the attribute from the table
                            table.removeAttribute(attributeName);

                            std::cout << termcolor::magenta << "[NOTE] " << termcolor::reset;
                            std::cout << termcolor::italic << "It is recommended to save the database after altering a table." << termcolor::reset << std::endl;
                        }
                        else
                        {
                            std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid action." << std::endl;
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
    std::cout << termcolor::magenta << message << termcolor::reset;
    std::getline(std::cin, inputString);

    return inputString;
}
