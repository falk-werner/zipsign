/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef OPENSSL_EXCEPTION_HPP
#define OPENSSL_EXCEPTION_HPP

#include <zipsign/exception.hpp>
#include <exception>
#include <string>

namespace openssl
{

class FileNotFoundException: public zipsign::ZipSignException
{
public:
    FileNotFoundException(char const * filename) noexcept;
    ~FileNotFoundException() = default;
};

class OpenSSLException: public zipsign::ZipSignException
{
public:
    OpenSSLException(char const * message_) noexcept;
    ~OpenSSLException() = default;
};

}

#endif
