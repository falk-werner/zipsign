#include "zipsign/openssl.h"
#include "zipsign/impl/openssl.h"

void
zipsign_openssl_init(
    struct zipsign_error * * error
)
{
    zipsign_impl_openssl_init(error);
}
