#include <gtest/gtest.h>
#include <cli/options.hpp>

using cli::Options;
using cli::Argument;

TEST(Options, ShortOpts)
{
    std::vector<Argument> args;
    args.push_back(Argument('f', "file"));
    args.push_back(Argument('v', "verbose").setFlag());

    Options opts(args);
    ASSERT_STREQ("f:vh", opts.shortOpts());
}

TEST(Options, LongOpts)
{
    std::vector<Argument> args;
    args.push_back(Argument('f', "file"));
    args.push_back(Argument('v', "verbose").setFlag());

    Options opts(args);
    option const * long_opts = opts.longOpts();

    // file
    ASSERT_STREQ("file", long_opts[0].name);
    ASSERT_EQ(nullptr, long_opts[0].flag);
    ASSERT_EQ('f', long_opts[0].val);
    ASSERT_EQ(required_argument, long_opts[0].has_arg);

    // verbose
    ASSERT_STREQ("verbose", long_opts[1].name);
    ASSERT_EQ(nullptr, long_opts[1].flag);
    ASSERT_EQ('v', long_opts[1].val);
    ASSERT_EQ(no_argument, long_opts[1].has_arg);

    // help
    ASSERT_STREQ("help", long_opts[2].name);
    ASSERT_EQ(nullptr, long_opts[2].flag);
    ASSERT_EQ('h', long_opts[2].val);
    ASSERT_EQ(no_argument, long_opts[2].has_arg);

    // end of options
    ASSERT_EQ(nullptr, long_opts[3].name);
    ASSERT_EQ(nullptr, long_opts[3].flag);
    ASSERT_EQ(0, long_opts[3].val);
    ASSERT_EQ(0, long_opts[3].has_arg);
}