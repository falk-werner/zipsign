/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "testutils.hpp"
#include "zipsign/main.hpp"
#include <zipsign/file.hpp>

#include <sstream>
#include <gtest/gtest.h>

using zipsign::File;

TEST(cli_sign, empty_args)
{
    char arg0[] = "zipsign";
    char arg1[] = "sign";
    char * argv[] = { arg0, arg1, nullptr };
    int argc = 2;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(EXIT_FAILURE, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_FALSE(err.str().empty());
}

TEST(cli_sign, unknown_option)
{
    char arg0[] = "zipsign";
    char arg1[] = "sign";
    char arg2[] = "--unknown";
    char * argv[] = { arg0, arg1, arg2, nullptr };
    int argc = 3;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    auto err_str = err.str();
    ASSERT_EQ(EXIT_FAILURE, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_FALSE(err_str.empty());
    ASSERT_EQ(0, err_str.find("error: unrecognized argument")) << err_str;
}

#define TEST_ARCHIVE "cli_sign.zip"

class CliSignTest: public ::testing::Test
{
protected:
    void SetUp() override
    {
        testutils::copy_file("test.zip", TEST_ARCHIVE);
    }

    void TearDown() override
    {
        File::remove(TEST_ARCHIVE);
    }
};

TEST_F(CliSignTest, Sign)
{
    char arg0[] = "zipsign";
    char arg1[] = "sign";
    char arg2[] = "-f";
    char arg3[] = TEST_ARCHIVE;
    char arg4[] = "-p";
    char arg5[] = "certs/alice.key";
    char arg6[] = "-c";
    char arg7[] = "certs/alice.crt";
    char arg8[] = "-i";
    char arg9[] = "ca/root-ca.crt";
    char arg10[] = "-e";
    char arg11[] = "-v";
    char * argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, nullptr };
    int argc = 12;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(EXIT_SUCCESS, result);
    ASSERT_TRUE(out.str().empty());
    ASSERT_TRUE(err.str().empty());

}