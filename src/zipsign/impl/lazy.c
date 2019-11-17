#include "zipsign/impl/lazy.h"
#include "zipsign/impl/error.h"
#include "zipsign/error_codes.h"

#include <stddef.h>

void
zipsign_impl_sign(
    char const * filename,
    char const * certificate_path,
    struct zipsign_error * * error)
{
    if (NULL != *error) { return; }

    zipsign_impl_error_set(error, ZIPSIGN_BAD_NOT_IMPLEMENTED, NULL);
}

bool
zipsign_impl_isvalid(
    char const * filename,
    char const * pki_path,
    struct zipsign_error * * error)
{
    if (NULL != *error) { return false; }

    zipsign_impl_error_set(error, ZIPSIGN_BAD_NOT_IMPLEMENTED, NULL);
}
