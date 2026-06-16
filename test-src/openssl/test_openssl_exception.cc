/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <openssl++/exception.hpp>
#include <openssl/bio.h>

using openssl::OpenSSLException;
using testing::HasSubstr;

TEST(OpenSSLException, ContainsOpenSSLError)
{
    BIO * file = BIO_new_file("non-existing.file", "rb");
    ASSERT_EQ(nullptr, file);

    OpenSSLException ex("open file");
    ASSERT_THAT(ex.what(), HasSubstr("open file"));
}