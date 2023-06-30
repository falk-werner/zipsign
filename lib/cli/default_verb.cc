/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cli/default_verb.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "cli/options.hpp"
#include "cli/default_arguments.hpp"

namespace cli
{

DefaultVerb::DefaultVerb(AppInfo & appInfo_, std::string const & name_, Command command_)
: appInfo(appInfo_)
, name(name_)
, command(command_)
{

}

DefaultVerb::~DefaultVerb()
{

}

std::string const & DefaultVerb::getName() const
{
    return name;
}

std::string const & DefaultVerb::getHelpText() const
{
    return helpText;
}

Verb & DefaultVerb::setHelpText(std::string const & helpText_)
{
    helpText = helpText_;
    return *this;
}

Verb & DefaultVerb::addFlag(char id, std::string const & name, std::string const & description)
{
    args.push_back(Argument(id, name, ArgumentType::FLAG, description, false, ""));
    return *this;
}

Verb & DefaultVerb::addArg(
    char id,
    std::string const & name,
    std::string const & description,
    bool isRequired,
    std::string const & defaultValue)
{
    (void) defaultValue;
    args.push_back(Argument(id, name, ArgumentType::STRING, description, isRequired, ""));
    return *this;
}

Verb & DefaultVerb::addList(
    char id,
    std::string const & name,
    std::string const & description,
    bool isRequired,
    std::string const & defaultValue)
{
    (void) defaultValue;
    args.push_back(Argument(id, name, ArgumentType::LIST, description, isRequired, ""));
    return *this;
}

int DefaultVerb::run(int argc, char* argv[]) const
{
    DefaultArguments arguments;

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
    optind = 0;

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
        result = command(arguments);
    }
    else
    {
        printUsage();
    }

    return result;
}


void DefaultVerb::printUsage() const
{
    std::cout
        << appInfo.getName() << ", Copyright (c) " << appInfo.getCopyright() << std::endl
        << appInfo.getDescription() << std::endl
        << std::endl
        ;
    
    std::cout << name << ": " << helpText << std::endl << std::endl;

    std::cout
        << "Usage:" << std::endl
        << '\t' << appInfo.getName() << ' ' << name
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
}

}