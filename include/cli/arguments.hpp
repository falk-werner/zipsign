/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_ARGUMENTS_HPP
#define CLI_ARGUMENTS_HPP

#include <string>

namespace cli
{

class Arguments
{
public:
    virtual ~Arguments() { }
    virtual bool contains(char id) const = 0;
    virtual std::string const & get(char id) const = 0;
};

}

#endif
