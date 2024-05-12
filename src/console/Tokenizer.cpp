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
    // Treat as one token if starts with (, and ends with )
    if (str[pos] == '(')
    {
        size_t end = str.find_first_of(")", pos);
        std::string token = str.substr(pos, end - pos + 1);
        pos = end + 1;
        return token;
    }

    // Finds the next token
    size_t start = str.find_first_not_of(" \t\n\r\f\v", pos);
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
