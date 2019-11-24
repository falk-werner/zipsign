#ifndef CLI_COMMAND_HPP
#define CLI_COMMAND_HPP

#include <functional>
#include <cli/arguments.hpp>

namespace cli
{

using Command = std::function<int (Arguments const &)>;

}

#endif
