#ifndef ZIPSIGN_IMPL_CMS_CMS_H
#define ZIPSIGN_IMPL_CMS_CMS_H


#ifdef __cplusplus
extern "C"
{
#endif

struct zipsign_error;

extern char *
zipsign_impl_cms_sign(
    char const * filename,
    char const * private_key_path,
    char const * certificate_path,
    struct zipsign_error * * error);


#ifdef __cplusplus
}
#endif


#endif
