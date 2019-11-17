#ifndef ZIPSIGN_ERROR_H
#define ZIPSIGN_ERROR_H

#ifndef __cplusplus
#include <stdarg.h>
#else
#include <cstdarg>
#endif

#include <zipsign/api.h>
#include <zipsign/error_code.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct zipsign_error;

extern ZIPSIGN_API zipsign_errorcode
zipsign_error_code(
    struct zipsign_error * error);

extern ZIPSIGN_API char const *
zipsign_error_message(
    struct zipsign_error * error);

extern ZIPSIGN_API void
zipsign_error_release(
    struct zipsign_error * error);

#ifdef __cplusplus
}
#endif

#endif
