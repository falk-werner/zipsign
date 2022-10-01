/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "testutils.hpp"

#include <zipsign/zipsign.hpp>
#include <zipsign/file.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using zipsign::Signer;
using zipsign::Informer;
using zipsign::File;

using testing::HasSubstr;
using testing::Not;

#define TEST_ARCHIVE ("informer.zip")

class InformerTest: public ::testing::Test
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

TEST_F(InformerTest, WithoutEmbeddedSignerCertificate)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";

    Signer signer(key_file, cert_file);
    signer.sign(TEST_ARCHIVE);

    Informer informer;
    std::stringstream out;
    informer.print(TEST_ARCHIVE, out);
    auto info = out.str();

    ASSERT_THAT(info.c_str(), HasSubstr("Signing CA"));
    ASSERT_THAT(info.c_str(), Not(HasSubstr("Alice")));
}

TEST_F(InformerTest, WithEmbeddedSignerCertificate)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";

    Signer signer(key_file, cert_file);
    signer.setEmbedCerts(true);
    signer.sign(TEST_ARCHIVE);

    Informer informer;
    std::stringstream out;
    informer.print(TEST_ARCHIVE, out);
    auto info = out.str();

    ASSERT_THAT(info.c_str(), HasSubstr("Signing CA"));
    ASSERT_THAT(info.c_str(), HasSubstr("Alice"));
}

TEST_F(InformerTest, Fail_UnsignedArchive)
{
    Informer informer;
    
    ASSERT_THROW({
        informer.print(TEST_ARCHIVE);
    }, std::exception);
}

