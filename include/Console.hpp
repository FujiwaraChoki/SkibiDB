#include "termcolor.hpp"

class Console
{
public:
    /* Constructor */
    Console();

    /* Destructor */
    ~Console();

    /* Starts the console */
    void start();

    /* Stops the console */
    void stop();

    /* Take user input */
    template <typename T>
    T input(std::string message)
    {
        T value;
        std::cout << message;
        std::cin >> value;
        return value;
    }

private:
    bool isRunning;
    std::string currentPath;
};
