/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "openssl++/private_key.hpp"
#include "openssl++/basic_io.hpp"
#include "openssl++/exception.hpp"

#include <openssl/pem.h>
#include <openssl/rsa.h>

namespace openssl
{

PrivateKey PrivateKey::fromPEM(std::string const & filename)
{
    BasicIO key_file = BasicIO::openInputFile(filename);

    EVP_PKEY * key = PEM_read_bio_PrivateKey(key_file, NULL, NULL, NULL);
    if (NULL == key)
    {
        throw OpenSSLException("failed to parse key file");
    }

    return PrivateKey(key);
}

PrivateKey::PrivateKey(EVP_PKEY * key_)
: key(key_)
{

}

PrivateKey & PrivateKey::operator=(PrivateKey && other)
{
    EVP_PKEY_free(this->key);

    this->key = other.key;
    other.key = nullptr;

    return *this;
}

PrivateKey::PrivateKey(PrivateKey && other)
{
    this->key = other.key;
    other.key = nullptr;
}

PrivateKey::~PrivateKey()
{
    EVP_PKEY_free(key);
}

PrivateKey::operator EVP_PKEY *()
{
    return key;
}

}