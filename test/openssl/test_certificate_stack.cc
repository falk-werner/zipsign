/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
