#include <gtest/gtest.h>
#include <cli/cli.hpp>

#include <cstdlib>

using cli::App;
using cli::Verb;
using cli::Argument;
using cli::Arguments;

class AppTest: public ::testing::Test
{
protected:
    void SetUp() override
    {
        app = new App("SimpleApp");
        app->setCopyright("(c) 2019 Falk Werner");
        app->setDescription("a simple app");
        app->setAdditionalInfo("<FOOTER>");
        app->add("run", AppTest::Run);
    }

    void TearDown() override
    {
        delete app;
    }

    static int Run(Arguments const & args)
    {
        return EXIT_SUCCESS;
    }

    App * app;
};

TEST_F(AppTest, RunSuccess)
{
    char arg0[] = "app";
    char arg1[] = "run";
    char * argv[] = {arg0, arg1, nullptr};
    int exitCode = app->run(2, argv);

    ASSERT_EQ(EXIT_SUCCESS, exitCode);
}

TEST_F(AppTest, PrintHelp)
{
    char arg0[] = "app";
    char arg1[] = "--help";
    char * argv[] = {arg0, arg1, nullptr};
    int exitCode = app->run(2, argv);
    ASSERT_EQ(EXIT_SUCCESS, exitCode);
}

TEST_F(AppTest, PrintHelpForVerb)
{
    char arg0[] = "app";
    char arg1[] = "run";
    char arg2[] = "--help";
    char * argv[] = {arg0, arg1, arg2, nullptr}; 
    int exitCode = app->run(3, argv);
    ASSERT_EQ(EXIT_SUCCESS, exitCode);
}

TEST_F(AppTest, Fail_MissingVerb)
{
    int exitCode = app->run(0, nullptr);
    ASSERT_EQ(EXIT_FAILURE, exitCode);
}

TEST_F(AppTest, Fail_InvalidVerb)
{
    char arg0[] = "app";
    char arg1[] = "invalid";
    char * argv[] = { arg0, arg1, nullptr };
    int exitCode = app->run(2, argv);
    ASSERT_EQ(EXIT_FAILURE, exitCode);
}
