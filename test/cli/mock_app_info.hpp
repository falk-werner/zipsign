#include <gmock/gmock.h>
#include <cli/app_info.hpp>

class AppInfoMock: public cli::AppInfo
{
public:
    MOCK_CONST_METHOD0(getName, std::string const & ());
    MOCK_CONST_METHOD0(getDescription, std::string const & ());
    MOCK_CONST_METHOD0(getCopyright, std::string const & ());
};
