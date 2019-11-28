/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef OPENSSL_PRIVATE_KEY_HPP
#define OPENSSL_PRIVATE_KEY_HPP

#include <openssl/evp.h>
#include <string>

namespace openssl
{

class PrivateKey
{
    PrivateKey& operator=(PrivateKey const &) = delete;
    PrivateKey(PrivateKey const &) = delete;
public:
    static PrivateKey generateRSA(int bits = 4096);
    static PrivateKey fromPEM(std::string const & filename);
    explicit PrivateKey(EVP_PKEY * key_);
    PrivateKey & operator=(PrivateKey && other);
    PrivateKey(PrivateKey && other);
    ~PrivateKey();
    operator EVP_PKEY*();
private:
    EVP_PKEY * key;
};

}



#endif
