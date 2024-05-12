#pragma once

#include <string>
#include <variant>
#include <stdexcept>

class Attribute
{
public:
    Attribute();
    ~Attribute();

    std::string getAttributeName() const;
    void setAttributeName(const std::string &name);

    std::string getAttributeType() const;
    void setAttributeType(const std::string &type);

private:
    std::string name;
    std::string type;
};
