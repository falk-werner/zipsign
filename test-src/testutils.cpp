/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "testutils.hpp"

#include <fstream>

namespace testutils
{

void copy_file(std::string const & from, std::string const & to)
{
    std::ifstream source_file(from, std::ios::binary);
    std::ofstream target_file(to, std::ios::binary);

    target_file << source_file.rdbuf();
}

}