/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef OPENSSL_BIO_HPP
#define OPENSSL_BIO_HPP

#include <openssl/bio.h>
#include <string>

namespace openssl
{

class BasicIO
{
    BasicIO operator=(BasicIO const &) = delete;
    BasicIO(BasicIO const &) = delete;
public:
    static BasicIO openInputFile(std::string const & filename);
    static BasicIO openOutputFile(std::string const & filename);
    static BasicIO getStdout(void);
    static BasicIO fromMemory();
    explicit BasicIO(BIO * bio_);
    BasicIO & operator=(BasicIO && other);
    BasicIO(BasicIO && other);
    ~BasicIO();
    operator BIO*();
private:
    BIO * bio;
};

}

#endif
