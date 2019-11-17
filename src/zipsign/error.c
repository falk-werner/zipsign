#include "zipsign/error.h"
#include "zipsign/impl/error.h"

zipsign_errorcode
zipsign_error_code(
    struct zipsign_error * error)
{
    return zipsign_impl_error_code(error);
}

char const *
zipsign_error_message(
    struct zipsign_error * error)
{
    return zipsign_impl_error_message(error);
}

void
zipsign_error_release(
    struct zipsign_error * error)
{
    return zipsign_impl_error_release(error);
}
