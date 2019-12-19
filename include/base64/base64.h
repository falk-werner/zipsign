/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BASE64_H
#define BASE64_H

#ifndef __cplusplus
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#else
#include <cinttypes>
#include <cstddef>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern size_t base64_encoded_size(size_t length);

extern size_t base64_encode(
    uint8_t const * data,
    size_t length,
    char * buffer,
    size_t buffer_size);

extern size_t base64_decoded_size(char const * data, size_t length);

extern size_t base64_decode(
    char const * data,
    size_t length,
    uint8_t * buffer,
    size_t buffer_size);

extern bool base64_isvalid(char const * data, size_t length);

#ifdef __cplusplus
}
#endif

#endif
