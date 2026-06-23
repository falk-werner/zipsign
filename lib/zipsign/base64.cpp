/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/base64.hpp"
#include <zipsign/exception.hpp>

namespace
{

uint8_t base64_valueof(uint8_t c)
{
    constexpr const uint8_t table[256] = {
        // 0     1     2     3     4     5     6     7     8      9    A     B     C     D     E     F  
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // 0
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // 1
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,   62, 0x80, 0x80, 0x80,   63, // 2
          52,   53,   54,   55,   56,   57,   58,   59,   60,   61, 0x80, 0x80, 0x80, 0xff, 0x80, 0x80, // 3
        0x80,    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14, // 4
          15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25, 0x80, 0x80, 0x80, 0x80, 0x80, // 5
        0x80,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40, // 6
          41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51, 0x80, 0x80, 0x80, 0x80, 0x80, // 7
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // 8
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // 9
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // A
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // B
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // C
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // D
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // E
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // F
    };


    uint8_t const value = table[c];
    if (value == 0x80)
    {
        throw zipsign::ZipSignException("base64 decoding error: invalid value");
    }
    else if (value == 0xff)
    {
        throw zipsign::ZipSignException("base64 decoding error: invalid use of padding");
    }

    return value;
}

}

namespace zipsign
{

std::string b64_encode(uint8_t const * buffer, size_t length)
{
    if (length > (SIZE_MAX / 2))
    {
        throw zipsign::ZipSignException("base64 encoding error: buffer too large");
    }

    constexpr const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t const encoded_size = ((length + 2) / 3) * 4;
    std::string encoded(encoded_size, '=');

    size_t out_pos = 0;
    size_t pos = 0;
    for(; (length - pos) >= 3; pos += 3)
    {
        encoded[out_pos++] = table[ buffer[pos] >> 2 ];
        encoded[out_pos++] = table[ ((buffer[pos    ] & 0x03) << 4) | (buffer[pos + 1] >> 4) ];
        encoded[out_pos++] = table[ ((buffer[pos + 1] & 0x0f) << 2) | (buffer[pos + 2] >> 6) ];
        encoded[out_pos++] = table[ buffer[pos + 2] & 0x3f ];
    }

    switch((length - pos))
    {
        case 1:
            encoded[out_pos++] = table[ buffer[pos] >> 2 ];
            encoded[out_pos++] = table[ ((buffer[pos] & 0x03) << 4) ];
            break;
        case 2:
            encoded[out_pos++] = table[ buffer[pos] >> 2 ];
            encoded[out_pos++] = table[ ((buffer[pos    ] & 0x03) << 4) | (buffer[pos + 1] >> 4) ];
            encoded[out_pos++] = table[ ((buffer[pos + 1] & 0x0f) << 2) ];
            break;
        default:
            break;
    }

    return encoded;
}


void b64_decode(std::string const &from, std::vector<uint8_t> &to)
{
    if ((from.size() % 4) != 0)
    {
        throw zipsign::ZipSignException("base64 decoding error: invalid size");
    }

    to.clear();
    to.reserve((from.size() * 3) / 4);

    size_t pos = 0;
    uint8_t const * data = reinterpret_cast<uint8_t const *>(from.data());
    for(; (pos + 4) < from.size() ; pos += 4)
    {
        uint8_t a = base64_valueof(data[pos    ]);
        uint8_t b = base64_valueof(data[pos + 1]);
        uint8_t c = base64_valueof(data[pos + 2]);
        uint8_t d = base64_valueof(data[pos + 3]);

        to.push_back((a << 2) | (b >> 4));
        to.push_back((b << 4) | (c >> 2));
        to.push_back((c << 6) | d);
    }

    // decode last block
    if (pos < from.size())
    {
        uint8_t a = base64_valueof(data[pos    ]);
        uint8_t b = base64_valueof(data[pos + 1]);

        to.push_back((a << 2) | (b >> 4));
        if (('=' == data[pos + 2]) && ('=' == data[pos + 3]))
        {
            return;
        }

        uint8_t c = base64_valueof(data[pos + 2]);
        to.push_back((b << 4) | (c >> 2));

        if ('=' == data[pos + 3])
        {
            return;
        }

        uint8_t d = base64_valueof(data[pos + 3]);
        to.push_back((c << 6) | d);
    }
}

}
