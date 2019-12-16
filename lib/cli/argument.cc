/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cli/argument.hpp"

namespace cli
{

Argument::Argument(
        char id_,
        std::string const & name_,
        ArgumentType type_,
        std::string const & helpText_,
        bool isRequired_,
        std::string const & defaultValue_)
: id(id_)
, name(name_)
, type(type_)
, helpText(helpText_)
, isRequired(isRequired_)
, defaultValue(defaultValue_)
{

}

Argument::~Argument()
{

}


char Argument::getId() const
{
    return id;
}

std::string const & Argument::getName() const
{
    return name;
}

std::string const & Argument::getHelpText() const
{
    return helpText;
}

std::string const & Argument::getDefaultValue() const
{
    return defaultValue;
}

bool Argument::isOptional() const
{
    return !isRequired;
}

bool Argument::isFlag() const
{
    return (ArgumentType::FLAG == type);
}

bool Argument::hasDefaultValue() const
{
    return !defaultValue.empty();
}


}