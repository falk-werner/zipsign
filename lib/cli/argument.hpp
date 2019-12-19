/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_DEFAULT_ARGUMENT_HPP
#define CLI_DEFAULT_ARGUMENT_HPP

#include <string>
#include "cli/argument_type.hpp"

namespace cli
{

class Argument
{
public:
    Argument(
        char id_,
        std::string const & name_,
        ArgumentType type_,
        std::string const & helpText_,
        bool isRequired_,
        std::string const & defaultValue_);
    ~Argument();
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
    ArgumentType type;
    std::string helpText;
    bool isRequired;
    std::string defaultValue;
};

}

#endif
