#include <iostream>

class Loader
{
public:
    Loader()
    {
        std::cout << "Loader constructor" << std::endl;
    }
    ~Loader()
    {
        std::cout << "Loader destructor" << std::endl;
    }
    void load()
    {
        std::cout << "Loader load" << std::endl;
    }
};
