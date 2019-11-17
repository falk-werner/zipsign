#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "zipsign/zipsign.h"

struct context
{
    int i;
};

int main(int argc, char * argv[])
{
    int exit_code = EXIT_SUCCESS;
    struct zipsign_error * error = NULL;

    zipsign_openssl_init(&error);


    if (NULL != error)
    {
        fprintf(stderr, "error: %s (%d)\n", zipsign_error_message(error), zipsign_error_code(error));
        exit_code = EXIT_FAILURE;
    }

    return exit_code;
}

