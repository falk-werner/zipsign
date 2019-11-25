#include <gtest/gtest.h>
#include <iostream>

#include "zipsign/partial_input_file.hpp"
#include "zipsign/zip.hpp"

using zipsign::PartialInputFile;
using zipsign::Zip;

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