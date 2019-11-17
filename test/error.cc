#include <gtest/gtest.h>
#include "zipsign/error.h"
#include "zipsign/impl/error.h"
#include "zipsign/error_codes.h"

TEST(zipsign_error, Set)
{
    struct zipsign_error * error = NULL;
    zipsign_impl_error_set(&error, ZIPSIGN_BAD_NOT_IMPLEMENTED, "some error");

    ASSERT_TRUE(nullptr != error);
    ASSERT_EQ(ZIPSIGN_BAD_NOT_IMPLEMENTED, zipsign_error_code(error));
    ASSERT_STREQ("some error", zipsign_error_message(error));

    zipsign_error_release(error);
}

TEST(zipsign_error, DontOverwrite)
{
    struct zipsign_error * error = NULL;
    zipsign_impl_error_set(&error, ZIPSIGN_BAD, "early error");
    zipsign_impl_error_set(&error, ZIPSIGN_BAD_NOT_IMPLEMENTED, "later error");

    ASSERT_TRUE(nullptr != error);
    ASSERT_EQ(ZIPSIGN_BAD, zipsign_error_code(error));
    ASSERT_STREQ("early error", zipsign_error_message(error));

    zipsign_error_release(error);
}

TEST(zipsign_error, IgnoreNull)
{
    zipsign_impl_error_set(NULL, ZIPSIGN_BAD_NOT_IMPLEMENTED, "");
}


TEST(zipsign_error, CreateMessageFromErrorCode)
{
    struct zipsign_error * error = NULL;
    zipsign_errorcode code = ZIPSIGN_BAD_NOT_IMPLEMENTED;
    zipsign_impl_error_set(&error, code, nullptr);

    ASSERT_TRUE(nullptr != error);
    ASSERT_EQ(code, zipsign_error_code(error));
    ASSERT_STREQ(zipsign_errorcode_name(code), zipsign_error_message(error));

    zipsign_error_release(error);
}

TEST(zipsign_error, CreateFromatedErrorMessage)
{
    struct zipsign_error * error = NULL;
    zipsign_errorcode code = ZIPSIGN_BAD_NOT_IMPLEMENTED;
    zipsign_impl_error_set(&error, code, "message: %s:%d", "error", 42);

    ASSERT_TRUE(nullptr != error);
    ASSERT_EQ(code, zipsign_error_code(error));
    ASSERT_STREQ("message: error:42", zipsign_error_message(error));

    zipsign_error_release(error);
}
