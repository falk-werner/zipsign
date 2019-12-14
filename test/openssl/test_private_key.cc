#include <gtest/gtest.h>
#include <openssl++/private_key.hpp>
#include <openssl++/exception.hpp>

using openssl::PrivateKey;
using openssl::OpenSSLException;

TEST(PrivateKey, FromPEM)
{
    PrivateKey::fromPEM("certs/alice.key");
}

TEST(PrivateKey, Fail_FromPEM)
{
    ASSERT_THROW({
        PrivateKey::fromPEM("certs/alice.csr");
    }, OpenSSLException);
}

TEST(PrivateKey, MoveConstructible)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    PrivateKey other = std::move(key);

    ASSERT_EQ(nullptr, static_cast<EVP_PKEY*>(key));
    ASSERT_NE(nullptr, static_cast<EVP_PKEY*>(other));
}

TEST(PrivateKey, MoveAssingable)
{
    PrivateKey key = PrivateKey::fromPEM("certs/alice.key");
    PrivateKey other = PrivateKey::fromPEM("certs/alice.key");
    other = std::move(key);

    ASSERT_EQ(nullptr, static_cast<EVP_PKEY*>(key));
    ASSERT_NE(nullptr, static_cast<EVP_PKEY*>(other));
}
