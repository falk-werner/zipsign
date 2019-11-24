/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "openssl++/init.hpp"
#include "openssl++/exception.hpp"

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>

namespace openssl
{

void OpenSSL::init(void)
{
#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
	OPENSSL_config(NULL);
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();
#else
    int const rc = OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    if (1 != rc)
    {
        throw OpenSSLException("unable to initialize openssl");
    }
#endif   
}

}
