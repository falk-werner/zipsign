/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>
#include <vector>
#include <cinttypes>

namespace zipsign
{

std::string b64_encode(uint8_t const * buffer, size_t length);

void b64_decode(std::string const &from, std::vector<uint8_t> &to);


}

#endif
