#include <vector>

std::vector<std::string> getSyntax()
{
    // Create a vector of strings
    std::vector<std::string> syntax;

    // Add the syntax to the vector (SQL syntax)
    syntax.push_back("SELECT [column] FROM [table] WHERE [condition]");
    syntax.push_back("INSERT INTO [table] ([column], [column], ...) VALUES ([value], [value], ...);");
    syntax.push_back("DELETE FROM [table] WHERE [condition]");
    syntax.push_back("CREATE TABLE [table] ([column] [type], [column] [type], ...)");
    syntax.push_back("ALTER TABLE [table] ADD [column] [type]");
    syntax.push_back("DROP TABLE [table]");
    syntax.push_back("CREATE INDEX [index] ON [table] ([column])");

    // Return the syntax
    return syntax;
}

std::string cleanSyntax(std::string syntax)
{
    // Remove EVERYTHING between the brackets
    while (syntax.find("[") != std::string::npos)
    {
        // Find the first bracket
        size_t start = syntax.find("[");

        // Find the second bracket
        size_t end = syntax.find("]");

        // Erase the content between the brackets
        syntax.erase(start, end - start + 1);
    }

    // Return the cleaned syntax
    return syntax;
}
