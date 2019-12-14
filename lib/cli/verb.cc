#include "cli/verb.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "cli/options.hpp"

namespace cli
{

Verb::Verb(std::string const & name_, Command command_)
: name(name_)
, command(command_)
, appInfo(nullptr)
{

}

Verb::~Verb()
{

}

std::string const & Verb::getName() const
{
    return name;
}

std::string const & Verb::getHelpText() const
{
    return helpText;
}

void Verb::setApp(AppInfo & appInfo_)
{
    appInfo = &appInfo_;
}

Verb & Verb::setHelpText(std::string const & helpText_)
{
    helpText = helpText_;
    return *this;
}

Verb & Verb::add(Argument arg)
{
    args.push_back(arg);
    return *this;
}

int Verb::run(int argc, char* argv[]) const
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


void Verb::printUsage() const
{
    if (nullptr != appInfo)
    {
        std::cout
            << appInfo->getName() << ", Copyright (c) " << appInfo->getCopyright() << std::endl
            << appInfo->getDescription() << std::endl
            << std::endl
            ;
    }
    
    std::cout << name << ": " << helpText << std::endl << std::endl;

    std::cout
        << "Usage:" << std::endl
        << '\t' << ((nullptr != appInfo) ? appInfo->getName() : "<app>") << ' ' << name
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