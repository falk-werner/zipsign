#include <gtest/gtest.h>
#include <openssl++/certificate_store.hpp>
#include <openssl++/certificate.hpp>
#include <openssl++/exception.hpp>

using openssl::CertificateStore;
using openssl::Certificate;
using openssl::OpenSSLException;

TEST(CertifiaceStore, Construct)
{
    CertificateStore store;
}

TEST(CertificateStore, AddCert)
{
    Certificate cert = Certificate::fromPEM("certs/alice.crt");

    CertificateStore store;
    store.add(cert);
}

TEST(CertificateStore, Fail_AddCertInvalid)
{
    CertificateStore store;

    ASSERT_THROW({
        store.add(nullptr);
    }, OpenSSLException);
}

TEST(CertificateStore, LoadFromFile)
{
    CertificateStore store;
    store.loadFromFile("keyring.pem");
}

TEST(CertificateStore, Fail_LoadFromFile_InvalidFile)
{
    CertificateStore store;

    ASSERT_THROW({
        store.loadFromFile("acerts/alice.csr");
    }, OpenSSLException);
}

TEST(CertificateStore, MoveConstructible)
{
    CertificateStore store;
    CertificateStore other = std::move(store);

    ASSERT_EQ(nullptr, static_cast<X509_STORE*>(store));
    ASSERT_NE(nullptr, static_cast<X509_STORE*>(other));
}

TEST(CertificateStore, MoveAssignable)
{
    CertificateStore store;
    CertificateStore other;
    other = std::move(store);

    ASSERT_EQ(nullptr, static_cast<X509_STORE*>(store));
    ASSERT_NE(nullptr, static_cast<X509_STORE*>(other));
}
