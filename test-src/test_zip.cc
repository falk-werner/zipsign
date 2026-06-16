/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>

#include "zipsign/zip.hpp"

using zipsign::Zip;

TEST(zipsign, getzipcommentpos)
{
    Zip zip("test.zip");

    ASSERT_EQ(50707, zip.getCommentStart());
}

TEST(zipsign, getzipcomment)
{
    Zip zip("test.zip");
    ASSERT_STREQ("brummni", zip.getComment().c_str());
}