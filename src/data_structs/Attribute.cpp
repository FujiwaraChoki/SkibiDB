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

std::string Attribute::getAttributeType() const
{
    return type;
}

void Attribute::setAttributeType(const std::string &type)
{
    this->type = type;
}
