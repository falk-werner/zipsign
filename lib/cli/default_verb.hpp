#ifndef CLI_DEFAULT_VERB_HPP
#define CLI_DEFAULT_VERB_HPP

#include "cli/verb.hpp"
#include "cli/app_info.hpp"

namespace cli
{

class DefaultVerb: public Verb
{
public:
    DefaultVerb(AppInfo & appInfo_, std::string const & name, Command command);
    ~DefaultVerb() override;
    Verb & add(Argument arg) override;
    Verb & setHelpText(std::string const & helpText_) override;

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
