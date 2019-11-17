#include "zipsign/error_code.h"
#include "zipsign/impl/error_code.h"

char const *
zipsign_errorcode_name(
    zipsign_errorcode status)
{
    return zipsign_impl_errorcode_name(status);
}
