/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_ARGUMENT_TYPE_HPP
#define CLI_ARGUMENT_TYPE_HPP

namespace cli
{
    enum class ArgumentType
    {
        FLAG,
        STRING,
        LIST
    };
}

#endif
