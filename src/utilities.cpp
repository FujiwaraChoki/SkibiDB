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
#include <cctype>
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

std::string soundex(const std::string &s)
{
    // Map letters to their corresponding Soundex digits
    std::unordered_map<char, char> soundexMap{
        {'b', '1'}, {'f', '1'}, {'p', '1'}, {'v', '1'}, {'c', '2'}, {'g', '2'}, {'j', '2'}, {'k', '2'}, {'q', '2'}, {'s', '2'}, {'x', '2'}, {'z', '2'}, {'d', '3'}, {'t', '3'}, {'l', '4'}, {'m', '5'}, {'n', '5'}, {'r', '6'}, {'p', '7'}, {'h', '7'}};

    // Convert input string to uppercase
    std::string uppercased;
    for (char c : s)
    {
        uppercased += std::toupper(c);
    }

    // Initialize Soundex code with the first letter of the input string
    std::string soundexCode = uppercased.substr(0, 1);

    // Process the rest of the characters in the input string
    for (size_t i = 1; i < uppercased.length() && soundexCode.length() < 4; ++i)
    {
        char digit = soundexMap[uppercased[i]];
        // If the digit is not the same as the last digit in the Soundex code and not '0'
        if (digit != soundexCode.back() && digit != '0')
        {
            soundexCode += digit;
        }
    }

    // Pad with zeros if the code is less than 4 characters long
    soundexCode.resize(4, '0');

    return soundexCode;
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
