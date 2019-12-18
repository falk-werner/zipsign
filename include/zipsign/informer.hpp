#ifndef ZIPSIGN_INFORMER_HPP
#define ZIPSIGN_INFORMER_HPP

#include <string>
#include <iostream>

namespace zipsign
{

class Informer
{
    Informer operator=(Informer const &) = delete;
    Informer(Informer const &) = delete;
public:
    Informer();
    ~Informer();
    void print(std::string const & filename, std::ostream & out = std::cout);
};

}

#endif
