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
#include "FileManager.hpp"

// For "clear" command later
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

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
    try
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
                                // Check if primary key
                                else if (value == "PRIMARY_KEY")
                                {
                                    value = "pk:" + value;
                                }
                                else if (value == "FOREIGN_KEY")
                                {
                                    value = "fk:" + value;
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
                            // Get columns (all tokens from SELECT to FROM)
                            std::vector<std::string> columns;
                            int fromIndex = -1;
                            int countIndex = -1;
                            std::string countColumn;

                            for (int j = i + 1; j < tokens.size(); j++)
                            {
                                if (strcmp("FROM", tokens[j].c_str()) == 0)
                                {
                                    fromIndex = j;
                                    break;
                                }

                                // Remove comma from column name
                                if (tokens[j].find(",") != std::string::npos)
                                {
                                    tokens[j].erase(std::remove(tokens[j].begin(), tokens[j].end(), ','), tokens[j].end());
                                }

                                // Add column between brackets if COUNT
                                if (tokens[j].find("COUNT") != std::string::npos)
                                {
                                    // Extract the column name
                                    std::string columnName = tokens[j].substr(tokens[j].find("(") + 1, tokens[j].find(")") - tokens[j].find("(") - 1);
                                    countColumn = columnName;
                                    columns.push_back(columnName);
                                }
                                else
                                {
                                    // Add column to columns
                                    columns.push_back(tokens[j]);
                                }
                            }

                            // Get table name
                            std::string tableName = tokens[fromIndex + 1];

                            // Check if command includes where
                            size_t whereIndex = getCurrentCommand().find("WHERE");

                            // Init rows
                            std::vector<std::map<std::string, std::string>> rows;

                            // Get the table
                            Table &table = this->skibiDB->getTable(tableName);

                            if (whereIndex != std::string::npos)
                            {

                                // Check if WHERE clause exists
                                std::vector<std::string> whereClause;

                                for (int j = fromIndex + 2; j < tokens.size(); j++)
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

                                rows = table.select(columns, whereClause);
                            }
                            else
                            {
                                // Call select function without a WHERE clause
                                rows = table.select(columns); // Empty whereClause
                            }

                            // Select the rows
                            std::vector<std::map<std::string, std::string>> visibleRows;

                            // Filter on columns
                            if (columns[0] == "*")
                            {
                                visibleRows = rows;
                            }
                            else
                            {
                                for (const auto &row : rows)
                                {
                                    for (const auto &pair : row)
                                    {
                                        if (std::find(columns.begin(), columns.end(), pair.first) != columns.end())
                                        {
                                            visibleRows.push_back(row);
                                            break;
                                        }
                                    }
                                }
                            }

                            if (visibleRows.size() > 0)
                            {
                                // Find column names
                                std::vector<std::string> columnNames;
                                for (const auto &row : rows)
                                {
                                    for (const auto &pair : row)
                                    {
                                        if (std::find(columnNames.begin(), columnNames.end(), pair.first) == columnNames.end())
                                        {
                                            columnNames.push_back(pair.first);
                                        }
                                    }
                                }

                                // Calculate column widths dynamically
                                std::vector<size_t> columnWidths(columnNames.size(), 0);
                                for (size_t i = 0; i < columnNames.size(); ++i)
                                {
                                    columnWidths[i] = columnNames[i].length(); // Initialize with column name lengths
                                    for (const auto &row : rows)
                                    {
                                        auto it = row.find(columnNames[i]);
                                        if (it != row.end())
                                        {
                                            columnWidths[i] = std::max(columnWidths[i], it->second.length());
                                        }
                                    }
                                }

                                // Check if COUNT is in the command
                                if (countColumn != "")
                                {
                                    // Count the rows
                                    std::cout << termcolor::green << "[COUNT] " << termcolor::reset << visibleRows.size() << std::endl;
                                    std::cout << std::endl;
                                }

                                if (visibleRows.size() > 0)
                                {

                                    // Print separator
                                    for (size_t i = 0; i < columnWidths.size(); ++i)
                                    {
                                        std::cout << std::setw(columnWidths[i] + 2) << std::setfill('_') << "_" << std::setfill(' ') << "|__";
                                    }
                                    std::cout << std::endl;

                                    // Print header
                                    for (size_t i = 0; i < columnNames.size(); ++i)
                                    {
                                        std::cout << std::setw(columnWidths[i] + 2) << std::left << termcolor::blue << columnNames[i] << termcolor::reset << "|  ";
                                    }
                                    std::cout << std::endl;

                                    // Print separator
                                    for (size_t i = 0; i < columnWidths.size(); ++i)
                                    {
                                        std::cout << std::setw(columnWidths[i] + 2) << std::setfill('_') << "_" << std::setfill(' ') << "|__";
                                    }
                                    std::cout << std::endl;

                                    // Print rows
                                    for (const auto &row : rows)
                                    {
                                        for (size_t i = 0; i < columnNames.size(); ++i)
                                        {
                                            auto it = row.find(columnNames[i]);
                                            if (it != row.end())
                                            {
                                                std::cout << std::setw(columnWidths[i] + 2) << std::left << termcolor::yellow << it->second << termcolor::reset << "|  ";
                                            }
                                            else
                                            {
                                                std::cout << std::setw(columnWidths[i] + 2) << std::left << " " << "|  ";
                                            }
                                        }
                                        std::cout << std::endl;
                                    }

                                    // Print separator
                                    for (size_t i = 0; i < columnWidths.size(); ++i)
                                    {
                                        std::cout << std::setw(columnWidths[i] + 2) << std::setfill('_') << "_" << std::setfill(' ') << "|__";
                                    }
                                }
                                std::cout << std::endl;
                            }
                            else
                            {
                                // Alert if no rows found
                                std::cout << termcolor::yellow << "[WARN] " << termcolor::reset << "No rows found." << std::endl;
                            }
                        }
                        else if (strcmp("clear", toLowerCase(token).c_str()) == 0)
                        {
                            // Clear the console
                            system(CLEAR);
                            std::cout << termcolor::green << "[SUCCESS] " << termcolor::reset << "Console cleared." << std::endl;
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
                            // Update a row
                            std::string tableName = tokens[i + 1];
                            std::vector<std::string> setClause;
                            std::vector<std::string> whereClause;

                            for (int j = i + 2; j < tokens.size(); j++)
                            {
                                // Correctly allocate set and where clauses
                                if (strcmp("SET", tokens[j].c_str()) == 0)
                                {
                                    j++;
                                    while (j < tokens.size())
                                    {
                                        if (strcmp("WHERE", tokens[j].c_str()) == 0)
                                        {
                                            break;
                                        }

                                        // Remove double quotes
                                        tokens[j].erase(std::remove(tokens[j].begin(), tokens[j].end(), '"'), tokens[j].end());

                                        setClause.push_back(tokens[j]);
                                        j++;
                                    }
                                }

                                if (strcmp("WHERE", tokens[j].c_str()) == 0)
                                {
                                    j++;
                                    while (j < tokens.size())
                                    {
                                        // Remove double quotes
                                        tokens[j].erase(std::remove(tokens[j].begin(), tokens[j].end(), '"'), tokens[j].end());

                                        whereClause.push_back(tokens[j]);
                                        j++;
                                    }
                                }
                            }

                            // Get the table
                            Table &table = this->skibiDB->getTable(tableName);

                            // Update the row
                            table.updateRow(setClause, whereClause);

                            // Example: UPDATE table_name SET column_name = value WHERE column_name = value
                        }
                        else if (strcmp("ALTER", token.c_str()) == 0 && strcmp("TABLE", tokens[i + 1].c_str()) == 0)
                        {
                            // Alter table
                            std::string tableName = tokens[i + 2];
                            std::string action = tokens[i + 3];
                            std::cout << tableName << std::endl;

                            if (action.empty())
                            {
                                std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Action is empty." << std::endl;
                                exit(1);
                            }

                            // Get the table
                            Table &table = this->skibiDB->getTable(tableName);
                            // Add column
                            std::string column = tokens[i + 4];

                            if (strcmp("ADD", action.c_str()) == 0)
                            {
                                // Get column name and type (embedded in parentheses)
                                std::string columnWithout = column.substr(column.find("(") + 1, column.find(")") - column.find("(") - 1);

                                // Tokenize
                                // TODO: Replace tokenizer with simpler mechanism (to save mem)
                                Tokenizer columnTokenizer(columnWithout);
                                std::vector<std::string> columnTokens = columnTokenizer.tokenize();
                                std::string columnName = columnTokens[0];
                                std::string columnType = columnTokens[1];

                                if (columnName.empty())
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Column name is empty." << std::endl;
                                    exit(1);
                                }

                                if (columnType.empty())
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Column type is empty." << std::endl;
                                    exit(1);
                                }

                                Attribute attr;
                                attr.setAttributeName(columnName);
                                attr.setAttributeType(columnType);

                                // Add the column
                                table.addColumn(attr);

                                std::cout << termcolor::magenta << "[NOTE] " << termcolor::reset;
                                std::cout << termcolor::italic << "It is recommended to save the database after adding a column." << termcolor::reset << std::endl;
                            }
                            else if (strcmp("DROP", action.c_str()) == 0)
                            {
                                if (column.empty())
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Column name is empty." << std::endl;
                                    exit(1);
                                }

                                // Drop the column
                                table.dropColumn(column);

                                std::cout << termcolor::magenta << "[NOTE] " << termcolor::reset;
                                std::cout << termcolor::italic << "It is recommended to save the database after dropping a column." << termcolor::reset << std::endl;
                            }
                            else
                            {
                                std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid action." << std::endl;
                            }
                        }
                        else if (strcmp("SET", token.c_str()) == 0)
                        {
                            if (strcmp("PK", tokens[i + 1].c_str()) == 0)
                            {
                                std::cout << "Setting primary key" << std::endl;
                                std::string tableName = tokens[i + 2];
                                std::string columnName = tokens[i + 3];

                                if (tableName.empty())
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Table name is empty." << std::endl;
                                    exit(1);
                                }

                                if (columnName.empty())
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Column name is empty." << std::endl;
                                    exit(1);
                                }
                                std::cout << tableName << std::endl;

                                // Get the table
                                Table &table = this->skibiDB->getTable(tableName);

                                // Set the primary key
                                table.setPK(columnName);

                                std::cout << termcolor::magenta << "[NOTE] " << termcolor::reset;
                                std::cout << termcolor::italic << "It is recommended to save the database after setting a primary key." << termcolor::reset << std::endl;

                                // xmpl: SET PK table_name column_name
                            }
                            else if (strcmp("FK", tokens[i + 1].c_str()) == 0)
                            {
                                // Add Foreign key using .addFK
                                std::string tableName = tokens[i + 3];
                                std::string columnName = tokens[i + 5];
                                std::string refTable = tokens[i + 7];
                                std::string refColumn = tokens[i + 9];

                                if (tableName.empty())
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Table name is empty." << std::endl;
                                    exit(1);
                                }

                                if (columnName.empty())
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Column name is empty." << std::endl;
                                    exit(1);
                                }

                                if (refTable.empty())
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Referenced table name is empty." << std::endl;
                                    exit(1);
                                }

                                if (refColumn.empty())
                                {
                                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Referenced column name is empty." << std::endl;
                                    exit(1);
                                }

                                // Get the table
                                Table &table = this->skibiDB->getTable(tableName);

                                // Add the foreign key
                                table.addFK(columnName, refTable, refColumn);

                                std::cout << termcolor::magenta << "[NOTE] " << termcolor::reset;
                                std::cout << termcolor::italic << "It is recommended to save the database after adding a foreign key." << termcolor::reset << std::endl;

                                // xmpl: SET FK table_name column_name ref_table ref_column
                            }
                        }
                        else if (strcmp("DROP", token.c_str()) == 0)
                        {
                            // Make sure it's not part of a bigger command
                            if (strcmp("TABLE", tokens[i + 1].c_str()) == 0)
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
                            continue;
                        }
                    }
                }
                else
                {
                    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid statement." << std::endl;
                }
            }
        }
    } // Catch errs
    catch (const std::exception &e)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << e.what() << std::endl;
        // Restart the console upon err
        this->stop();
        this->start();
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