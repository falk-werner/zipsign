/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_APP_INFO_HPP
#define CLI_APP_INFO_HPP

#include <string>

namespace cli
{

class AppInfo
{
public:
    virtual ~AppInfo() { }
    virtual std::string const & getName() const = 0;
    virtual std::string const & getDescription() const = 0;
    virtual std::string const & getCopyright() const = 0;
};

}

#endif
