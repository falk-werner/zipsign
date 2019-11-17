#include "zipsign/lazy.h"
#include "zipsign/impl/lazy.h"

void
zipsign_sign(
    char const * filename, 
    char const * certificate_path,
    struct zipsign_error * * error)
{
    return zipsign_impl_sign(filename, certificate_path, error);
}

bool
zipsign_isvalid(
    char const * filename,
    char const * pki_path,
    struct zipsign_error * * error)
{
    return zipsign_impl_isvalid(filename, pki_path, error);
}

