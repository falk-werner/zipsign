/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cli/argument.hpp"

namespace cli
{

Argument::Argument(char id_, std::string const & name_)
: id(id_)
, name(name_)
, is_optional(false)
, is_flag(false)
, has_defaultValue(false)
{

}

Argument::~Argument()
{

}

Argument & Argument::setHelpText(std::string const & value)
{
    helpText = value;
    return *this;
}

Argument & Argument::setDefaultValue(std::string const & value)
{
    defaultValue = value;
    has_defaultValue = true;
    return *this;
}

Argument & Argument::setOptional(bool value)
{
    is_optional = value;
    return *this;
}

Argument & Argument::setFlag(bool value)
{
    is_flag = value;
    return *this;
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
    return is_optional;
}

bool Argument::isFlag() const
{
    return is_flag;
}

bool Argument::hasDefaultValue() const
{
    return has_defaultValue;
}


}