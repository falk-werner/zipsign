/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cli/options.hpp"

namespace
{

void setOption(
    option & option,
    char id,
    char const * name,
    int hasArg)
{
    option.name = name;
    option.has_arg = hasArg;
    option.flag = nullptr;
    option.val = static_cast<int>(id);
}

void setOption(
    option & option,
    cli::Argument const & arg
)
{
        option.name = arg.getName().c_str();
        option.has_arg = arg.isFlag() ? no_argument : required_argument;
        option.flag = nullptr;
        option.val = static_cast<int>(arg.getId());
}

}

namespace cli
{

Options::Options(std::vector<Argument> const & args)
{
    size_t const length = args.size();
    long_opts = new option[length + 2];
    short_opts = "";

    for(size_t i = 0; i < length; i++)
    {
        setOption(long_opts[i], args[i]);

        short_opts += args[i].getId();
        if (!args[i].isFlag())
        {
            short_opts += ':';
        }
        short_opts += 'h';
    }

    setOption(long_opts[length], 'h', "help", no_argument);
    setOption(long_opts[length + 1], '\0', nullptr, 0);
}

Options::~Options()
{
    delete[] long_opts;
}

char const * Options::shortOpts() const
{
    return short_opts.c_str();
}

option const * Options::longOpts() const
{
    return long_opts;
}

}