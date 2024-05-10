#include "Attribute.hpp"

Attribute::Attribute() {}

Attribute::~Attribute() {}

std::string Attribute::getAttributeName() const
{
    return name;
}

void Attribute::setAttributeName(const std::string &name)
{
    this->name = name;
}

std::variant<int, double, std::string> Attribute::getAttributeValue() const
{
    return value;
}

void Attribute::setAttributeValue(const std::variant<int, double, std::string> &value)
{
    this->value = value;
}

std::string Attribute::getAttributeType() const
{
    return type;
}

void Attribute::setAttributeType(const std::string &type)
{
    this->type = type;
}
