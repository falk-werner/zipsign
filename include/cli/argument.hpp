/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_ARGUMENT_HPP
#define CLI_ARGUMENT_HPP

#include <string>

namespace cli
{

class Argument
{
public:
    Argument(char id_, std::string const & name_);
    ~Argument();
    Argument & setHelpText(std::string const & value);
    Argument & setDefaultValue(std::string const & value);
    Argument & setOptional(bool value = true);
    Argument & setFlag(bool value = true);
    char getId() const;
    std::string const & getName() const;
    std::string const & getHelpText() const;
    std::string const & getDefaultValue() const;
    bool isOptional() const;
    bool isFlag() const;
    bool hasDefaultValue() const;
private:
    char id;
    std::string name;
    std::string helpText;
    std::string defaultValue;
    bool is_optional;
    bool is_flag;
    bool has_defaultValue;
};

}

#endif
