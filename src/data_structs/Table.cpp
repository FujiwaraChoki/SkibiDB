#include "Table.hpp"

#include <nlohmann/json.hpp>
#include <stdexcept>

Table::Table(const std::string &name, const std::vector<Attribute> &attributes)
{
    this->name = name;
    this->attributes = attributes;
}

std::string Table::getTableName() const
{
    return name;
}

std::vector<Attribute> Table::getAttributes() const
{
    return attributes;
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
    throw std::runtime_error("Attribute not found");
}

Attribute Table::getAttribute(int index) const
{
    if (index < 0 || index >= attributes.size())
    {
        throw std::runtime_error("Invalid index");
    }
    return attributes[index];
}

std::string Table::toString() const
{
    nlohmann::json j;
    j["name"] = name;
    j["attributes"] = nlohmann::json::array();
    for (const auto &attribute : attributes)
    {
        nlohmann::json attr;
        attr["name"] = attribute.getAttributeName();
        attr["type"] = attribute.getAttributeType();
        j["attributes"].push_back(attr);
    }
    return j.dump(4);
}
