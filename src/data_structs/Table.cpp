#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Table.hpp"
#include "Attribute.hpp"
#include "termcolor.hpp"
#include "utilities.hpp"

Table::Table(const std::string &name, const std::vector<Attribute> &attributes)
{
    this->name = name;
    this->attributes = attributes;
    this->numRows = 0;
    this->pk = "__id__";
    this->fks = std::map<std::string, std::pair<std::string, std::string>>();
}

Table::Table(const std::string &name, const std::vector<Attribute> &attributes, const std::vector<std::map<std::string, std::string>> &data)
{
    this->name = name;
    this->attributes = attributes;
    this->numRows = data.size() > 0 ? data.size() : 0;
    this->data = data;
    this->pk = "__id__";
    this->fks = std::map<std::string, std::pair<std::string, std::string>>();
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

void Table::dropColumn(const std::string &name)
{
    // Remove the column from the list of attributes
    for (auto it = attributes.begin(); it != attributes.end(); ++it)
    {
        if (it->getAttributeName() == name)
        {
            attributes.erase(it);
            break;
        }
    }

    // Remove the column from each row
    for (auto &row : data)
    {
        row.erase(name);
    }
    // Remove the column from the primary key
    if (pk == name)
    {
        pk = "";
    }
    // Remove the column from the foreign keys
    for (auto it = fks.begin(); it != fks.end(); ++it)
    {
        if (it->first == name)
        {
            fks.erase(it);
            break;
        }
    }
}

void Table::addColumn(const Attribute &attribute)
{
    // Add the attribute to the list of attributes
    attributes.push_back(attribute);
}

void Table::updateRow(const std::vector<std::string> &setTokens, const std::vector<std::string> &conditionTokens)
{
    // Update a data row if it fulfills certain conditions
    if (conditionTokens.size() < 3)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid condition format" << std::endl;
        return;
    }

    // Extract condition components
    const std::string &attribute = conditionTokens[0];
    const std::string &op = conditionTokens[1];
    std::string value = conditionTokens[2]; // Make a copy of the value string

    // Remove double quotes from value
    if (value.front() == '"')
    {
        value.erase(0, 1);
    }

    if (value.back() == '"')
    {
        value.pop_back();
    }

    // Validate operator
    bool validOperator = (op == "==" || op == "!=" || op == ">" || op == "<" || op == ">=" || op == "<=" || op == "LIKE" || op == "IS" || op == "NOT" || op == "GT" || op == "LT" || op == "GE" || op == "LE" || op == "~");
    if (!validOperator)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid operator: " << op << std::endl;
        return;
    }

    // Perform search
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        bool match = false;

        // Check if attribute exists in row
        if (it->find(attribute) != it->end())
        {
            const std::string &rowValue = it->at(attribute);

            // Perform comparison based on operator
            if (op == "==" || op == "=" || op == "IS")
            {
                match = (strcmp(rowValue.c_str(), value.c_str()) == 0);
            }
            else if (op == "!=" || op == "NOT")
            {
                match = (rowValue != value);
            }
            else if (op == ">" || op == "GT")
            {
                match = (std::stoi(rowValue) > std::stoi(value));
            }
            else if (op == "<" || op == "LT")
            {
                match = (std::stoi(rowValue) < std::stoi(value));
            }
            else if (op == ">=" || op == "GE")
            {
                match = (std::stoi(rowValue) >= std::stoi(value));
            }
            else if (op == "<=" || op == "LE")
            {
                match = (std::stoi(rowValue) <= std::stoi(value));
            }
            else if (op == "~" || op == "LIKE")
            {
                // If starts with s/, do soundex search
                if (value.find("s/") == 0)
                {
                    std::string rowSx = soundex(rowValue);
                    std::string rvSx = soundex(value.substr(2));

                    // Check if soundex value matches exactly
                    match = (rvSx == rowSx);
                }
                else
                {
                    match = (toLowerCase(rowValue).find(toLowerCase(value)) != std::string::npos);
                }
            }

            // Update row if it matches the condition
            if (match)
            {
                std::string columnName = setTokens[0];
                std::string newValue = setTokens[2];

                // Remove double quotes from value
                if (newValue.front() == '"')
                {
                    newValue.erase(0, 1);
                }

                if (newValue.back() == '"')
                {
                    newValue.pop_back();
                }

                // Check if value starts with double: int: or nothing (string)
                if (newValue.find("int:") == 0)
                {
                    std::string value = newValue;

                    // If column exists, use at, if not, use insert
                    if (it->find(columnName) == it->end())
                    {
                        it->insert({columnName, value.erase(0, 4)});
                    }
                    else
                    {
                        it->at(columnName) = value.erase(0, 4);
                    }
                }
                else if (newValue.find("double:") == 0)
                {
                    std::string value = newValue;

                    // If column exists, use at, if not, use insert
                    if (it->find(columnName) == it->end())
                    {
                        it->insert({columnName, value.erase(0, 7)});
                    }
                    else
                    {
                        it->at(columnName) = value.erase(0, 7);
                    }
                }
                else
                {
                    // If column exists, use at, if not, use insert
                    if (it->find(columnName) == it->end())
                    {
                        it->insert({columnName, newValue});
                    }
                    else
                    {
                        it->at(columnName) = newValue;
                    }
                }

                break;
            }
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
    j["pk"] = pk;
    j["fks"] = nlohmann::json::array();
    for (const auto &attribute : attributes)
    {
        nlohmann::json attr;
        attr["name"] = attribute.getAttributeName();
        attr["type"] = attribute.getAttributeType();
        j["attributes"].push_back(attr);
    }

    // Add data
    for (const auto &row : this->data)
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
    // Print all values
    for (const auto &value : values)
    {
        std::cerr << value << std::endl;
    }
    // Check length of vectors, if they are not equal, return
    if (attributes.size() != values.size())
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Attributes and values do not match" << std::endl;
        return;
    }

    std::map<std::string, std::string> row;
    for (size_t i = 0; i < attributes.size(); i++)
    {
        // Check if value starts with double: int: or nothing (string)
        if (values[i].find("int:") == 0)
        {
            std::string value = values[i];
            row[attributes[i]] = value.erase(0, 4);
        }
        else if (values[i].find("double:") == 0)
        {
            std::string value = values[i];
            row[attributes[i]] = value.erase(0, 7);
        }
        else
        {
            row[attributes[i]] = values[i];
        }
        row["__id__"] = generateUUID();
        row["__row__"] = std::to_string(this->numRows + 1);
        row["__created_at__"] = getCurrentTimestamp();
    }

    this->data.push_back(row);
    this->numRows++;
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

std::vector<std::map<std::string, std::string>> Table::select(const std::vector<std::string> &columns) const
{
    std::vector<std::map<std::string, std::string>> result;

    // Select all columns if "*" is specified
    if (columns.size() == 1 && columns[0] == "*")
    {
        return data;
    }

    // NOTE: Nessecary?
    std::vector<std::string> attrs;
    for (const auto &attr : this->attributes)
    {
        std::string attr2 = attr.getAttributeName();
        // Remove all commas
        attr2.erase(std::remove(attr2.begin(), attr2.end(), ','), attr2.end());
        attrs.push_back(attr2);
    }

    // Check if all columns exist
    for (const auto &column : columns)
    {
        if (column != "*" && std::find_if(attrs.begin(), attrs.end(),
                                          [&](const auto &attr)
                                          { return attr == column; }) == attrs.end())
        {
            std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Column not found: " << column << std::endl;
            return result;
        }
    }

    // Extract specified columns
    for (const auto &row : data)
    {
        std::map<std::string, std::string> newRow;
        for (const auto &column : columns)
        {
            if (column == "*")
            {
                newRow = row;
                break;
            }
            newRow[column] = row.at(column);
        }
        result.push_back(newRow);
    }

    return result;
}

std::vector<std::map<std::string, std::string>> Table::select(const std::vector<std::string> &columns, const std::vector<std::string> &conditionTokens) const
{
    std::vector<std::map<std::string, std::string>> result;

    // Select all columns if "*" is specified
    if (columns.size() == 1 && columns[0] == "*" && conditionTokens.empty())
    {
        return select(columns);
    }

    // Ensure conditionTokens has at least three elements
    if (conditionTokens.size() < 3)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid condition format" << std::endl;
        return result;
    }

    // Extract condition components
    const std::string &attribute = conditionTokens[0];
    const std::string &op = conditionTokens[1];
    const std::string &valueConst = conditionTokens[2];

    std::string value = valueConst;

    // Remove double quotes from value
    if (value.front() == '"')
    {
        value.erase(0, 1);
    }

    if (value.back() == '"')
    {
        value.pop_back();
    }

    // Validate operator
    bool validOperator = (op == "==" || op == "!=" || op == ">" || op == "<" || op == ">=" || op == "<=" || op == "LIKE" || op == "IS" || op == "NOT" || op == "GT" || op == "LT" || op == "GE" || op == "LE" || op == "~");
    if (!validOperator)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid operator: " << op << std::endl;
        return result;
    }

    // Perform search
    for (const auto &row : data)
    {
        bool match = false;

        // Check if attribute exists in row
        if (row.find(attribute) != row.end())
        {
            const std::string &rowValue = row.at(attribute);

            // Perform comparison based on operator
            if (op == "==" || op == "=" || op == "IS")
            {
                match = (strcmp(rowValue.c_str(), value.c_str()) == 0);
            }
            else if (op == "!=" || op == "NOT")
            {
                match = (rowValue != value);
            }
            else if (op == ">" || op == "GT")
            {
                match = (std::stoi(rowValue) > std::stoi(value));
            }
            else if (op == "<" || op == "LT")
            {
                match = (std::stoi(rowValue) < std::stoi(value));
            }
            else if (op == ">=" || op == "GE")
            {
                match = (std::stoi(rowValue) >= std::stoi(value));
            }
            else if (op == "<=" || op == "LE")
            {
                match = (std::stoi(rowValue) <= std::stoi(value));
            }
            else if (op == "~" || op == "LIKE")
            {
                // If starts with s/, do soundex search
                if (value.find("s/") == 0)
                {
                    std::string rowSx = soundex(rowValue);
                    std::string rvSx = soundex(value.substr(2));

                    // Check if soundex value matches exactly
                    match = (rvSx == rowSx);
                }
                else
                {
                    match = (toLowerCase(rowValue).find(toLowerCase(value)) != std::string::npos);
                }
            }
        }

        // Add row to result if it matches the condition
        if (match)
        {
            std::map<std::string, std::string> selectedRow;
            for (const auto &col : columns)
            {
                if (columns[0] == "*")
                {
                    selectedRow = row;
                    break;
                }
                else
                {
                    // If the column exists in the row, add it to the selected row
                    if (row.find(col) != row.end())
                    {
                        selectedRow[col] = row.at(col);
                    }
                }
            }
            result.push_back(selectedRow);
        }
    }

    return result;
}

bool Table::deleteRow(const std::vector<std::string> &conditionTokens)
{
    // Removes a data row if it fulfills certain conditions
    if (conditionTokens.size() < 3)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid condition format" << std::endl;
        return false;
    }

    // Extract condition components
    const std::string &attribute = conditionTokens[0];
    const std::string &op = conditionTokens[1];
    std::string value = conditionTokens[2];

    // Remove double quotes from value
    if (value.front() == '"')
    {
        value.erase(0, 1);
    }

    if (value.back() == '"')
    {
        value.pop_back();
    }

    // Validate operator
    bool validOperator = (op == "==" || op == "!=" || op == ">" || op == "<" || op == ">=" || op == "<=" || op == "LIKE" || op == "IS" || op == "NOT" || op == "GT" || op == "LT" || op == "GE" || op == "LE" || op == "~");
    if (!validOperator)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid operator: " << op << std::endl;
        return false;
    }

    // Perform search
    for (auto it = data.begin(); it != data.end();)
    {
        bool match = false;

        // Check if attribute exists in row
        if (it->find(attribute) != it->end())
        {
            const std::string &rowValue = it->at(attribute);

            // Perform comparison based on operator
            if (op == "==" || op == "=" || op == "IS")
            {
                match = (strcmp(rowValue.c_str(), value.c_str()) == 0);
            }
            else if (op == "!=" || op == "NOT")
            {
                match = (rowValue != value);
            }
            else if (op == ">" || op == "GT")
            {
                match = (std::stoi(rowValue) > std::stoi(value));
            }
            else if (op == "<" || op == "LT")
            {
                match = (std::stoi(rowValue) < std::stoi(value));
            }
            else if (op == ">=" || op == "GE")
            {
                match = (std::stoi(rowValue) >= std::stoi(value));
            }
            else if (op == "<=" || op == "LE")
            {
                match = (std::stoi(rowValue) <= std::stoi(value));
            }
            else if (op == "~" || op == "LIKE")
            {
                // If starts with s/, do soundex search
                if (value.find("s/") == 0)
                {
                    std::string rowSx = soundex(rowValue);
                    std::string rvSx = soundex(value.substr(2));

                    // Check if soundex value matches exactly
                    match = (rvSx == rowSx);
                }
                else
                {
                    match = (toLowerCase(rowValue).find(toLowerCase(value)) != std::string::npos);
                }
            }
        }

        // Remove row if it matches the condition
        if (match)
        {
            it = data.erase(it);
            numRows--;

            return true;
        }
        else
        {
            ++it;
        }
    }

    return false;
}

void Table::setPK(const std::string &name)
{
    // Set the primary key of the table
    this->pk = name;
}

void Table::addFK(const std::string &name, const std::string &refTable, const std::string &refColumn)
{
    // Add a foreign key to the table
    // Format: FK_name_refTable_refColumn
    std::string fk = "FK_" + name + "_" + refTable + "_" + refColumn;
    Attribute attr;
    attr.setAttributeName(fk);
    attr.setAttributeType("FOREIGN_KEY");
    addAttribute(attr);
}

std::string Table::getPK() const
{
    return pk;
}

std::vector<std::string> Table::getFKs() const
{
    std::vector<std::string> fks;
    for (const auto &attribute : attributes)
    {
        if (attribute.getAttributeType() == "FOREIGN KEY")
        {
            fks.push_back(attribute.getAttributeName());
        }
    }
    return fks;
}

bool Table::hasPK() const
{
    return !pk.empty();
}

bool Table::hasFK(const std::string &name) const
{
    for (const auto &attribute : attributes)
    {
        if (attribute.getAttributeName() == name && attribute.getAttributeType() == "FOREIGN KEY")
        {
            return true;
        }
    }
    return false;
}
