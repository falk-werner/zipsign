/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_ARGUMENTS_HPP
#define CLI_ARGUMENTS_HPP

#include <string>
#include <unordered_map>
namespace cli
{

class Arguments
{
public:
    Arguments();
    ~ Arguments();
    bool contains(char id) const;
    std::string const & get(char id) const;
    void set(char id, std::string const & value);
private:
    std::unordered_map<char, std::string> values;    
};

}

#endif
