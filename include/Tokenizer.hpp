#pragma once

#include <string>
#include <vector>

class Tokenizer
{
public:
    Tokenizer();
    Tokenizer(const std::string &str);
    std::string nextToken();
    bool hasMoreTokens() const;

    std::vector<std::string> tokenize(const std::string &str);

private:
    std::string str;
    size_t pos;
    std::vector<std::string> tokens;
};
