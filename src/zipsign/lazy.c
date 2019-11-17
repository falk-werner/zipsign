#include "zipsign/lazy.h"
#include "zipsign/impl/lazy.h"

zipsign_status
zipsign_sign(
    char const * filename, 
    char const * certificate_path)
{
    return zipsign_impl_sign(filename, certificate_path);
}

zipsign_status
zipsign_isvalid(
    char const * filename,
    char const * pki_path)
{
    return zipsign_impl_isvalid(filename, pki_path);
}

