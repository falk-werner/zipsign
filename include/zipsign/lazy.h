#ifndef ZIPSIGN_LAZY_H
#define ZIPSIGN_LAZY_H

#include <zipsign/api.h>
#include <zipsign/status.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern ZIPSIGN_API zipsign_status
zipsign_sign(
    char const * filename, 
    char const * certificate_path);

extern ZIPSIGN_API zipsign_status
zipsign_isvalid(
    char const * filename,
    char const * pki_path);

#ifdef __cplusplus
}
#endif

#endif
