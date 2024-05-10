#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include "SkibiDB.hpp"

#include <nlohmann/json.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

class FileManager
{
public:
    FileManager();
    ~FileManager();
    void load();
    void save();
    std::vector<std::string> listFiles(std::string skibiPath);

    SkibiDB getSkibiDB() { return skibiDB; }
    void setSkibiDB(SkibiDB skibiDB) { this->skibiDB = skibiDB; }
    std::string getSkibiPath();

    void addFile(std::string file);

private:
    std::string skibiPath;
    SkibiDB skibiDB;
    std::vector<std::string>
        files;
    std::vector<std::string> extensions;
};

#endif // FILE_MANAGER_HPP
