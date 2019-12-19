/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include <openssl++/exception.hpp>

using openssl::OpenSSLBaseException;

TEST(OpenSSLBaseException, what)
{
    OpenSSLBaseException ex("some error message");
    ASSERT_STREQ("some error message", ex.what());
}