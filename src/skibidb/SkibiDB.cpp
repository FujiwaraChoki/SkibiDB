#include <stdint.h>

#include "termcolor.hpp"
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

using json = nlohmann::json;

// CPP file for SkibiDB class
SkibiDB::SkibiDB()
{
}

SkibiDB::~SkibiDB()
{
}

int16_t SkibiDB::addTable(std::string name, std::string path)
{
    // Parse the JSON file
    json j = json::parse(path);

    std::vector<Attribute> finalAttributes;

    // Find attributes in the JSON file
    json attributes = j["attributes"];

    // Each attribute is a JSON object (each has a name, value and type)
    for (json::iterator it = attributes.begin(); it != attributes.end(); ++it)
    {
        // Get the attribute name
        std::string name = it.key();

        // Get the attribute value
        json value = it.value();

        // Get the attribute type
        std::string type = value["type"];

        // Get the attribute value
        std::string valueStr = value["value"];

        // Create a new attribute
        Attribute attribute;
        attribute.setAttributeName(name);
        attribute.setAttributeType(type);

        // Set the attribute value based on the type
        if (type == "int")
        {
            attribute.setAttributeValue(std::stoi(valueStr));
        }
        else if (type == "double")
        {
            attribute.setAttributeValue(std::stod(valueStr));
        }
        else if (type == "string")
        {
            attribute.setAttributeValue(valueStr);
        }
        else if (type == "boolean")
        {
            attribute.setAttributeValue(valueStr == "true" ? 0 : 1);
        }
        else
        {
            std::cout << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid attribute type: " << type << std::endl;

            return 1; // error out
        }

        // Add the attribute to the final list
        finalAttributes.push_back(attribute);
    }

    // Create a new table
    Table table(name, finalAttributes);

    // Add the table to the list of tables
    this->tables.push_back(table);

    std::cout << termcolor::green << "[INFO] " << termcolor::reset << "Added table: " << name << std::endl;

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
        std::cout << termcolor::green << "[INFO] " << termcolor::reset << "Removed table: " << name << std::endl;

        return 0;
    }
    else
    {
        std::cout << termcolor::red << "[ERROR] " << termcolor::reset << "Table not found: " << name << std::endl;

        return 1;
    }
}

Table SkibiDB::getTable(std::string name) const
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
        throw std::runtime_error("Table not found");
    }
}
