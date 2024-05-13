#include "Tokenizer.hpp"

Tokenizer::Tokenizer()
    : pos(0)
{
}

Tokenizer::Tokenizer(const std::string &str)
    : str(str), pos(0)
{
}

std::string Tokenizer::nextToken()
{
    // Finds the next token
    size_t start = str.find_first_not_of(" \t\n\r\f\v", pos);

    // Check for quotes
    if (str[start] == '"')
    {
        size_t end = start + 1;
        while (end < str.size() && (str[end] != '"' || str[end - 1] == '\\'))
        {
            end++;
        }
        std::string token = str.substr(start, end - start);
        pos = end + 1;
        return token;
    }

    // Check for parentheses
    if (str[start] == '(')
    {
        size_t end = str.find_first_of(")", start + 1);
        std::string token = str.substr(start, end - start);
        pos = end + 1;
        return token;
    }

    // Find the end of the token
    size_t end = str.find_first_of(" \t\n\r\f\v", start);

    // If the end is not found, set it to the end of the string
    if (end == std::string::npos)
    {
        end = str.size();
    }

    // Get the token
    std::string token = str.substr(start, end - start);

    // Update the position
    pos = end;

    // Return the token
    return token;
}

bool Tokenizer::hasMoreTokens() const
{
    // Check if there are more tokens
    return pos < str.size();
}

std::vector<std::string> Tokenizer::tokenize()
{
    std::vector<std::string> tokens;

    // Reset the position
    pos = 0;

    // Loop until there are no more tokens
    while (hasMoreTokens())
    {
        // Get the next token
        std::string token = nextToken();

        // Add the token to the list
        tokens.push_back(token);
    }

    // Return the tokens
    return tokens;
}

std::vector<std::string> Tokenizer::tokenize(const std::string &str)
{
    // Create a tokenizer with the given string
    Tokenizer tokenizer(str);

    // Tokenize the string
    return tokenizer.tokenize();
}
