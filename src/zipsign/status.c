#include "zipsign/status.h"
#include "zipsign/impl/status.h"

char const *
zipsign_status_name(
    zipsign_status status)
{
    return zipsign_impl_status_name(status);
}
