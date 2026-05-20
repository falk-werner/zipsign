/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef ZIPSIGN_PARTIAL_INPUT_FILE_HPP
#define ZIPSIGN_PARTIAL_INPUT_FILE_HPP

#include <string>
#include <cstddef>
#include <openssl++/openssl++.hpp>

namespace zipsign
{

namespace PartialInputFile
{

openssl::BasicIO open(std::string const & filename, std::size_t upperLimit);

}

}

#endif
