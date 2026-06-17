#include "zipsign/main.hpp"
#include <sstream>
#include <gtest/gtest.h>

TEST(cli_main, empty_args)
{
    char arg0[] = "zipsign";
    char * argv[] = { arg0, nullptr };
    int argc = 1;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(EXIT_FAILURE, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_FALSE(err.str().empty());
}

TEST(cli_main, unknown_verb)
{
    char arg0[] = "zipsign";
    char arg1[] = "unknown";
    char * argv[] = { arg0, arg1, nullptr };
    int argc = 2;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    auto err_str = err.str();
    ASSERT_EQ(EXIT_FAILURE, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_FALSE(err_str.empty());
    ASSERT_EQ(0, err_str.find("error: unknown verb")) << err_str;
}