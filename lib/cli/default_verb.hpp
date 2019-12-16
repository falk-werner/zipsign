#ifndef CLI_DEFAULT_VERB_HPP
#define CLI_DEFAULT_VERB_HPP

#include "cli/verb.hpp"
#include "cli/app_info.hpp"
#include "cli/argument.hpp"
#include "cli/command.hpp"

namespace cli
{

class DefaultVerb: public Verb
{
public:
    DefaultVerb(AppInfo & appInfo_, std::string const & name, Command command);
    ~DefaultVerb() override;
    Verb & setHelpText(std::string const & helpText_) override;
    virtual Verb & addFlag(char id, std::string const & name, std::string const & description) override;
    virtual Verb & addArg(
        char id,
        std::string const & name,
        std::string const & description,
        bool isRequired,
        std::string const & defaultValue) override;
    virtual Verb & addList(
        char id,
        std::string const & name,
        std::string const & description,
        bool isRequires,
        std::string const & defaultValue) override;

    std::string const & getName() const;
    std::string const & getHelpText() const;
    int run(int argc, char * argv[]) const;
private:
    void printUsage() const;

    AppInfo & appInfo;
    std::string name;
    Command command;
    std::vector<Argument> args;
    std::string helpText;
};

}

#endif
