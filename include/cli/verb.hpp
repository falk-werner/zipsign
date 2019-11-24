#ifndef CLI_VERB_HPP
#define CLI_VERB_HPP

#include <string>
#include <vector>
#include <cli/argument.hpp>
#include <cli/command.hpp>
#include <cli/app_info.hpp>

namespace cli
{

class Verb
{
public:
    Verb(std::string const & name, Command command);
    ~Verb();
    void setApp(AppInfo & appInfo_);
    std::string const & getName() const;
    std::string const & getHelpText() const;
    int run(int argc, char * argv[]) const;
    Verb & add(Argument arg);
    Verb & setHelpText(std::string const & helpText_);
private:
    void printUsage() const;

    std::string name;
    Command command;
    std::vector<Argument> args;
    std::string helpText;
    AppInfo * appInfo;
};


}

#endif
