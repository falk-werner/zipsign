/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "testutils.hpp"

#include <zipsign/zipsign.hpp>
#include <zipsign/file.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sstream>

using zipsign::Signer;
using zipsign::File;

using testing::HasSubstr;
using testing::Not;

#define TEST_ARCHIVE "informer.zip"

class InfoTest: public ::testing::Test
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

TEST_F(InfoTest, WithoutEmbeddedSignerCertificate)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";

    Signer signer(key_file, cert_file);
    signer.sign(TEST_ARCHIVE);

    int argc = 4;
    char arg0[] = "zipsign";
    char arg1[] = "info";
    char arg2[] = "-f";
    char arg3[] = TEST_ARCHIVE;
    char* argv[] = {arg0, arg1, arg2, arg3, nullptr};
    std::stringstream err;
    std::stringstream out;
    auto result = zipsign::main(argc, argv, out, err);
    ASSERT_EQ(0, result);
    ASSERT_TRUE(err.str().empty());

    auto out_str = out.str();
    ASSERT_THAT(out_str.c_str(), HasSubstr("Signing CA"));
    ASSERT_THAT(out_str.c_str(), Not(HasSubstr("Alice")));
}

TEST_F(InfoTest, WithEmbeddedSignerCertificate)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";

    Signer signer(key_file, cert_file);
    signer.setEmbedCerts(true);
    signer.sign(TEST_ARCHIVE);

    int argc = 4;
    char arg0[] = "zipsign";
    char arg1[] = "info";
    char arg2[] = "-f";
    char arg3[] = TEST_ARCHIVE;
    char* argv[] = {arg0, arg1, arg2, arg3, nullptr};
    std::stringstream err;
    std::stringstream out;
    auto result = zipsign::main(argc, argv, out, err);
    ASSERT_EQ(0, result);
    ASSERT_TRUE(err.str().empty());

    auto out_str = out.str();
    ASSERT_THAT(out_str.c_str(), HasSubstr("Signing CA"));
    ASSERT_THAT(out_str.c_str(), HasSubstr("Alice"));
}

TEST_F(InfoTest, Fail_InvalidArgument)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";

    Signer signer(key_file, cert_file);
    signer.setEmbedCerts(true);
    signer.sign(TEST_ARCHIVE);

    int argc = 5;
    char arg0[] = "zipsign";
    char arg1[] = "info";
    char arg2[] = "-f";
    char arg3[] = TEST_ARCHIVE;
    char arg4[] = "--invalid";
    char* argv[] = {arg0, arg1, arg2, arg3, arg4, nullptr};
    std::stringstream err;
    std::stringstream out;
    auto const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(EXIT_FAILURE, result);
    ASSERT_FALSE(err.str().empty());
    ASSERT_FALSE(out.str().empty());
}

TEST_F(InfoTest, Fail_UnsignedArchive)
{
    int argc = 4;
    char arg0[] = "zipsign";
    char arg1[] = "info";
    char arg2[] = "-f";
    char arg3[] = TEST_ARCHIVE;
    char* argv[] = {arg0, arg1, arg2, arg3, nullptr};
    std::stringstream err;
    std::stringstream out;
    auto const result = zipsign::main(argc, argv, out, err);
    
    ASSERT_EQ(EXIT_FAILURE, result);
    ASSERT_FALSE(err.str().empty());
    ASSERT_TRUE(out.str().empty());
}

TEST(info, fail_missing_filename)
{
    int argc = 2;
    char arg0[] = "zipsign";
    char arg1[] = "info";
    char* argv[] = {arg0, arg1, nullptr};
    std::stringstream err;
    std::stringstream out;
    auto const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(EXIT_FAILURE, result);
    ASSERT_FALSE(err.str().empty());
    ASSERT_FALSE(out.str().empty());
}

TEST(info, fail_nonexisting_file)
{
    int argc = 4;
    char arg0[] = "zipsign";
    char arg1[] = "info";
    char arg2[] = "-f";
    char arg3[] = TEST_ARCHIVE ".nonexisting";
    char* argv[] = {arg0, arg1, arg2, arg3, nullptr};
    std::stringstream err;
    std::stringstream out;
    auto const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(EXIT_FAILURE, result);
    ASSERT_FALSE(err.str().empty());
    ASSERT_TRUE(out.str().empty());
}

