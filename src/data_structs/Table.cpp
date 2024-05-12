#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Table.hpp"
#include "Attribute.hpp"
#include "termcolor.hpp"

Table::Table(const std::string &name, const std::vector<Attribute> &attributes)
{
    this->name = name;
    this->attributes = attributes;
    this->numRows = 0;
}

Table::Table(const std::string &name, const std::vector<Attribute> &attributes, const std::vector<std::map<std::string, std::string>> &data)
{
    this->name = name;
    this->attributes = attributes;
    this->numRows = data.size() > 0 ? data.size() : 0;
    this->data = data;
}

std::string Table::getTableName() const
{
    return name;
}

std::vector<Attribute> Table::getAttributes() const
{
    return attributes;
}

int Table::getNumRows() const
{
    return numRows;
}

std::vector<std::map<std::string, std::string>> Table::getData() const
{
    return data;
}

void Table::addAttribute(const Attribute &attribute)
{
    attributes.push_back(attribute);
}

void Table::removeAttribute(const std::string &name)
{
    for (auto it = attributes.begin(); it != attributes.end(); ++it)
    {
        if (it->getAttributeName() == name)
        {
            attributes.erase(it);
            return;
        }
    }
}

Attribute Table::getAttribute(const std::string &name) const
{
    for (const auto &attribute : attributes)
    {
        if (attribute.getAttributeName() == name)
        {
            return attribute;
        }
    }
    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Attribute not found" << std::endl;
    exit(1);
}

Attribute Table::getAttribute(int index) const
{
    if (index < 0 || index >= attributes.size())
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid index" << std::endl;
    }
    return attributes[index];
}

std::string Table::toString() const
{
    nlohmann::json j;
    j["name"] = name;
    j["attributes"] = nlohmann::json::array();
    j["numRows"] = numRows;
    j["data"] = nlohmann::json::array();
    for (const auto &attribute : attributes)
    {
        nlohmann::json attr;
        attr["name"] = attribute.getAttributeName();
        attr["type"] = attribute.getAttributeType();
        j["attributes"].push_back(attr);
    }

    // Add data
    for (const auto &row : this->getRows())
    {
        nlohmann::json r;
        for (const auto &pair : row)
        {
            r[pair.first] = pair.second;
        }
        j["data"].push_back(r);
    }

    return j.dump(4);
}

void Table::addRow(const std::vector<std::string> &attributes, const std::vector<std::string> &values)
{
    // Check length of vectors, if they are not equal, return
    if (attributes.size() != values.size())
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Attributes and values do not match" << std::endl;
        return;
    }

    std::map<std::string, std::string> row;
    for (size_t i = 0; i < attributes.size(); i++)
    {
        row[attributes[i]] = values[i];
    }

    data.push_back(row);
    numRows++;
}

std::map<std::string, std::string> Table::getRow(int index) const
{
    if (index < 0 || index >= numRows)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid index" << std::endl;
        // Return an empty map or throw an exception
        return std::map<std::string, std::string>(); // Empty map
    }
    return data[index];
}

std::vector<std::map<std::string, std::string>> Table::getRows() const
{
    return data;
}
