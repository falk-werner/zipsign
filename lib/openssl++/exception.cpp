/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
 
#include "openssl++/exception.hpp"

#include <openssl/err.h>
#include <cstring>
#include <sstream>
#include <iomanip>

namespace
{

std::string getOpenSSLError(char const * message)
{
    std::stringstream stream;
    stream << message;


    unsigned long error_code = ERR_get_error();
    if (error_code != 0)
    {
        stream << " (OpenSSL:";
        while (error_code != 0)
        {
            constexpr size_t buffer_size = 256;
            char buffer[buffer_size] = "\0";
            ERR_error_string_n(error_code, buffer, buffer_size);
            stream << ' ' << buffer << " [0x" << std::setw(8) << std::setfill('0') << std::hex << error_code << ']';
            error_code = ERR_get_error();
        }    
        stream << ')';
    }

    return stream.str();
}

}

namespace openssl
{

FileNotFoundException::FileNotFoundException(char const * filename) noexcept
: zipsign::ZipSignException("file not found")
{
    (void) snprintf(message, sizeof(message), "file not found: %s", filename);
}

OpenSSLException::OpenSSLException(char const * message_) noexcept
: zipsign::ZipSignException(message_)
{
    try
    {
        (void) snprintf(message, sizeof(message), "%s", getOpenSSLError(message_).c_str());
    }
    catch(...)
    {
        // message is already initialized by the base class constructor
    }
}

}