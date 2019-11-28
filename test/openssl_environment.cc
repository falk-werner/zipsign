#include <gtest/gtest.h>
#include "openssl++/openssl++.hpp"

namespace
{

class OpennSSLEnvironment: ::testing::Environment
{
public:
    virtual ~OpennSSLEnvironment() { }

    void SetUp() override
    {
        openssl::OpenSSL::init();
    }
};

}
