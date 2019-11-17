#ifndef ZIPSIGN_ERROR_CODE_H
#define ZIPSIGN_ERROR_CODE_H

#include <zipsign/api.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef int zipsign_errorcode;

extern ZIPSIGN_API char const *
zipsign_errorcode_name(
        zipsign_errorcode code);

#ifdef __cplusplus
}
#endif

#endif
