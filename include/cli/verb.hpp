#ifndef CLI_VERB_HPP
#define CLI_VERB_HPP

#include <string>

namespace cli
{

class Verb
{
public:
    virtual ~Verb() { }
    virtual Verb & setHelpText(std::string const & helpText_) = 0;
    virtual Verb & addFlag(
        char id,
        std::string const & name,
        std::string const & description = "") = 0;
    virtual Verb & addArg(
        char id,
        std::string const & name,
        std::string const & description = "",
        bool isRequired = true,
        std::string const & defaultValue = "") = 0;
    virtual Verb & addList(
        char id,
        std::string const & name,
        std::string const & description = "",
        bool isRequired = true,
        std::string const & defaultValue = "") = 0;
};


}

#endif
