#include "zipsign/exception.hpp"

#include <cstdio>

namespace zipsign
{

ZipSignException::ZipSignException(char const * message_) noexcept
{
    (void) snprintf(message, sizeof(message), "%s", message_);
}


char const * ZipSignException::what() const noexcept
{
    return message;
}


}