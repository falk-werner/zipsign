#include "zipsign/impl/error_code.h"
#include "zipsign/error_codes.h"

char const *
zipsign_impl_errorcode_name(
    zipsign_errorcode code)
{
    switch (code)
    {
        case ZIPSIGN_GOOD:
            return "good";
        case ZIPSIGN_BAD:
            return "bad";
        case ZIPSIGN_BAD_NOT_IMPLEMENTED:
            return "not implemented";
        case ZIPSIGN_BAD_OPENSSL_INIT:
            return "init openssl";
        default:
            return "<unknown>";
    }
}