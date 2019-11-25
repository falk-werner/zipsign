#ifndef ZIPSIGN_PARTIAL_INPUT_FILE_HPP
#define ZIPSIGN_PARTIAL_INPUT_FILE_HPP

#include <string>
#include <cstddef>
#include <openssl++/openssl++.hpp>

namespace zipsign
{

class PartialInputFile
{
    PartialInputFile operator=(PartialInputFile const &) = delete;
    PartialInputFile(PartialInputFile const &other) = delete;
public:
    PartialInputFile();
    ~PartialInputFile();
    openssl::BasicIO open(std::string const & filename, std::size_t upperLimit);
private:
    BIO_METHOD * method;
};

}

#endif
