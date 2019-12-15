/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cli/app.hpp"

#include "cli/app_info.hpp"
#include "cli/default_verb.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>

namespace cli
{

class App::Private: public AppInfo
{
public:
    Private(std::string const & name_);    
    ~Private() override;    
    std::string const & getName() const override;
    std::string const & getDescription() const override;
    std::string const & getCopyright() const override;
    int run(int argc, char* argv[]) const;
    Verb & add(std::string const & name, Command command);
    void setCopyright(std::string const & value);
    void setDescription(std::string const & value);
    void setAdditionalInfo(std::string const & value);
private:
    void printUsage() const;
    DefaultVerb const * getVerb(std::string const & name) const;
    std::string name;
    std::string copyright;
    std::string description;
    std::string additionalInfo;
    std::vector<DefaultVerb> verbs;

};

App::App(std::string const & name_)
: d(new App::Private(name_))
{

}
App::~App()
{
    delete d;
}   

int App::run(int argc, char* argv[]) const
{
    return d->run(argc, argv);
}

Verb & App::add(std::string const & name, Command command)
{
    return d->add(name, command);
}

App & App::setCopyright(std::string const & value)
{
    d->setCopyright(value);
    return *this;
}
App & App::setDescription(std::string const & value)
{
    d->setDescription(value);
    return *this;
}

App & App::setAdditionalInfo(std::string const & value)
{
    d->setAdditionalInfo(value);
    return *this;
}


App::Private::Private(std::string const & name_)
: name(name_)
{

}
App::Private::~Private()
{

}   

std::string const & App::Private::getName() const
{
    return name;
}

std::string const & App::Private::getDescription() const
{
    return description;
}

std::string const & App::Private::getCopyright() const
{
    return copyright;
}

int App::Private::run(int argc, char* argv[]) const
{
    int exitCode = EXIT_FAILURE;

    if (argc > 1)
    {
        std::string verbName = argv[1];
        DefaultVerb const * verb = getVerb(verbName);

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

Verb & App::Private::add(std::string const & verb, Command command)
{    
    verbs.push_back(DefaultVerb(*this, verb, command));

    return verbs[verbs.size() - 1];
}

void App::Private::setCopyright(std::string const & value)
{
    copyright = value;
}

void App::Private::setDescription(std::string const & value)
{
    description = value;
}

void App::Private::setAdditionalInfo(std::string const & value)
{
    additionalInfo = value;
}

DefaultVerb const * App::Private::getVerb(std::string const & name) const
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


void App::Private::printUsage(void) const
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
