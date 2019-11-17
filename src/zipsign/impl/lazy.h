#ifndef ZIPSIGN_IMPL_LAZY_H
#define ZIPSIGN_IMPL_LAZY_H

#include "zipsign/status.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern zipsign_status 
zipsign_impl_sign(
    char const * filename,
    char const * certificate_path);

extern zipsign_status
zipsign_impl_isvalid(
    char const * filename,
    char const * pki_path);

#ifdef __cplusplus
}
#endif

#endif
