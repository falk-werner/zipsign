#include <gtest/gtest.h>
#include <cstdlib>

#include "cli/cli.hpp"
#include "cli/default_verb.hpp"
#include "cli/mock_app_info.hpp"

using cli::DefaultVerb;
using cli::Arguments;

using testing::ReturnRefOfCopy;
using testing::AtLeast;

int RunSuccess(Arguments const & args)
{
    return EXIT_SUCCESS;
}

int RunFailure(Arguments const & args)
{
    return EXIT_FAILURE;
}

TEST(DefaultVerb, RunSuccess)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(0);
    EXPECT_CALL(appInfo, getCopyright()).Times(0);
    EXPECT_CALL(appInfo, getDescription()).Times(0);

    DefaultVerb verb(appInfo, "run", RunSuccess);

    int exitCode = verb.run(0, nullptr);
    ASSERT_EQ(EXIT_SUCCESS, exitCode);    
}

TEST(DefaultVerb, RunSuccessWithArgs)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(0);
    EXPECT_CALL(appInfo, getCopyright()).Times(0);
    EXPECT_CALL(appInfo, getDescription()).Times(0);

    DefaultVerb verb(appInfo, "run", RunSuccess);
    verb.addArg('f', "file", "a file", true, "");
    verb.addFlag('v', "verbose", "enable chatty mode");
    verb.addArg('l', "log-level", "set log level", false, "error");

    char arg0[] = "run";
    char arg1[] = "--file";
    char arg2[] = "some.file";
    char arg3[] = "--verbose";
    char * argv[] = { arg0, arg1, arg2, arg3, nullptr };
    int exitCode = verb.run(4, argv);
    ASSERT_EQ(EXIT_SUCCESS, exitCode);    
}

TEST(DefaultVerb, RunFailure)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(0);
    EXPECT_CALL(appInfo, getCopyright()).Times(0);
    EXPECT_CALL(appInfo, getDescription()).Times(0);

    DefaultVerb verb(appInfo, "run", RunFailure);

    int exitCode = verb.run(0, nullptr);
    ASSERT_EQ(EXIT_FAILURE, exitCode);    
}

TEST(DefaultVerb, Fail_MissingRequiredArg)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(AtLeast(1)).WillRepeatedly(ReturnRefOfCopy(std::string("app")));
    EXPECT_CALL(appInfo, getCopyright()).Times(1).WillOnce(ReturnRefOfCopy(std::string("2019 Falk Werner")));
    EXPECT_CALL(appInfo, getDescription()).Times(1).WillOnce(ReturnRefOfCopy(std::string("Simple App")));

    DefaultVerb verb(appInfo, "run", RunSuccess);
    verb.addArg('f', "file", "a file", true, "");
    verb.addFlag('v', "verbose", "enable chatty mode");
    verb.addArg('l', "log-level", "set log level", true, "error");

    char arg0[] = "run";
    char arg1[] = "--file";
    char arg2[] = "some.file";
    char * argv[] = { arg0, arg1, arg2, nullptr };
    int exitCode = verb.run(3, argv);
    ASSERT_EQ(EXIT_FAILURE, exitCode);    
}

TEST(DefaultVerb, Fail_UnrecognizedArg)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(AtLeast(1)).WillRepeatedly(ReturnRefOfCopy(std::string("app")));
    EXPECT_CALL(appInfo, getCopyright()).Times(1).WillOnce(ReturnRefOfCopy(std::string("2019 Falk Werner")));
    EXPECT_CALL(appInfo, getDescription()).Times(1).WillOnce(ReturnRefOfCopy(std::string("Simple App")));

    DefaultVerb verb(appInfo, "run", RunSuccess);
    verb.addArg('f', "file", "a file", true, "");
    verb.addFlag('v', "verbose", "enable chatty mode");
    verb.addArg('l', "log-level", "set log level", false, "error");

    char arg0[] = "run";
    char arg1[] = "--file";
    char arg2[] = "some.file";
    char arg3[] = "-v";
    char arg4[] = "--unknown";
    char * argv[] = { arg0, arg1, arg2, arg3, arg4, nullptr };
    int exitCode = verb.run(5, argv);
    ASSERT_EQ(EXIT_FAILURE, exitCode);    
}

TEST(DefaultVerb, PrintHelp)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(AtLeast(1)).WillRepeatedly(ReturnRefOfCopy(std::string("app")));
    EXPECT_CALL(appInfo, getCopyright()).Times(1).WillOnce(ReturnRefOfCopy(std::string("2019 Falk Werner")));
    EXPECT_CALL(appInfo, getDescription()).Times(1).WillOnce(ReturnRefOfCopy(std::string("Simple App")));

    DefaultVerb verb(appInfo, "run", RunSuccess);
    verb.setHelpText("run simple app");
    verb.addArg('f', "file", "a file", true, "");
    verb.addFlag('v', "verbose", "enable chatty mode");
    verb.addArg('l', "log-level", "set log level", false, "error");

    char arg0[] = "run";
    char arg1[] = "-h";
    char * argv[] = { arg0, arg1, nullptr };
    int exitCode = verb.run(2, argv);
    ASSERT_EQ(EXIT_SUCCESS, exitCode);
}
