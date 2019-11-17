#ifndef ZIPSIGN_ERRORCODES_H
#define ZIPSIGN_ERRORCODES_H

#include <zipsign/error_code.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define ZIPSIGN_GOOD                ((zipsign_errorcode) 0)
#define ZIPSIGN_BAD                 ((zipsign_errorcode) 1)
#define ZIPSIGN_BAD_NOT_IMPLEMENTED ((zipsign_errorcode) 2)

#define ZIPSIGN_BAD_OPENSSL_INIT ((zipsign_errorcode) 1001)

#ifdef __cplusplus
}
#endif


#endif
