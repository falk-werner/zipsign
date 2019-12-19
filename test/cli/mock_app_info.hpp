/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_MOCK_APP_INFO_HPP
#define CLI_MOCK_APP_INFO_HPP

#include <gmock/gmock.h>
#include <cli/app_info.hpp>

class AppInfoMock: public cli::AppInfo
{
public:
    MOCK_CONST_METHOD0(getName, std::string const & ());
    MOCK_CONST_METHOD0(getDescription, std::string const & ());
    MOCK_CONST_METHOD0(getCopyright, std::string const & ());
};

#endif
