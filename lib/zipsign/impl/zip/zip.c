#include "zipsign/impl/zip/zip.h"
#include "zipsign/impl/error.h"
#include "zipsign/error_codes.h"

#include <stddef.h>

void
zipsign_impl_zip_setcomment(
    char const * filename,
    char comment,
    struct zipsign_error * * error)
{
    if (NULL != *error) { return; }

    zipsign_impl_error_set(error, ZIPSIGN_BAD_NOT_IMPLEMENTED, NULL, "");
}

char *
zipsign_impl_zip_getcomment(
    char const * filename,
    struct zipsign_error * * error)
{
    if (NULL != *error) { return NULL; }

    zipsign_impl_error_set(error, ZIPSIGN_BAD_NOT_IMPLEMENTED, NULL, "");
    return NULL;
}

long
zipsign_impl_zip_eocd_pos(
    char const * filename,
    struct zipsign_error * * error)
{
    if (NULL != *error) { return 0; }

    zipsign_impl_error_set(error, ZIPSIGN_BAD_NOT_IMPLEMENTED, NULL, "");
    return 0;
}
