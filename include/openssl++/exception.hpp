/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef OPENSSL_EXCEPTION_HPP
#define OPENSSL_EXCEPTION_HPP

#include <exception>
#include <string>

namespace openssl
{

class OpenSSLBaseException: public std::exception
{
public:
    OpenSSLBaseException(std::string const & message_);
    ~OpenSSLBaseException();
    char const * what() const noexcept override;
private:
    std::string message;
};

class FileNotFoundException: public OpenSSLBaseException
{
public:
    FileNotFoundException(std::string const & filename_);
    ~FileNotFoundException();
    std::string const & path() const;
private:
    std::string filename;
};

class OpenSSLException: public OpenSSLBaseException
{
public:
    OpenSSLException(std::string const & message_);
    ~OpenSSLException();
};

}

#endif
