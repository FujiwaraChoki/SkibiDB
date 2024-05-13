#include "utilities.hpp"

#include <filesystem>
#include <algorithm>
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <Lmcons.h>
#include <iomanip>
#include <random>
#include <ctime>

std::string generateUUID()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::stringstream ss;
    for (int i = 0; i < 4; ++i)
    {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

std::string toUpperCase(std::string str)
{
    std::string result = str.c_str();
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string toLowerCase(std::string str)
{
    std::string result = str.c_str();
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string join(const std::vector<std::string> &elements, const std::string &separator)
{
    if (elements.empty())
    {
        return "";
    }

    std::string result = elements[0];
    for (size_t i = 1; i < elements.size(); ++i)
    {
        result += separator + elements[i];
    }

    return result;
}

std::string buildSkibiPath()
{
    // Declare username
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameA(username, &username_len);

    // Build .skibi path in home directory
    std::string skibiPath = "C:\\Users\\" + std::string(username) + "\\.skibi";

    // If doesn't exist, create the directory
    if (!std::filesystem::exists(skibiPath))
    {
        std::filesystem::create_directory(skibiPath);
    }

    // Set env
    _putenv_s("SKIBI_PATH", skibiPath.c_str());

    return skibiPath;
}

std::string getCurrentTimestamp()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    std::string timestamp = std::to_string(1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);

    return timestamp;
}
