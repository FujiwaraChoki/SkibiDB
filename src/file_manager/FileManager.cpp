#include "FileManager.hpp"
#include "utilities.hpp"
#include "termcolor.hpp"
#include "Table.hpp"

#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstdio>

FileManager::FileManager()
{
    const char *skibiPath = std::getenv("SKIBI_PATH");
    std::string localPath = skibiPath ? skibiPath : "";

    std::cout << termcolor::cyan << "[INFO] " << termcolor::reset << "Loading Skibi database..." << std::endl;

    if (localPath.empty())
    {
        std::cout << termcolor::yellow << "[WARN] " << termcolor::reset << "SKIBI_PATH environment variable not set. Setting default path." << std::endl;

        this->skibiPath = buildSkibiPath();
        _putenv_s("SKIBI_PATH", this->skibiPath.c_str());
    }
    else
    {
        this->skibiPath = localPath;
    }

    this->extensions = {".skb", ".json"};

    this->load();
}

FileManager::~FileManager()
{
    this->save();
}

void FileManager::addFile(std::string file)
{
    this->files.push_back(file);
}

std::vector<std::string> FileManager::listFiles(std::string path)
{
    std::vector<std::string> result;

    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        std::string filename = entry.path().filename().string();

        if (!filename.empty())
        {
            // Check without altering the original string
            std::string ext = filename.substr(filename.find_last_of('.'));
            if (std::find(this->extensions.begin(), this->extensions.end(), ext) != this->extensions.end())
            {
                result.push_back(filename);
            }
        }
    }

    return result;
}

void FileManager::load()
{
    // Load the skibi database
    this->skibiDB = new SkibiDB();

    // List all files in the .skibi directory
    this->files = listFiles(this->skibiPath);

    // Load all .skb files
    for (std::string file : listFiles(this->skibiPath))
    {
        // Load the file
        std::ifstream in(this->skibiPath + "\\" + file);
        std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        // Add to files
        this->addFile(content);

        // Add the table to the SkibiDB
        this->skibiDB->addTable(file.substr(0, file.find(".skb")), file);

        std::cout << termcolor::cyan << "[INFO] " << termcolor::reset << "Loaded file: " << file << std::endl;
    }
}

void FileManager::save()
{
    // Loop through SkibiDB tables
    for (Table table : this->skibiDB->getTables())
    {
        // Save each table to a file
        std::string filename = table.getTableName() + ".skb";
        std::ofstream out(this->skibiPath + "\\" + filename);
        out << table.toString();
        out.close();
    }
}
