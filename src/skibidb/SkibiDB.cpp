#include <stdint.h>

#include "termcolor.hpp"
#include "utilities.hpp"
#include "Attribute.hpp"
#include "SkibiDB.hpp"
#include "Table.hpp"

#include <nlohmann/json.hpp>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using json = nlohmann::json;

// CPP file for SkibiDB class
SkibiDB::SkibiDB()
{
}

SkibiDB::~SkibiDB()
{
}

int16_t SkibiDB::addTable(std::string tableName, std::vector<std::map<std::string, std::string>> attributes)
{
    std::vector<Attribute> finalAttributes;

    // Each attribute is a map (each has a name, value, and type)
    for (const auto &attributeMap : attributes)
    {
        // Make a copy of the map to allow modification
        std::map<std::string, std::string> mutableAttributeMap = attributeMap;

        // Get the attribute name
        std::string name = mutableAttributeMap["name"];

        // Get the attribute type
        std::string type = mutableAttributeMap["type"];

        // Create a new attribute
        Attribute newAttribute;
        newAttribute.setAttributeName(name);
        newAttribute.setAttributeType(type);

        // Add the attribute to the final list
        finalAttributes.push_back(newAttribute);
    }

    // Create a new table
    Table table(tableName, finalAttributes);

    // Add the table to the list of tables
    this->tables.push_back(table);

    std::cout << termcolor::cyan << "[INFO] " << termcolor::reset << "Added table: " << tableName << std::endl;

    return 0;
}

int16_t SkibiDB::addTable(std::string tableName, std::string path)
{
    std::string fullPath = buildSkibiPath() + "\\" + path;
    // Read file contents
    std::string contents;
    std::ifstream file(fullPath);

    // Use getline
    std::string line;
    while (std::getline(file, line))
    {
        contents += line;
    }

    // Parse the JSON file
    json j = json::parse(contents);

    std::vector<Attribute> finalAttributes;

    // Find attributes in the JSON file
    json attributes = j["attributes"];

    json data = j["data"];

    // Each attribute is a JSON object (each has a name and type)
    for (const auto &attribute : attributes)
    {
        // Get the attribute name
        std::string name = attribute["name"];

        // Get the attribute type
        std::string type = attribute["type"];

        // Create a new attribute
        Attribute newAttribute;
        newAttribute.setAttributeName(name);
        newAttribute.setAttributeType(type);

        // Add the attribute to the final list
        finalAttributes.push_back(newAttribute);
    }

    // Create a new table
    Table table(tableName, finalAttributes, data);

    // Add the table to the list of tables
    this->tables.push_back(table);

    return 0;
}

int16_t SkibiDB::removeTable(std::string name)
{
    // Find the table with the given name
    auto it = std::find_if(this->tables.begin(), this->tables.end(), [name](const Table &table)
                           { return table.getTableName() == name; });

    // If the table is found, remove it
    if (it != this->tables.end())
    {
        this->tables.erase(it);
        std::cout << termcolor::cyan << "[INFO] " << termcolor::reset << "Removed table: " << name << std::endl;

        return 0;
    }
    else
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Table not found: " << name << std::endl;

        return 1;
    }
}

Table &SkibiDB::getTable(std::string name)
{
    // Find the table with the given name
    auto it = std::find_if(this->tables.begin(), this->tables.end(), [name](const Table &table)
                           { return table.getTableName() == name; });

    // If the table is found, return it
    if (it != this->tables.end())
    {
        return *it;
    }
    else
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Table not found: " << name << std::endl;
        exit(1);
    }
}
