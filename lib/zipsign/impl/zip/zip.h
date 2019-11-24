#ifndef ZIPSIGN_IMPL_ZIP_ZIP_H
#define ZIPSIGN_IMPL_ZIP_ZIP_H

#ifdef __cplusplus
extern "C"
{
#endif

struct zipsign_error;

extern void
zipsign_impl_zip_setcomment(
    char const * filename,
    char comment,
    struct zipsign_error * * error);

extern char *
zipsign_impl_zip_getcomment(
    char const * filename,
    struct zipsign_error * * error);

extern long
zipsign_impl_zip_eocd_pos(
    char const * filename,
    struct zipsign_error * * error);

#ifdef __cplusplus
}
#endif

#endif
