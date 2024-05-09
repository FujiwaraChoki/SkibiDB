#include "utilities.hpp"

#include <windows.h>
#include <Lmcons.h>

std::string toUpperCase(std::string str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string buildSkibiPath()
{
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName(username, &username_len);

    // Build .skibi path in home directory
    std::string skibiPath = "C:\\Users\\" + std::string(username) + "\\.skibi";

    return skibiPath;
}