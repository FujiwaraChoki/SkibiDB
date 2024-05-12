#pragma once

#include "Table.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <map>

class SkibiDB
{
public:
    SkibiDB();
    ~SkibiDB();

    int16_t addTable(std::string name, std::vector<std::map<std::string, std::string>> attributes);
    int16_t addTable(std::string name, std::string path);
    int16_t removeTable(std::string tableName);

    Table &getTable(std::string name);

    std::vector<Table> getTables() const { return this->tables; }
    void setTables(const std::vector<Table> &tables) { this->tables = tables; }

private:
    std::vector<Table> tables;
};
