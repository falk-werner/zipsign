/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include "zipsign/partial_input_file.hpp"

using zipsign::PartialInputFile;

TEST(zipsign, dummy)
{
    PartialInputFile partialFile;
    auto file = partialFile.open("message.txt", 2);

    BIO * in = file;

    char buffer[100];
    int i = BIO_read(in, buffer, 100);
    ASSERT_EQ(2, i);
    ASSERT_EQ('4', buffer[0]);
    ASSERT_EQ('2', buffer[1]);
}
