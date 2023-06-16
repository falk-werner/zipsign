/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "testutils.hpp"

#include <zipsign/zipsign.hpp>
#include <zipsign/file.hpp>

#include <gtest/gtest.h>

using zipsign::Signer;
using zipsign::Verifier;
using zipsign::File;

#define TEST_ARCHIVE ("sign_and_verify.zip")

class SignAndVerifyTest: public ::testing::Test
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

TEST_F(SignAndVerifyTest, SelfSigned)
{
    std::string key_file = "self-signed/key.pem";
    std::string cert_file = "self-signed/cert.pem";

    Verifier verifier(cert_file);
    ASSERT_EQ(Verifier::BadMissingSignature, verifier.verify(TEST_ARCHIVE, ""));

    Signer signer(key_file, cert_file);
    signer.sign(TEST_ARCHIVE);

    ASSERT_EQ(Verifier::Good, verifier.verify(TEST_ARCHIVE, "", false, true));
}

TEST_F(SignAndVerifyTest, PkiSigned)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";
    std::string keyring = "keyring.pem";

    Verifier verifier(cert_file);
    ASSERT_EQ(Verifier::BadMissingSignature, verifier.verify(TEST_ARCHIVE, ""));

    Signer signer(key_file, cert_file);
    signer.sign(TEST_ARCHIVE);

    ASSERT_EQ(Verifier::Good, verifier.verify(TEST_ARCHIVE, keyring));
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
    ASSERT_EQ(Verifier::Good, verifier.verify(TEST_ARCHIVE, keyring));
}

TEST_F(SignAndVerifyTest, Fail_ValidateWithoutIntermediateCert)
{
    std::string key_file = "certs/alice.key";
    std::string cert_file = "certs/alice.crt";
    std::string keyring = "ca/root-ca.crt";

    Signer signer(key_file, cert_file);
    signer.sign(TEST_ARCHIVE);

    Verifier verifier(cert_file);
    ASSERT_EQ(Verifier::BadInvalidCertificateChain, verifier.verify(TEST_ARCHIVE, keyring));
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
    ASSERT_EQ(Verifier::Good, self_verifier.verify(TEST_ARCHIVE, ""));

    Verifier alice_verifier(alice_crt);
    ASSERT_EQ(Verifier::Good, alice_verifier.verify(TEST_ARCHIVE, keyring));

    Verifier all_verifier(alice_crt);
    all_verifier.addCertificate(self_crt);
    ASSERT_EQ(Verifier::Good, all_verifier.verify(TEST_ARCHIVE, keyring));
}
