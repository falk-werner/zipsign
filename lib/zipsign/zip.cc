#include "zipsign/zip.hpp"

namespace zipsign
{

Zip::Zip(std::string const & filename_)
: filename(filename_)
{

}

Zip::~Zip()
{

}

void Zip::setComment(std::string const & comment)
{

}

std::string Zip::getComment()
{
    return "";    
}

std::size_t Zip::getEocdPos()
{
    return 0;
}

}