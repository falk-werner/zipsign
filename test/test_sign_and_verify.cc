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

TEST_F(SignAndVerifyTest, PkiSigned)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";
    std::string keyring = "keyring.pem";

    Verifier verifier(cert_file);
    ASSERT_FALSE(verifier.verify(TEST_ARCHIVE, keyring));

    Signer signer(key_file, cert_file);
    signer.sign(TEST_ARCHIVE);

    ASSERT_TRUE(verifier.verify(TEST_ARCHIVE, keyring));
}

TEST_F(SignAndVerifyTest, UseIntermediateCert)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";
    std::string keyring = "ca/root-ca.crt";
    std::string intermediate = "ca/signing-ca.crt";

    Signer signer(key_file, cert_file);
    signer.addIntermediate(intermediate);
    signer.sign(TEST_ARCHIVE);

    Verifier verifier(cert_file);
    ASSERT_TRUE(verifier.verify(TEST_ARCHIVE, keyring));
}

TEST_F(SignAndVerifyTest, Fail_ValidateWithoutIntermediateCert)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";
    std::string keyring = "ca/root-ca.crt";

    Signer signer(key_file, cert_file);
    signer.sign(TEST_ARCHIVE);

    Verifier verifier(cert_file);
    ASSERT_FALSE(verifier.verify(TEST_ARCHIVE, keyring));
}

TEST_F(SignAndVerifyTest, Multisign)
{
    std::string self_key = "self-signed/key.pem";
    std::string self_crt = "self-signed/cert.pem";
    std::string alice_key = "certs/alice.key";
    std::string alice_crt = "certs/alice.crt";
    std::string keyring = "keyring.pem";

    Signer signer(self_key, self_crt);
    signer.addSigner(alice_key, alice_crt);
    signer.setEmbedCerts();
    signer.sign(TEST_ARCHIVE);

    Verifier self_verifier(self_crt);
    ASSERT_TRUE(self_verifier.verify(TEST_ARCHIVE, ""));

    Verifier alice_verifier(alice_crt);
    ASSERT_TRUE(alice_verifier.verify(TEST_ARCHIVE, keyring));

    Verifier all_verifier(alice_crt);
    all_verifier.addCertificate(self_crt);
    ASSERT_TRUE(all_verifier.verify(TEST_ARCHIVE, keyring));
}
