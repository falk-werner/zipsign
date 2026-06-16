/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef TEST_FILE_UTILS_HPP
#define TEST_FILE_UTILS_HPP

#include <string>

namespace testutils
{

void copy_file(std::string const & from, std::string const & to);

}

#endif
