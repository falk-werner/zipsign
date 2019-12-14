#include <gtest/gtest.h>
#include <openssl++/exception.hpp>

using openssl::OpenSSLBaseException;

TEST(OpenSSLBaseException, what)
{
    OpenSSLBaseException ex("some error message");
    ASSERT_STREQ("some error message", ex.what());
}