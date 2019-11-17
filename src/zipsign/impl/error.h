#ifndef ZIPSIGN_IMPL_ERROR_H
#define ZIPSIGN_IMPL_ERROR_H

#include "zipsign/error.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void
zipsign_impl_error_set(
    struct zipsign_error * * error,
    zipsign_errorcode code,
    char const * format,
    ...);

extern zipsign_errorcode
zipsign_impl_error_code(
    struct zipsign_error * error);

extern char const *
zipsign_impl_error_message(
    struct zipsign_error * error);


extern void
zipsign_impl_error_release(
    struct zipsign_error * error);


#ifdef __cplusplus
}
#endif

#endif
