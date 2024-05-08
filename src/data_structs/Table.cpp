#include "Table.hpp"

class Table
{
public:
    Table(const std::string &name, const std::vector<Attribute> &attributes)
    {
        this->name = name;
        this->attributes = attributes;
    }

    std::string getTableName() const
    {
        return name;
    }

    std::vector<Attribute> getAttributes() const
    {
        return attributes;
    }

    void addAttribute(const Attribute &attribute)
    {
        attributes.push_back(attribute);
    }

    void removeAttribute(const std::string &name)
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

    Attribute getAttribute(const std::string &name) const
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

    Attribute getAttribute(int index) const
    {
        if (index < 0 || index >= attributes.size())
        {
            throw std::runtime_error("Invalid index");
        }
        return attributes[index];
    }

private:
    std::string name;
    std::vector<Attribute> attributes;
};
