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

    std::variant<int, double, std::string> getAttributeValue() const;
    void setAttributeValue(const std::variant<int, double, std::string> &value);

    std::string getAttributeType() const;
    void setAttributeType(const std::string &type);

private:
    std::string name;

    std::variant<int, double, std::string> value;

    std::string type;
};
