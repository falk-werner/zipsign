/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cli/default_arguments.hpp"
#include <stdexcept>

namespace cli
{

DefaultArguments::DefaultArguments()
{

}

DefaultArguments::~DefaultArguments()
{

}

bool DefaultArguments::contains(char id) const
{
    return values.end() != values.find(id);
}

std::string const & DefaultArguments::get(char id) const
{
    auto it = values.find(id);
    if (values.end() == it)
    {
        throw  std::out_of_range("id not found : " + id);
    }

    return it->second;
}

std::vector<std::string> const & DefaultArguments::getList(char id) const
{
    throw std::out_of_range("id not found: " + id);
}


void DefaultArguments::set(char id, std::string const & value)
{
    values[id] = value;
}

}