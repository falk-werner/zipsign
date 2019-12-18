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
    ASSERT_THAT(ex.what(), HasSubstr("[0x2"));
    ASSERT_THAT(ex.what(), HasSubstr("open file"));
}