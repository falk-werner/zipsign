/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include <stdexcept>
#include <cli/default_arguments.hpp>

using cli::DefaultArguments;

TEST(DefaultArguments, Contains)
{
    DefaultArguments args;
    args.set('c', "");

    ASSERT_TRUE(args.contains('c'));
    ASSERT_FALSE(args.contains('n'));
}

TEST(DefaultArguments, GetAndSet)
{
    DefaultArguments args;
    
    ASSERT_THROW({
        args.get('c');
    }, std::exception);

    args.set('c', "contained");
    ASSERT_STREQ("contained", args.get('c').c_str());
}