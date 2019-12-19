/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <gtest/gtest.h>
#include <cli/argument.hpp>

using cli::Argument;
using cli::ArgumentType;

TEST(Argument, Id)
{
    Argument arg('a', "argument", ArgumentType::STRING, "", true, "");

    ASSERT_EQ('a', arg.getId());
}

TEST(Argument, Name)
{
    Argument arg('a', "argument", ArgumentType::STRING, "", true, "");

    ASSERT_STREQ("argument", arg.getName().c_str());
}

TEST(Argument, HelpText)
{
    Argument arg('a', "argument", ArgumentType::STRING, "Description", true, "");

    ASSERT_STREQ("Description", arg.getHelpText().c_str());
}

TEST(Argument, DefaultValue)
{
    Argument arg('a', "argument", ArgumentType::STRING, "", true, "Value");

    ASSERT_STREQ("Value", arg.getDefaultValue().c_str());
    ASSERT_TRUE(arg.hasDefaultValue());
}

TEST(DefaultArgument, IsOptionalTrue)
{
    Argument arg('a', "argument", ArgumentType::STRING, "", false, "");

    ASSERT_TRUE(arg.isOptional());
}

TEST(Argument, IsOptionalFalse)
{
    Argument arg('a', "argument", ArgumentType::STRING, "", true, "");

    ASSERT_FALSE(arg.isOptional());
}

TEST(Argument, ArgumentTypeFlag)
{
    Argument arg('a', "argument", ArgumentType::FLAG, "", false, "");

    ASSERT_TRUE(arg.isFlag());
}

TEST(Argument, IsFlagFalse)
{
    Argument arg('a', "argument", ArgumentType::STRING, "", true, "");

    ASSERT_FALSE(arg.isFlag());
}
