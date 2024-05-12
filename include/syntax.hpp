#include <vector>

std::vector<std::string> getSyntax()
{
    // Create a vector of strings
    std::vector<std::string> syntax;

    // Add the syntax to the vector (SQL syntax)
    syntax.push_back("SELECT [column] FROM [table] WHERE [condition]");
    syntax.push_back("INSERT INTO [table] VALUES ([value])");
    syntax.push_back("UPDATE [table] SET [column] = [value] WHERE [condition]");
    syntax.push_back("DELETE FROM [table] WHERE [condition]");
    syntax.push_back("CREATE TABLE [table] ([column] [type], [column] [type], ...)");
    syntax.push_back("ALTER TABLE [table] ADD [column] [type]");
    syntax.push_back("DROP TABLE [table]");
    syntax.push_back("CREATE INDEX [index] ON [table] ([column])");
    syntax.push_back("PRIMARY KEY ([column])");
    syntax.push_back("FOREIGN KEY ([column]) REFERENCES [table] ([column])");
    syntax.push_back("JOIN [table] ON [condition]");
    syntax.push_back("INNER JOIN [table] ON [condition]");
    syntax.push_back("LEFT JOIN [table] ON [condition]");
    syntax.push_back("RIGHT JOIN [table] ON [condition]");
    syntax.push_back("OUTER JOIN [table] ON [condition]");
    syntax.push_back("UNION [table]");
    syntax.push_back("AND [condition]");
    syntax.push_back("OR [condition]");
    syntax.push_back("NOT [condition]");
    syntax.push_back("IN [condition]");
    syntax.push_back("LIKE [condition]");
    syntax.push_back("IS [condition]");

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
