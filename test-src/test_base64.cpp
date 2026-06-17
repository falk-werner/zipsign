/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include "zipsign/base64.hpp"

using zipsign::b64_decode;
using zipsign::b64_encode;

TEST(Base64, Encode)
{
    std::string in = "Hello";
    auto encoded = b64_encode((uint8_t const*) in.c_str(), in.size());
    ASSERT_EQ(8, encoded.size());
    ASSERT_EQ("SGVsbG8=", encoded);

    in = "Hello\n";
    encoded = b64_encode((uint8_t const*) in.c_str(), in.size());
    ASSERT_EQ(8, encoded.size());
    ASSERT_EQ("SGVsbG8K", encoded);

    in = "Blue";
    encoded = b64_encode((uint8_t const*) in.c_str(), in.size());
    ASSERT_EQ(8, encoded.size());
    ASSERT_EQ("Qmx1ZQ==", encoded);
}

TEST(Base64, EncodeFailOnVeryLargeBuffers)
{
    EXPECT_ANY_THROW({
        b64_encode(nullptr, SIZE_MAX);
    });
}


TEST(Base64, Decode)
{
    std::vector<uint8_t> buffer;

    std::string in = "SGVsbG8=";    // Hello
    b64_decode(in, buffer);
    ASSERT_EQ(5, buffer.size());
    ASSERT_EQ("Hello", std::string((char const *)buffer.data(), buffer.size()));

    in = "SGVsbG8K";    // Hello\n
    b64_decode(in, buffer);
    ASSERT_EQ(6, buffer.size());
    ASSERT_EQ("Hello\n", std::string((char const *)buffer.data(), buffer.size()));

    in = "Qmx1ZQ==";    // Blue
    b64_decode(in, buffer);
    ASSERT_EQ(4, buffer.size());
    ASSERT_EQ("Blue", std::string((char const *)buffer.data(), buffer.size()));
}

TEST(Base64, DecodeEmpty)
{
    std::vector<uint8_t> buffer;
    b64_decode("", buffer);
    ASSERT_EQ(0, buffer.size());
}

TEST(Base64, DecodeFailInvalidSize)
{
    std::vector<uint8_t> buffer;
    EXPECT_ANY_THROW({
        b64_decode("Qmx1ZQ", buffer);
    });
}

TEST(Base64, DecodeFailInvalidInput)
{
    std::vector<uint8_t> buffer;
    EXPECT_ANY_THROW({
        b64_decode(";mx1ZQ==", buffer);
    });
}

TEST(Base64, DecodeFailWithPaddingInData)
{
    std::vector<uint8_t> buffer;
    EXPECT_ANY_THROW({
        b64_decode("=mx1ZQ==", buffer);
    });
}

TEST(Base64, DecodeFailWithInvalidPadding)
{
    std::vector<uint8_t> buffer;
    EXPECT_ANY_THROW({
        b64_decode("Qmx1ZQ=A", buffer);
    });
}