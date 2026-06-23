#ifndef ZIPSIGN_EXCEPTION_HPP
#define ZIPSIGN_EXCEPTION_HPP

#include <exception>

namespace zipsign
{

class ZipSignException: public std::exception
{
public:
    ZipSignException(char const * message_) noexcept;
    ~ZipSignException() = default;
    char const * what() const noexcept override;
protected:
    char message[80];
};

}

#endif

