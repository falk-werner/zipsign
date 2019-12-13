#include <gtest/gtest.h>
#include <cli/cli.hpp>
#include <cstdlib>
#include <cli/mock_app_info.hpp>

using cli::Verb;
using cli::Argument;
using cli::Arguments;

int RunSuccess(Arguments const & args)
{
    return EXIT_SUCCESS;
}

int RunFailure(Arguments const & args)
{
    return EXIT_FAILURE;
}

TEST(Verb, RunSuccess)
{
    AppInfoMock appInfo;
    Verb verb("run", RunSuccess);
    verb.setApp(appInfo);

    int exitCode = verb.run(0, nullptr);
    ASSERT_EQ(EXIT_SUCCESS, exitCode);    
}

TEST(Verb, RunFailure)
{
    AppInfoMock appInfo;
    Verb verb("run", RunFailure);
    verb.setApp(appInfo);

    int exitCode = verb.run(0, nullptr);
    ASSERT_EQ(EXIT_FAILURE, exitCode);    
}