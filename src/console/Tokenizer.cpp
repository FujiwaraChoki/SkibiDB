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
    // Find the next space starting from current position
    size_t nextSpace = str.find(' ', pos);

    // Check if no space is found from the current position onwards
    if (nextSpace == std::string::npos)
    {
        // Get the token from current position to the end of the string
        std::string token = str.substr(pos);
        // Update the position to the end of the string
        pos = str.size();
        // Return the token
        return token;
    }

    // Get the token
    std::string token = str.substr(pos, nextSpace - pos);

    // Update the position
    pos = nextSpace + 1;

    // Return the token
    return token;
}

bool Tokenizer::hasMoreTokens() const
{
    // Check if the position is less than the string length
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
