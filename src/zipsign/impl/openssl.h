#ifndef ZIPSIGN_IMPL_OPENSSL_H
#define ZIPSIGN_IMPL_OPENSSL_H


#ifdef __cplusplus
extern "C"
{
#endif

struct zipsign_error;

extern void
zipsign_impl_openssl_init(
    struct zipsign_error * * error);

#ifdef __cplusplus
}
#endif


#endif
