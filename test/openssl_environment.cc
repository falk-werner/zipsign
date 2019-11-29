/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
