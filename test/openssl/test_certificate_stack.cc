#include <gtest/gtest.h>
#include <openssl++/certificate_stack.hpp>
#include <openssl++/certificate.hpp>
#include <openssl++/exception.hpp>

using openssl::CertificateStack;
using openssl::Certificate;
using openssl::OpenSSLException;

TEST(CertificateStack, Constructible)
{
    CertificateStack stack;
}

TEST(CertificateStack, AddCertificate)
{
    Certificate cert = Certificate::fromPEM("certs/alice.crt");
    CertificateStack stack;

    stack.push(cert);
}

TEST(CertificateStack, CastToX509Stack)
{
    CertificateStack stack;
    STACK_OF(X509) * certs = static_cast<STACK_OF(X509) *>(stack);
}
