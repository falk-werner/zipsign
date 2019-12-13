#include <gtest/gtest.h>
#include <stdexcept>
#include <cli/arguments.hpp>

using cli::Arguments;

TEST(Arguments, Contains)
{
    Arguments args;
    args.set('c', "");

    ASSERT_TRUE(args.contains('c'));
    ASSERT_FALSE(args.contains('n'));
}

TEST(Arguments, GetAndSet)
{
    Arguments args;
    
    ASSERT_THROW({
        args.get('c');
    }, std::exception);

    args.set('c', "contained");
    ASSERT_STREQ("contained", args.get('c').c_str());
}