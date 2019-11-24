#ifndef CLI_APP_INFO_HPP
#define CLI_APP_INFO_HPP

#include <string>

namespace cli
{

class AppInfo
{
public:
    virtual ~AppInfo() { }
    virtual std::string const & getName() const = 0;
    virtual std::string const & getDescription() const = 0;
    virtual std::string const & getCopyright() const = 0;
};

}

#endif
