/*
Loader class for SkibiDB.

This class is responsible for loading the database from the disk and storing it in memory,
in the form of Class Instances of Tables and Attributes.
*/
#include "Loader.hpp"

Loader::Loader()
{
    std::cout << "Loader created" << std::endl;
}

Loader::~Loader()
{
    std::cout << "Loader destroyed" << std::endl;
}

void Loader::load()
{
    std::cout << "Loading database" << std::endl;
}