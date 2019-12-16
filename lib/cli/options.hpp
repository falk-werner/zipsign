/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_OPTIONS_HPP
#define CLI_OPTIONS_HPP

#include <string>
#include <vector>
#include <memory>

#include "cli/argument.hpp"

extern "C" 
{
#include <getopt.h>
}

namespace cli
{

class Options
{
public:
    explicit Options(std::vector<Argument> const & args);
    ~Options();
    char const * shortOpts() const;
    option const * longOpts() const;
private:
    option * long_opts;
    std::string short_opts;
};

}


#endif