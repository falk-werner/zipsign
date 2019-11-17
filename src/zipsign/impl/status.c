#include "zipsign/impl/status.h"

char const *
zipsign_impl_status_name(
    zipsign_status status)
{
    switch (status)
    {
        case ZIPSIGN_OK:
            return "ok";
        case ZIPSIGN_BAD:
            return "bad";
        case ZIPSIGN_BAD_NOT_IMPLEMENTED:
            return "not implemented";
        default:
            return "<unknown>";
    }
}
