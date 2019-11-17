#include "zipsign/impl/error.h"

#include <stdio.h>
#include <stdlib.h>

struct zipsign_error
{
    zipsign_errorcode code;
    char * message;
};

void
zipsign_impl_error_set(
    struct zipsign_error * * error,
    zipsign_errorcode code,
    char const * format,
    ...)
{
    if ((NULL != error) && (NULL == *error))
    {
        struct zipsign_error * err = malloc(sizeof(struct zipsign_error));
        err->code = code;
        err->message = NULL;

        if (NULL != format)
        {
            va_list measure_args;
            va_start(measure_args, format);
            size_t length = vsnprintf(NULL, 0, format, measure_args);
            va_end(measure_args);

            err->message = malloc(length + 1);

            va_list args;
            va_start(args, format);
            vsnprintf(err->message, length + 1, format, args);
            va_end(args);
        }

        *error = err;
    }
}

zipsign_errorcode
zipsign_impl_error_code(
    struct zipsign_error * error)
{
    return error->code;
}

char const *
zipsign_impl_error_message(
    struct zipsign_error * error)
{
    return (NULL != error->message) ? error->message : zipsign_errorcode_name(error->code);
}

void
zipsign_impl_error_release(
    struct zipsign_error * error)
{
    if (NULL != error)
    {
        free(error->message);
        free(error);
    }
}
