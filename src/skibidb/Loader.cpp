/*
Loader class for SkibiDB.

This class is responsible for loading the database from the disk and storing it in memory,
in the form of Class Instances of Tables and Attributes.
*/
#include "Loader.hpp"

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