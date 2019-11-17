#ifndef ZIPSIGN_OPENSSL_H
#define ZIPSIGN_OPENSSL_H

#include <zipsign/api.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct zipsign_error;

extern void
zipsign_openssl_init(
    struct zipsign_error * * error);

#ifdef __cplusplus
}
#endif

#endif