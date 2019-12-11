#include <gtest/gtest.h>
#include <zipsign/zipsign.hpp>
#include <zipsign/file.hpp>

using zipsign::Signer;
using zipsign::Verifier;
using zipsign::File;

#define TEST_ARCHIVE ("sign_and_verify.zip")

class SignAndVerifyTest: public ::testing::Test
{
protected:
    void SetUp() override
    {
        File archive("test.zip", "rb");
        File test_file(TEST_ARCHIVE, "wb");

        archive.copyTo(test_file);
    }

    void TearDown() override
    {
        File::remove(TEST_ARCHIVE);
    }
};

TEST_F(SignAndVerifyTest, SelfSigned)
{
    std::string key_file = "self-signed/key.pem";
    std::string cert_file = "self-signed/cert.pem";

    Verifier verifier(cert_file);
    ASSERT_FALSE(verifier.verify(TEST_ARCHIVE, ""));

    Signer signer(key_file, cert_file);
    signer.sign(TEST_ARCHIVE);

    ASSERT_TRUE(verifier.verify(TEST_ARCHIVE, ""));
}