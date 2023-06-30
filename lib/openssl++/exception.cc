/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
 
#include "openssl++/exception.hpp"

#include <openssl/err.h>
#include <sstream>
#include <iomanip>

namespace
{

std::string getOpenSSLError(std::string const & message)
{
    constexpr size_t buffer_size = 256;
    char buffer[buffer_size] = "\0";
    unsigned long const error_code = ERR_get_error();
    ERR_error_string_n(error_code, buffer, buffer_size);
    
    std::stringstream stream;
    stream << "error: " << message
        << " (OpenSSL: " << buffer
        << " [0x" << std::setw(8) << std::setfill('0') << std::hex << error_code << "])"
    ;

    return stream.str();
}

}

namespace openssl
{

OpenSSLBaseException::OpenSSLBaseException(std::string const & message_)
: message(message_)
{

}

OpenSSLBaseException::~OpenSSLBaseException()
{

}

char const * OpenSSLBaseException::what() const noexcept
{
    return message.c_str();
}

FileNotFoundException::FileNotFoundException(std::string const & filename_)
: OpenSSLBaseException("file not found: " + filename_)
, filename(filename_)
{

}

FileNotFoundException::~FileNotFoundException()
{

}

std::string const & FileNotFoundException::path() const
{
    return filename;
}

OpenSSLException::OpenSSLException(std::string const & message_)
: OpenSSLBaseException(getOpenSSLError(message_))
{

}

OpenSSLException::~OpenSSLException()
{

}

}