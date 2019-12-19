/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cli/arguments.hpp"
#include <unordered_map>

namespace cli
{

class DefaultArguments: public Arguments
{
public:
    DefaultArguments();
    ~DefaultArguments() override;
    bool contains(char id) const override;
    std::string const & get(char id) const override;
    std::vector<std::string> const & getList(char id) const override;
    void set(char id, std::string const & value);
private:
    std::unordered_map<char, std::vector<std::string>> values;    

};

}