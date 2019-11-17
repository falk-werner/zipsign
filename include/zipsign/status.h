#ifndef ZIPSIGN_STATUS_H
#define ZIPSIGN_STATUS_H

#include <zipsign/api.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define ZIPSIGN_OK                  ((zipsign_status) 0)
#define ZIPSIGN_BAD                 ((zipsign_status) 1)
#define ZIPSIGN_BAD_NOT_IMPLEMENTED ((zipsign_status) 2)

typedef int zipsign_status;

extern ZIPSIGN_API char const *
zipsign_status_name(
        zipsign_status status);

#ifdef __cplusplus
}
#endif

#endif
