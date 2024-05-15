#include "utilities.hpp"

#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <Lmcons.h>
#include <iomanip>
#include <string>
#include <random>
#include <ctime>
#include <regex>

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

std::string soundex(const std::string &name)
{
    if (name.empty())
    {
        return "";
    }

    std::unordered_map<char, char> replacements = {
        {'B', '1'}, {'F', '1'}, {'P', '1'}, {'V', '1'}, {'C', '2'}, {'G', '2'}, {'J', '2'}, {'K', '2'}, {'Q', '2'}, {'S', '2'}, {'X', '2'}, {'Z', '2'}, {'D', '3'}, {'T', '3'}, {'L', '4'}, {'M', '5'}, {'N', '5'}, {'R', '6'}};

    std::string upper_name = toUpperCase(name);

    std::string result(1, upper_name[0]);
    int count = 1;

    char last = replacements.count(upper_name[0]) ? replacements[upper_name[0]] : '\0';

    for (size_t i = 1; i < upper_name.length() && count < 4; ++i)
    {
        char letter = upper_name[i];
        if (replacements.count(letter))
        {
            char sub = replacements[letter];
            if (sub != last)
            {
                result += sub;
                count++;
            }
            last = sub;
        }
        else
        {
            if (letter != 'H' && letter != 'W')
            {
                last = '\0';
            }
        }
    }

    while (result.length() < 4)
    {
        result += '0';
    }

    return result;
}

bool isNumber(const std::string &s)
{
    std::regex integerRegex("^[-+]?[0-9]+$");
    std::regex doubleRegex("^[-+]?[0-9]*\\.?[0-9]+$");

    if (std::regex_match(s, integerRegex) || std::regex_match(s, doubleRegex))
    {
        return true;
    }

    return false;
}

bool isNumber(const char &c)
{
    return c >= '0' && c <= '9';
}
