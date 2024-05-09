#pragma once

#include <string>
#include <vector>

#include "Attribute.hpp"

class Table
{
public:
    Table(const std::string &name, const std::vector<Attribute> &attributes);

    // Get the name of the table
    std::string getTableName() const;

    // Get the attributes of the table
    std::vector<Attribute> getAttributes() const;

    // Add a new attribute to the table
    void addAttribute(const Attribute &attribute);

    // Remove an attribute from the table
    void removeAttribute(const std::string &name);

    // Get the attribute with the given name
    Attribute getAttribute(const std::string &name) const;

    // Get the attribute with the given index
    Attribute getAttribute(int index) const;

    std::string toString() const;

private:
    std::string name;
    std::vector<Attribute> attributes;
};
