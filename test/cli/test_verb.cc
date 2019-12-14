#include <gtest/gtest.h>
#include <cli/cli.hpp>
#include <cstdlib>
#include <cli/mock_app_info.hpp>

using cli::Verb;
using cli::Argument;
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

TEST(Verb, RunSuccess)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(0);
    EXPECT_CALL(appInfo, getCopyright()).Times(0);
    EXPECT_CALL(appInfo, getDescription()).Times(0);

    Verb verb("run", RunSuccess);
    verb.setApp(appInfo);

    int exitCode = verb.run(0, nullptr);
    ASSERT_EQ(EXIT_SUCCESS, exitCode);    
}

TEST(Verb, RunSuccessWithArgs)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(0);
    EXPECT_CALL(appInfo, getCopyright()).Times(0);
    EXPECT_CALL(appInfo, getDescription()).Times(0);

    Verb verb("run", RunSuccess);
    verb.setApp(appInfo);
    verb.add(Argument('f', "file").setHelpText("a file"));
    verb.add(Argument('v', "verbose").setHelpText("enable chatty mode").setFlag());
    verb.add(Argument('l', "log-level").setHelpText("set log level").setDefaultValue("error").setOptional());

    char arg0[] = "run";
    char arg1[] = "--file";
    char arg2[] = "some.file";
    char arg3[] = "--verbose";
    char * argv[] = { arg0, arg1, arg2, arg3, nullptr };
    int exitCode = verb.run(4, argv);
    ASSERT_EQ(EXIT_SUCCESS, exitCode);    
}

TEST(Verb, RunFailure)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(0);
    EXPECT_CALL(appInfo, getCopyright()).Times(0);
    EXPECT_CALL(appInfo, getDescription()).Times(0);

    Verb verb("run", RunFailure);
    verb.setApp(appInfo);

    int exitCode = verb.run(0, nullptr);
    ASSERT_EQ(EXIT_FAILURE, exitCode);    
}

TEST(Verb, Fail_MissingRequiredArg)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(AtLeast(1)).WillRepeatedly(ReturnRefOfCopy(std::string("app")));
    EXPECT_CALL(appInfo, getCopyright()).Times(1).WillOnce(ReturnRefOfCopy(std::string("2019 Falk Werner")));
    EXPECT_CALL(appInfo, getDescription()).Times(1).WillOnce(ReturnRefOfCopy(std::string("Simple App")));

    Verb verb("run", RunSuccess);
    verb.setApp(appInfo);
    verb.add(Argument('f', "file").setHelpText("a file"));
    verb.add(Argument('v', "verbose").setHelpText("enable chatty mode").setFlag());
    verb.add(Argument('l', "log-level").setHelpText("set log level").setDefaultValue("error").setOptional());

    char arg0[] = "run";
    char arg1[] = "--file";
    char arg2[] = "some.file";
    char * argv[] = { arg0, arg1, arg2, nullptr };
    int exitCode = verb.run(3, argv);
    ASSERT_EQ(EXIT_FAILURE, exitCode);    
}

TEST(Verb, Fail_UnrecognizedArg)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(AtLeast(1)).WillRepeatedly(ReturnRefOfCopy(std::string("app")));
    EXPECT_CALL(appInfo, getCopyright()).Times(1).WillOnce(ReturnRefOfCopy(std::string("2019 Falk Werner")));
    EXPECT_CALL(appInfo, getDescription()).Times(1).WillOnce(ReturnRefOfCopy(std::string("Simple App")));

    Verb verb("run", RunSuccess);
    verb.setApp(appInfo);
    verb.add(Argument('f', "file").setHelpText("a file"));
    verb.add(Argument('v', "verbose").setHelpText("enable chatty mode").setFlag());
    verb.add(Argument('l', "log-level").setHelpText("set log level").setDefaultValue("error").setOptional());

    char arg0[] = "run";
    char arg1[] = "--file";
    char arg2[] = "some.file";
    char arg3[] = "-v";
    char arg4[] = "--unknown";
    char * argv[] = { arg0, arg1, arg2, arg3, arg4, nullptr };
    int exitCode = verb.run(5, argv);
    ASSERT_EQ(EXIT_FAILURE, exitCode);    
}

TEST(Verb, PrintHelp)
{
    AppInfoMock appInfo;
    EXPECT_CALL(appInfo, getName()).Times(AtLeast(1)).WillRepeatedly(ReturnRefOfCopy(std::string("app")));
    EXPECT_CALL(appInfo, getCopyright()).Times(1).WillOnce(ReturnRefOfCopy(std::string("2019 Falk Werner")));
    EXPECT_CALL(appInfo, getDescription()).Times(1).WillOnce(ReturnRefOfCopy(std::string("Simple App")));

    Verb verb("run", RunSuccess);
    verb.setApp(appInfo);
    verb.setHelpText("run simple app");
    verb.add(Argument('f', "file").setHelpText("a file"));
    verb.add(Argument('v', "verbose").setHelpText("enable chatty mode").setFlag());
    verb.add(Argument('l', "log-level").setHelpText("set log level").setDefaultValue("error").setOptional());

    char arg0[] = "run";
    char arg1[] = "-h";
    char * argv[] = { arg0, arg1, nullptr };
    int exitCode = verb.run(2, argv);
    ASSERT_EQ(EXIT_SUCCESS, exitCode);
}