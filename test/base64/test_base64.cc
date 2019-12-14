/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include "base64/base64.h"

TEST(Base64, EncodedSize)
{
    ASSERT_EQ(4, base64_encoded_size(1));
    ASSERT_EQ(4, base64_encoded_size(2));
    ASSERT_EQ(4, base64_encoded_size(3));

    ASSERT_EQ(8, base64_encoded_size(4));
    ASSERT_EQ(8, base64_encoded_size(5));
    ASSERT_EQ(8, base64_encoded_size(6));

    ASSERT_EQ(120, base64_encoded_size(90));
}

TEST(Base64, Encode)
{
    char buffer[42];

    std::string in = "Hello";
    size_t length = base64_encode((uint8_t const*) in.c_str(), in.size(), buffer, 42);
    ASSERT_EQ(8, length);
    ASSERT_STREQ("SGVsbG8=", buffer);

    in = "Hello\n";
    length = base64_encode((uint8_t const*) in.c_str(), in.size(), buffer, 42);
    ASSERT_EQ(8, length);
    ASSERT_STREQ("SGVsbG8K", buffer);

    in = "Blue";
    length = base64_encode((uint8_t const*) in.c_str(), in.size(), buffer, 42);
    ASSERT_EQ(8, length);
    ASSERT_STREQ("Qmx1ZQ==", buffer);
}

TEST(Base64, FailedToEncodeBufferTooSmall)
{
    char buffer[1];

    std::string in = "Hello";
    size_t length = base64_encode((uint8_t const*) in.c_str(), in.size(), buffer, 1);
    ASSERT_EQ(0, length);
}

TEST(Base64, DecodedSize)
{
    std::string in = "SGVsbG8=";    // Hello
    size_t length = base64_decoded_size(in.c_str(), in.size());
    ASSERT_EQ(5, length);

    in = "SGVsbG8K";    // Hello\n
    length = base64_decoded_size(in.c_str(), in.size());
    ASSERT_EQ(6, length);

    in = "Qmx1ZQ==";    // Blue
    length = base64_decoded_size(in.c_str(), in.size());
    ASSERT_EQ(4, length);
}

TEST(Base64, IsValid)
{
    std::string in = "SGVsbG8=";    // Hello
    ASSERT_TRUE(base64_isvalid(in.c_str(), in.size()));

    in = "SGVsbG8K";    // Hello\n
    ASSERT_TRUE(base64_isvalid(in.c_str(), in.size()));

    in = "Qmx1ZQ==";    // Blue
    ASSERT_TRUE(base64_isvalid(in.c_str(), in.size()));

    in = "Qmx1ZQ=a";
    ASSERT_FALSE(base64_isvalid(in.c_str(), in.size()));

    in = "Qmx1ZQ";
    ASSERT_FALSE(base64_isvalid(in.c_str(), in.size()));

    in = "Qmx1ZQ=";
    ASSERT_FALSE(base64_isvalid(in.c_str(), in.size()));

    in = "Qmx1Z===";
    ASSERT_FALSE(base64_isvalid(in.c_str(), in.size()));

    in = "Qmx1ZQ?=";
    ASSERT_FALSE(base64_isvalid(in.c_str(), in.size()));

    in = "Qm?1ZQ==";
    ASSERT_FALSE(base64_isvalid(in.c_str(), in.size()));
}

TEST(Base64, Decode)
{
    char buffer[42];

    std::string in = "SGVsbG8=";    // Hello
    size_t length = base64_decode(in.c_str(), in.size(), (uint8_t*) buffer, 42);
    ASSERT_EQ(5, length);
    buffer[length] = '\0';
    ASSERT_STREQ("Hello", buffer);

    in = "SGVsbG8K";    // Hello\n
    length = base64_decode(in.c_str(), in.size(), (uint8_t*) buffer, 42);
    ASSERT_EQ(6, length);
    buffer[length] = '\0';
    ASSERT_STREQ("Hello\n", buffer);

    in = "Qmx1ZQ==";    // Blue
    length = base64_decode(in.c_str(), in.size(), (uint8_t*) buffer, 42);
    ASSERT_EQ(4, length);
    buffer[length] = '\0';
    ASSERT_STREQ("Blue", buffer);
}

TEST(Base64, FailToDecodeBufferTooSmall)
{
    char buffer[1];

    std::string in = "SGVsbG8=";    // Hello
    size_t length = base64_decode(in.c_str(), in.size(), (uint8_t*) buffer, 1);
    ASSERT_EQ(0, length);
}

