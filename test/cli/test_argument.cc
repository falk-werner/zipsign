#include <gtest/gtest.h>
#include <cli/cli.hpp>

using cli::Argument;

TEST(Argument, Id)
{
    Argument arg('a', "argument");

    ASSERT_EQ('a', arg.getId());
}

TEST(Argument, Name)
{
    Argument arg('a', "argument");

    ASSERT_STREQ("argument", arg.getName().c_str());
}

TEST(Argument, HelpText)
{
    Argument arg('a', "argument");
    arg.setHelpText("Description");

    ASSERT_STREQ("Description", arg.getHelpText().c_str());
}

TEST(Argument, DefaultValue)
{
    Argument arg('a', "argument");
    arg.setDefaultValue("Value");

    ASSERT_STREQ("Value", arg.getDefaultValue().c_str());
    ASSERT_TRUE(arg.hasDefaultValue());
}

TEST(Argument, IsOptionalDefaultOff)
{
    Argument arg('a', "argument");
    ASSERT_FALSE(arg.isOptional());
}

TEST(Argument, IsOptionalTrue)
{
    Argument arg('a', "argument");
    arg.setOptional(true);

    ASSERT_TRUE(arg.isOptional());
}

TEST(Argument, IsOptionalFalse)
{
    Argument arg('a', "argument");
    arg.setOptional(false);

    ASSERT_FALSE(arg.isOptional());
}

TEST(Argument, IsFlagDefaultOff)
{
    Argument arg('a', "argument");

    ASSERT_FALSE(arg.isFlag());
}

TEST(Argument, IsFlagTrue)
{
    Argument arg('a', "argument");
    arg.setFlag(true);

    ASSERT_TRUE(arg.isFlag());
}

TEST(Argument, IsFlagFalse)
{
    Argument arg('a', "argument");
    arg.setFlag(false);

    ASSERT_FALSE(arg.isFlag());
}

TEST(Argument, HasDefaultValueDefaultOff)
{
    Argument arg('a', "argument");
    
    ASSERT_FALSE(arg.hasDefaultValue());
}
