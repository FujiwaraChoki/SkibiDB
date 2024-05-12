#pragma once

#include <string>
#include <vector>
#include <map>

#include "Attribute.hpp"

class Table
{
public:
    Table(const std::string &name, const std::vector<Attribute> &attributes);
    Table(const std::string &name, const std::vector<Attribute> &attributes, const std::vector<std::map<std::string, std::string>> &data);

    // Get the name of the table
    std::string getTableName() const;

    // Get the attributes of the table
    std::vector<Attribute> getAttributes() const;

    std::vector<std::map<std::string, std::string>> getData() const;

    // Add a new attribute to the table
    void addAttribute(const Attribute &attribute);

    // Remove an attribute from the table
    void removeAttribute(const std::string &name);

    // Get the attribute with the given name
    Attribute getAttribute(const std::string &name) const;

    // Get the attribute with the given index
    Attribute getAttribute(int index) const;

    std::string toString() const;

    // Get the number of rows in the table
    int getNumRows() const;

    // Add a new row to the table
    void addRow(const std::vector<std::string> &attributes, const std::vector<std::string> &values);

    // Get the row at the given index
    std::map<std::string, std::string> getRow(int index) const;

    // Get all rows in the table
    std::vector<std::map<std::string, std::string>> getRows() const;

private:
    int numRows;
    std::string name;
    std::vector<Attribute> attributes;
    std::vector<std::map<std::string, std::string>> data;
};
