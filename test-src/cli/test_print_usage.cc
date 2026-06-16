#include "zipsign/main.hpp"
#include <sstream>
#include <gtest/gtest.h>

TEST(cli, print_usage)
{
    char arg0[] = "zipsign";
    char arg1[] = "-h";
    char * argv[] = { arg0, arg1, nullptr };
    int argc = 2;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(0, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_TRUE(err.str().empty());
}

TEST(cli, sign_print_usage)
{
    char arg0[] = "zipsign";
    char arg1[] = "sign";
    char arg2[] = "--help";
    char * argv[] = { arg0, arg1, arg2, nullptr };
    int argc = 3;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(0, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_TRUE(err.str().empty());
}

TEST(cli, sign_print_usage_short)
{
    char arg0[] = "zipsign";
    char arg1[] = "sign";
    char arg2[] = "-h";
    char * argv[] = { arg0, arg1, arg2, nullptr };
    int argc = 3;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(0, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_TRUE(err.str().empty());
}

TEST(cli, verify_print_usage)
{
    char arg0[] = "zipsign";
    char arg1[] = "verify";
    char arg2[] = "--help";
    char * argv[] = { arg0, arg1, arg2, nullptr };
    int argc = 3;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(0, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_TRUE(err.str().empty());
}

TEST(cli, verify_print_usage_short)
{
    char arg0[] = "zipsign";
    char arg1[] = "verify";
    char arg2[] = "-h";
    char * argv[] = { arg0, arg1, arg2, nullptr };
    int argc = 3;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(0, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_TRUE(err.str().empty());
}

TEST(cli, info_print_usage)
{
    char arg0[] = "zipsign";
    char arg1[] = "info";
    char arg2[] = "--help";
    char * argv[] = { arg0, arg1, arg2, nullptr };
    int argc = 3;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(0, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_TRUE(err.str().empty());
}

TEST(cli, info_print_usage_short)
{
    char arg0[] = "zipsign";
    char arg1[] = "info";
    char arg2[] = "-h";
    char * argv[] = { arg0, arg1, arg2, nullptr };
    int argc = 3;
    std::stringstream out;
    std::stringstream err;
    int const result = zipsign::main(argc, argv, out, err);

    ASSERT_EQ(0, result);
    ASSERT_FALSE(out.str().empty());
    ASSERT_TRUE(err.str().empty());
}
