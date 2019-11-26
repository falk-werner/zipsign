/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cli/app.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>

namespace cli
{

App::App(std::string const & name_)
: name(name_)
{

}
App::~App()
{

}   

std::string const & App::getName() const
{
    return name;
}

std::string const & App::getDescription() const
{
    return description;
}

std::string const & App::getCopyright() const
{
    return copyright;
}

int App::run(int argc, char* argv[]) const
{
    int exitCode = EXIT_FAILURE;

    if (argc > 1)
    {
        std::string verbName = argv[1];
        Verb const * verb = getVerb(verbName);

        if (nullptr != verb)
        {
            exitCode = verb->run(argc - 1, &argv[1]);
        }
        else if ((verbName == "-h") || (verbName == "--help"))
        {
            printUsage();
            exitCode = EXIT_SUCCESS;
        }
        else
        {
            std::cerr << "error: unknwon verb: " << verbName << std::endl;
            printUsage();
        }
    }
    else
    {
        std::cerr << "error: missing verb" << std::endl;        
        printUsage();
    }

    return exitCode;
}

App & App::add(Verb const & verb)
{
    verbs.push_back(verb);
    verbs[verbs.size() - 1].setApp(*this);

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

Verb const * App::getVerb(std::string const & name) const
{
    for(auto & verb: verbs)
    {
        if (name == verb.getName())
        {
            return &verb;
        }
    }

    return nullptr;
}


void App::printUsage(void) const
{
    std::cout
        << name << ", Copyright (c) " << copyright << std::endl
        << description << std::endl
        << std::endl
        << "Usage:" << std::endl
        << '\t' << name << ' '
    ;

    for (auto const & verb: verbs)
    {
        std::cout << verb.getName() << " <args...> | ";
    }

    std::cout 
        << "-h" << std::endl
        << std::endl
        << "Verbs:" << std::endl;

    for (auto const & verb: verbs)
    {
        std::cout 
            << "\t" << std::left << std::setw(20) << verb.getName()
            << "\t" << verb.getHelpText()
            << std::endl;
    }

    std::cout
        << "\t" << std::left << std::setw(20) << "-h, --help" << "\tPrint usage." << std::endl
        << std::endl;
    
    std::cout << additionalInfo;

}

}
