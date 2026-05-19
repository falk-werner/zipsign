/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include "base64/base64.hpp"

TEST(Base64, Encode)
{
    std::string in = "Hello";
    auto encoded = base64::encode((uint8_t const*) in.c_str(), in.size());
    ASSERT_EQ(8, encoded.size());
    ASSERT_EQ("SGVsbG8=", encoded);

    in = "Hello\n";
    encoded = base64::encode((uint8_t const*) in.c_str(), in.size());
    ASSERT_EQ(8, encoded.size());
    ASSERT_EQ("SGVsbG8K", encoded);

    in = "Blue";
    encoded = base64::encode((uint8_t const*) in.c_str(), in.size());
    ASSERT_EQ(8, encoded.size());
    ASSERT_EQ("Qmx1ZQ==", encoded);
}

TEST(Base64, EncodeFailOnVeryLargeBuffers)
{
    EXPECT_ANY_THROW({
        base64::encode(nullptr, SIZE_MAX);
    });
}


TEST(Base64, Decode)
{
    std::vector<uint8_t> buffer;

    std::string in = "SGVsbG8=";    // Hello
    base64::decode(in, buffer);
    ASSERT_EQ(5, buffer.size());
    ASSERT_EQ("Hello", std::string((char const *)buffer.data(), buffer.size()));

    in = "SGVsbG8K";    // Hello\n
    base64::decode(in, buffer);
    ASSERT_EQ(6, buffer.size());
    ASSERT_EQ("Hello\n", std::string((char const *)buffer.data(), buffer.size()));

    in = "Qmx1ZQ==";    // Blue
    base64::decode(in, buffer);
    ASSERT_EQ(4, buffer.size());
    ASSERT_EQ("Blue", std::string((char const *)buffer.data(), buffer.size()));
}

TEST(Base64, DecodeEmpty)
{
    std::vector<uint8_t> buffer;
    base64::decode("", buffer);
    ASSERT_EQ(0, buffer.size());
}

TEST(Base64, DecodeFailInvalidSize)
{
    std::vector<uint8_t> buffer;
    EXPECT_ANY_THROW({
        base64::decode("Qmx1ZQ", buffer);
    });
}

TEST(Base64, DecodeFailInvalidInput)
{
    std::vector<uint8_t> buffer;
    EXPECT_ANY_THROW({
        base64::decode(";mx1ZQ==", buffer);
    });
}

TEST(Base64, DecodeFailWithPaddingInData)
{
    std::vector<uint8_t> buffer;
    EXPECT_ANY_THROW({
        base64::decode("=mx1ZQ==", buffer);
    });
}

TEST(Base64, DecodeFailWithInvalidPadding)
{
    std::vector<uint8_t> buffer;
    EXPECT_ANY_THROW({
        base64::decode("Qmx1ZQ=A", buffer);
    });
}