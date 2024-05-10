#include "utilities.hpp"
#include <filesystem>
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <Lmcons.h>

std::string toUpperCase(std::string str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
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
    SetEnvironmentVariableA("SKIBI_PATH", skibiPath.c_str());

    return skibiPath;
}
