/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_COMMAND_HPP
#define CLI_COMMAND_HPP

#include <functional>
#include <cli/arguments.hpp>

namespace cli
{

using Command = std::function<int (Arguments const &)>;

}

#endif
