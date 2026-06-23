#include <gtest/gtest.h>
#include <openssl++/openssl++.hpp>
#include <zipsign/base64.hpp>

#include <sstream>

using openssl::CMS;
using openssl::PrivateKey;
using openssl::Certificate;
using openssl::CertificateStore;
using openssl::CertificateStack;
using openssl::BasicIO;
using openssl::OpenSSLException;

TEST(CMS, SignAndVerify)
{
    PrivateKey key = PrivateKey::fromPEM("self-signed/key.pem");
    Certificate cert = Certificate::fromPEM("self-signed/cert.pem");
    BasicIO file = BasicIO::openInputFile("test.zip");

    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);

    CertificateStore store;
    store.add(cert);
    
    CertificateStack certs;
    certs.push(cert);

    file = BasicIO::openInputFile("test.zip");
    
    std::stringstream err;
    bool isValid = cms.verify(certs, store, file, nullptr,  CMS_DETACHED | CMS_BINARY, err, false);
    ASSERT_TRUE(isValid);
    ASSERT_TRUE(err.str().empty());
}

TEST(CMS, FailedToSign)
{
    ASSERT_THROW({
        CMS::sign(nullptr, nullptr, nullptr, nullptr, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);
    }, OpenSSLException);
}

TEST(CMS, FailedToVerify)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    Certificate cert = Certificate::fromPEM("certs/alice.crt");
    BasicIO file = BasicIO::openInputFile("test.zip");

    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);

    cert = Certificate::fromPEM("self-signed/cert.pem");
    CertificateStore store;
    store.add(cert);
    
    CertificateStack certs;
    certs.push(cert);

    file = BasicIO::openInputFile("test.zip");
    
    std::stringstream err;
    bool isValid = cms.verify(certs, store, file, nullptr,  CMS_DETACHED | CMS_BINARY, err, true);
    ASSERT_FALSE(isValid);
    ASSERT_FALSE(err.str().empty());
}

TEST(CMS, SaveToBIO)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    Certificate cert = Certificate::fromPEM("certs/alice.crt");
    BasicIO file = BasicIO::openInputFile("test.zip");

    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);

    BasicIO bio = BasicIO::fromMemory();
    cms.saveToBIO(bio);
}

TEST(CMS, FailedToSaveToBIO)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    Certificate cert = Certificate::fromPEM("certs/alice.crt");
    BasicIO file = BasicIO::openInputFile("test.zip");

    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);

    ASSERT_THROW({
        cms.saveToBIO(nullptr);
    }, OpenSSLException);
}

TEST(CMS, FromBase64)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    Certificate cert = Certificate::fromPEM("certs/alice.crt");
    BasicIO file = BasicIO::openInputFile("test.zip");

    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);
    std::string data = cms.toBase64();

    cms.fromBase64(data);
}

TEST(CMS, FailFromBase64InvalidContent)
{
    char text[] = "Hugo";
    size_t text_size = sizeof(text) - 1;
    std::string data(zipsign::b64_encode((uint8_t const*) text, text_size));

    ASSERT_THROW({
        CMS::fromBase64(data);
    }, OpenSSLException);
}

TEST(CMS, CastToContentInfo)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    Certificate cert = Certificate::fromPEM("certs/alice.crt");
    BasicIO file = BasicIO::openInputFile("test.zip");

    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);
    CMS_ContentInfo * contentInfo = static_cast<CMS_ContentInfo*>(cms);
    (void) contentInfo;
}

TEST(CMS, MoveConstructible)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    Certificate cert = Certificate::fromPEM("certs/alice.crt");
    BasicIO file = BasicIO::openInputFile("test.zip");

    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);
    CMS other = std::move(cms);
}

TEST(CMS, MoveAssignable)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    Certificate cert = Certificate::fromPEM("certs/alice.crt");

    BasicIO file = BasicIO::openInputFile("test.zip");
    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);

    file = BasicIO::openInputFile("test.zip");
    CMS other = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);
    other = std::move(cms);
}

TEST(CMS, SelfMoveAssignment)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    Certificate cert = Certificate::fromPEM("certs/alice.crt");

    BasicIO file = BasicIO::openInputFile("test.zip");
    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);

    CMS &ref = cms;
    cms = std::move(ref);

    ASSERT_NE(nullptr, static_cast<CMS_ContentInfo*>(cms));
}

TEST(CMS, AddInvalidSigner)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    Certificate cert = Certificate::fromPEM("certs/alice.crt");

    BasicIO file = BasicIO::openInputFile("test.zip");
    CMS cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);

    ASSERT_THROW({
        cms.addSigner(nullptr, nullptr, nullptr, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);
    }, OpenSSLException);
}