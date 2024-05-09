#include <vector>

std::vector<std::string> getSyntaxKeywords()
{
    // Create a vector of strings
    std::vector<std::string> keywords;

    // Add the keywords to the vector (SQL keywords)
    keywords.push_back("SELECT");
    keywords.push_back("FROM");
    keywords.push_back("WHERE");
    keywords.push_back("INSERT");
    keywords.push_back("INTO");
    keywords.push_back("VALUES");
    keywords.push_back("UPDATE");
    keywords.push_back("SET");
    keywords.push_back("DELETE");
    keywords.push_back("CREATE");
    keywords.push_back("TABLE");
    keywords.push_back("ALTER");
    keywords.push_back("DROP");
    keywords.push_back("INDEX");
    keywords.push_back("PRIMARY");
    keywords.push_back("KEY");
    keywords.push_back("FOREIGN");
    keywords.push_back("JOIN");
    keywords.push_back("INNER");
    keywords.push_back("LEFT");
    keywords.push_back("RIGHT");
    keywords.push_back("OUTER");
    keywords.push_back("UNION");
    keywords.push_back("*");
    keywords.push_back("AND");
    keywords.push_back("OR");
    keywords.push_back("NOT");
    keywords.push_back("IN");
    keywords.push_back("LIKE");
    keywords.push_back("IS");

    // Return the vector
    return keywords;
}
