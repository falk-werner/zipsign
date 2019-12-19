/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include <openssl++/exception.hpp>

using openssl::FileNotFoundException;

TEST(FileNotFoundException, path)
{
    FileNotFoundException ex("some.file");

    ASSERT_STREQ("some.file", ex.path().c_str());
}