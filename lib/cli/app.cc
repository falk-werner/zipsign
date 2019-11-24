/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cli/app.hpp"
#include "cli/options.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>

namespace cli
{

App::App(std::string const & name_, Command defaultCommand_)
: name(name_)
, defaultCommand(defaultCommand_)
{

}
App::~App()
{

}   

int App::run(int argc, char* argv[]) const
{
    Arguments arguments;

    for(auto & arg: args)
    {
        if (arg.hasDefaultValue())
        {
            arguments.set(arg.getId(), arg.getDefaultValue());
        }
    }


    Options opts(args);
    int result = EXIT_SUCCESS;
    bool is_finished = false;
    bool print_usage = false;

    while (!is_finished)
    {
        opterr = 0;
        int option_index = 0;
        int const c = getopt_long(argc, argv, opts.shortOpts(), opts.longOpts(), &option_index);

        switch (c)
        {
            case -1:
                is_finished = true;
                break;
            case 'h':
                is_finished = true;
                print_usage = true;
                break;
            case '?':
                std::cout << "error: unrecognized argument" << std::endl;
                is_finished = true;
                print_usage = true;
                result = EXIT_FAILURE;
                break;
            default:
                arguments.set(c, (nullptr != optarg) ? optarg : "");
                break;
        }
    }

    if ((EXIT_SUCCESS == result) && (!print_usage))
    {
        for (auto & arg: args)
        {
            if ((!arg.isOptional()) && (!arguments.contains(arg.getId())))
            {
                std::cerr << "error: missing required argument: -" << arg.getId() << std::endl;
                print_usage = true;
                result = EXIT_FAILURE;                
            }
        }
    }

    if (!print_usage)
    {
        result = defaultCommand(arguments);
    }
    else
    {
        printUsage();
    }

    return result;
}

App & App::add(Argument const & arg)
{
    args.push_back(arg);
    return *this;
}

App & App::setCopyright(std::string const & value)
{
    copyright = value;
    return *this;
}

App & App::setDescription(std::string const & value)
{
    description = value;
    return *this;
}

App & App::setAdditionalInfo(std::string const & value)
{
    additionalInfo = value;
    return *this;
}

void App::printUsage() const
{
    std::cout
        << name << ", " << copyright << std::endl
        << description << std::endl
        << std::endl
        << "Usage:" << std::endl
        << '\t' << name
    ;

    for (auto const & arg: args)
    {
        std::cout << ' ';
        if (arg.isOptional())
        {
            std::cout << '[';
        }

        std::cout << '-' << arg.getId();
        if (!arg.isFlag())
        {
            std::cout << " <value>";
        }

        if (arg.isOptional())
        {
            std::cout << ']';
        }
    }

    std::cout 
        << " | -h" << std::endl
        << std::endl
        << "Arguments:" << std::endl;

    for (auto const & arg: args)
    {
        std::cout 
            << "\t-" << arg.getId() 
            << ", --" << std::left << std::setw(20) << arg.getName()
            << "\t";
            
        if (!arg.isOptional())
        {
            std::cout << "Required. ";
        }

        std::cout << arg.getHelpText();

        if (arg.hasDefaultValue())
        {
            std::cout << " (default: " << arg.getDefaultValue() << ')';
        }

        std::cout << std::endl;
    }

    std::cout
        << "\t-h, --" << std::left << std::setw(20) << "help" << "\tPrint usage." << std::endl
        << std::endl;
    
    std::cout << additionalInfo;
}

}
