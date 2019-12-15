#ifndef CLI_VALUE_HPP
#define CLI_VALUE_HPP

#include <string>
#include <vector>

namespace cli
{

class Value
{
public:
    virtual ~Value() { }
    virtual void set(std::string& value) = 0;
    virtual bool getAsBool() = 0;
    virtual std::string const & getAsString() = 0;
    virtual std::vector<std::string> const & getAsList() = 0;
};

}

#endif
