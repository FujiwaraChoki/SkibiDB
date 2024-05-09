#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>

class Tokenizer
{
public:
    Tokenizer();
    Tokenizer(const std::string &str);

    std::vector<std::string> tokenize();
    std::vector<std::string> tokenize(const std::string &str);

    bool hasMoreTokens() const;
    std::string nextToken();

private:
    std::string str;
    size_t pos;
};

#endif // TOKENIZER_HPP
