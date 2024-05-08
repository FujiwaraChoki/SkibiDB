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

template <typename T>
T Attribute::getAttributeValue() const
{
    if (std::holds_alternative<T>(value))
    {
        return std::get<T>(value);
    }
    else
    {
        throw std::runtime_error("Invalid attribute type");
    }
}

template <typename T>
void Attribute::setAttributeValue(const T &value)
{
    this->value = value;
}
