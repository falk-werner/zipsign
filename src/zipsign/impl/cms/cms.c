#include "zipsign/impl/cms/cms.h"
#include "zipsign/impl/error.h"
#include "zipsign/error_codes.h"

#include <stddef.h>

static void
zipsign_impl_cms_load_private_key(
    char const * filename,
    struct zipsign_error * * error);

char *
zipsign_impl_cms_sign(
    char const * filename,
    char const * private_key_path,
    char const * certificate_path,
    struct zipsign_error * * error)
{
    if (NULL != *error) { return NULL; }

    zipsign_impl_error_set(error, ZIPSIGN_BAD_NOT_IMPLEMENTED, NULL, "");
    return NULL;
}

static void
zipsign_impl_cms_load_private_key(
    char const * filename,
    struct zipsign_error * * error)
{
    if (NULL != *error) { return; }

    zipsign_impl_error_set(error, ZIPSIGN_BAD_NOT_IMPLEMENTED, NULL, "");
}
