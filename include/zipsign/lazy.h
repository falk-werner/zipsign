#ifndef ZIPSIGN_LAZY_H
#define ZIPSIGN_LAZY_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <zipsign/api.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct zipsign_error;

extern ZIPSIGN_API void
zipsign_sign(
    char const * filename, 
    char const * certificate_path,
    struct zipsign_error * * error);

extern ZIPSIGN_API bool
zipsign_isvalid(
    char const * filename,
    char const * pki_path,
    struct zipsign_error * * error);

#ifdef __cplusplus
}
#endif

#endif
