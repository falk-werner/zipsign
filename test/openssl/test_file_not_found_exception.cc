#include <gtest/gtest.h>
#include <openssl++/exception.hpp>

using openssl::FileNotFoundException;

TEST(FileNotFoundException, path)
{
    FileNotFoundException ex("some.file");

    ASSERT_STREQ("some.file", ex.path().c_str());
}