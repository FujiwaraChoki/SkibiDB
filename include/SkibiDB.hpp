#pragma once

#include "Table.hpp"

#include <string>
#include <vector>

class SkibiDB
{
public:
    SkibiDB();
    ~SkibiDB();
    void addTable(Table table);
    void removeTable(std::string tableName);
    Table getTable(std::string tableName);
    std::vector<Table> getTables();
    void setTables(std::vector<Table> tables);

private:
    std::vector<Table> tables;
};
