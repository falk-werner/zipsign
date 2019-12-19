/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include <openssl++/certificate.hpp>
#include <openssl++/exception.hpp>

using openssl::Certificate;
using openssl::OpenSSLException;

TEST(Certificate, FromPEM)
{
    Certificate::fromPEM("certs/alice.crt");
}

TEST(Certificate, Fail_FromPEMInvalidFile)
{
    ASSERT_THROW({
        Certificate::fromPEM("certs/alice.csr");
    }, OpenSSLException);
}


TEST(Certificate, MoveConstructible)
{
    Certificate cert = Certificate::fromPEM("certs/alice.crt");
    Certificate other = std::move(cert);

    ASSERT_EQ(nullptr, static_cast<X509*>(cert));
    ASSERT_NE(nullptr, static_cast<X509*>(other));
}

TEST(Certificate, MoveAssignable)
{
    Certificate cert = Certificate::fromPEM("certs/alice.crt");
    Certificate other = Certificate::fromPEM("certs/alice.crt");
    other = std::move(cert);

    ASSERT_EQ(nullptr, static_cast<X509*>(cert));
    ASSERT_NE(nullptr, static_cast<X509*>(other));
}
