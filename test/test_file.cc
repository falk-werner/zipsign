#include <gtest/gtest.h>
#include "zipsign/file.hpp"

using zipsign::File;

TEST(File, FailedToOpenNonExistingFile)
{
    ASSERT_THROW({
        File file("_non_existing.file", "rb");
    }, std::exception);
}

TEST(File, OpenReadOnly)
{
    File file("test.zip", "rb");
}

TEST(File, SeekAndTell)
{
    File file("test.zip", "rb");
    file.seek(0, SEEK_END);
    auto length = file.tell();

    ASSERT_LT(0, length);
}

TEST(File, FailedToSeekBeforeBegin)
{
    File file("test.zip", "rb");

    ASSERT_THROW({
        file.seek(-1, SEEK_SET);
    }, std::exception);
}

TEST(File, WriteAndRead)
{    
    {
        File file("new.file", "wb");

        uint8_t data[] = {1, 2, 3};
        file.write(data, 3);
    }

    {
        File file("new.file", "rb");

        uint8_t data[3];
        auto length = file.read(data, 3);

        ASSERT_EQ(3, length);
        ASSERT_EQ(1, data[0]);
        ASSERT_EQ(2, data[1]);
        ASSERT_EQ(3, data[2]);
    }

    File::remove("new.file");
}

TEST(File, FailedToWriteReadonlyFile)
{
    File file("test.zip", "rb");

    ASSERT_THROW({
        uint8_t data[] = { 0x42 };
        file.write(data, 1);
    }, std::exception);
}

TEST(File, FailedToReadOutOfBounds)
{
    {
        File file("new.file", "wb");

        uint8_t data[] = {1, 2, 3};
        file.write(data, 3);
    }

    {
        File file("new.file", "rb");

        ASSERT_THROW({
            uint8_t data[3];
            auto length = file.read(data, 4);
        }, std::exception);
    }

    File::remove("new.file");
}

TEST(File, ReadUnceckedOutOfBound)
{
    {
        File file("new.file", "wb");

        uint8_t data[] = {1, 2, 3};
        file.write(data, 3);
    }

    {
        File file("new.file", "rb");

        uint8_t data[3];
        auto length = file.read(data, 4, false);

        ASSERT_EQ(3, length);
        ASSERT_EQ(1, data[0]);
        ASSERT_EQ(2, data[1]);
        ASSERT_EQ(3, data[2]);
    }

    File::remove("new.file");
}

TEST(File, Truncate)
{
    {
        File file("new.file", "wb");

        uint8_t data[] = {1, 2, 3};
        file.write(data, 3);
        file.seek(1, SEEK_SET);
        file.truncate(1);
    }

    {
        File file("new.file", "rb");

        uint8_t data[3];
        auto length = file.read(data, 1);

        ASSERT_EQ(1, length);
        ASSERT_EQ(1, data[0]);
    }

    File::remove("new.file");
}

TEST(File, FailToTruncateReadonlyFile)
{
    File file("test.zip", "rb");
    
    ASSERT_THROW({
        file.truncate(1);
    }, std::exception);
}