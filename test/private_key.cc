#include <gtest/gtest.h>
#include "openssl++/openssl++.hpp"

using openssl::PrivateKey;

TEST(PrivateKey, GenerateRSA)
{
    PrivateKey key = PrivateKey::generateRSA();
}