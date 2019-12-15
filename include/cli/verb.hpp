#ifndef CLI_VERB_HPP
#define CLI_VERB_HPP

#include <string>
#include <vector>
#include <cli/argument.hpp>
#include <cli/command.hpp>

namespace cli
{

class Verb
{
public:
    virtual ~Verb() { }
    virtual Verb & setHelpText(std::string const & helpText_) = 0;
    virtual Verb & add(Argument arg) = 0;
};


}

#endif
