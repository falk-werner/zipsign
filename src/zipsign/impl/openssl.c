#include "zipsign/impl/openssl.h"
 
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/crypto.h>

#include "zipsign/impl/error.h"
#include "zipsign/error_codes.h"

void
zipsign_impl_openssl_init(
    struct zipsign_error * * error)
{
    if (NULL != *error) { return; }

#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
    OPENSSL_config(NULL);
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_string();
#else
    int const status = OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    if (1 != status)
    {
        unsigned long const error_code = ERR_get_error();
        zipsign_impl_error_set(error, ZIPSIGN_BAD_OPENSSL_INIT, "%s", ERR_error_string(error_code, NULL));
    }
#endif
}
