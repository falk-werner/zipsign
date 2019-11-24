/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "openssl++/basic_io.hpp"
#include "openssl++/exception.hpp"

#include <cstdio>

namespace openssl
{

BasicIO BasicIO::getStdout(void)
{
    BIO * out = BIO_new_fp(stdout, BIO_NOCLOSE);

    return std::move(BasicIO(out));
}

BasicIO BasicIO::openInputFile(std::string const & filename)
{
    BIO * file = BIO_new_file(filename.c_str(), "rb");
    if (NULL == file)
    {
        throw FileNotFoundException(filename);
    }

    return std::move(BasicIO(file));
}

BasicIO BasicIO::openOutputFile(std::string const & filename)
{
    BIO * file = BIO_new_file(filename.c_str(), "wb");
    if (NULL == file)
    {
        throw FileNotFoundException(filename);
    }

    return std::move(BasicIO(file));
}


BasicIO BasicIO::fromMemory()
{
    BIO * bio = BIO_new(BIO_s_mem());
    return std::move(BasicIO(bio));
}

BasicIO::BasicIO(BIO * bio_)
: bio(bio_)
{

}

BasicIO::~BasicIO()
{
    BIO_free_all(bio);
}

BasicIO & BasicIO::operator=(BasicIO && other)
{
    BIO_free_all(this->bio);

    this->bio = other.bio;
    other.bio = nullptr;

    return *this;
}

BasicIO::BasicIO(BasicIO && other)
{
    this->bio = other.bio;
    other.bio = nullptr;
}

BasicIO::operator BIO*()
{
    return bio;
}

}
