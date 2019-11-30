/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "openssl++/private_key.hpp"
#include "openssl++/basic_io.hpp"
#include "openssl++/big_number.hpp"
#include "openssl++/exception.hpp"

#include <openssl/pem.h>
#include <openssl/rsa.h>

namespace openssl
{

PrivateKey PrivateKey::generateRSA(int bits)
{
    BigNumber exponent;
    exponent.setWord(RSA_F4);

    RSA * rsa = RSA_new();
    int rc = RSA_generate_key_ex(rsa, bits, exponent, nullptr);

    EVP_PKEY * key = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(key, rsa);

    return std::move(PrivateKey(key));
}

PrivateKey PrivateKey::fromPEM(std::string const & filename)
{
    BasicIO key_file = BasicIO::openInputFile(filename);

    EVP_PKEY * key = PEM_read_bio_PrivateKey(key_file, NULL, NULL, NULL);
    if (NULL == key)
    {
        throw OpenSSLException("failed to parse key file");
    }

    return std::move(PrivateKey(key));
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