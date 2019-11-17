#ifndef ZIPSIGN_IMPL_LAZY_H
#define ZIPSIGN_IMPL_LAZY_H

#ifndef __cpluscplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct zipsign_error;

extern void 
zipsign_impl_sign(
    char const * filename,
    char const * certificate_path,
    struct zipsign_error * * error);

extern bool
zipsign_impl_isvalid(
    char const * filename,
    char const * pki_path,
    struct zipsign_error * * error);

#ifdef __cplusplus
}
#endif

#endif
