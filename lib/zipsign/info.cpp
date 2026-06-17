/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/main.hpp"
#include "zipsign/cli_command.hpp"
#include "zipsign/zip.hpp"
#include "zipsign/partial_input_file.hpp"
#include "zipsign/signature.hpp"

#include <getopt.h>

#include <string>

using zipsign::CliCommand;

namespace
{

struct InfoContext
{

    InfoContext(int argc, char* argv[], std::ostream & err)
    : command(CliCommand::RUN)
    , exit_code(EXIT_SUCCESS)
    {
        option const long_opts[] = {
            {"file", required_argument, nullptr, 'f'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, 0, nullptr, 0}
        };

        optind = 0;
        opterr = 0;

        bool done = false;
        while (!done) {
            int option_index = 0;
            int const c = getopt_long(argc, argv, "f:h", long_opts, &option_index);

            switch (c) {
                case -1:
                    done = true;
                    break;
                case 'f':
                    filename = optarg;
                    break;
                case 'h':
                    command = CliCommand::PRINT_USAGE;
                    done = true;
                    break;
                default:
                    err << "error: unrecognized argument" << std::endl;
                    exit_code = EXIT_FAILURE;
                    command = CliCommand::PRINT_USAGE;
                    done = true;
                    break;
            }
        }

        if (command == CliCommand::PRINT_USAGE) {
            return;
        }

        if (filename.empty()) {
            err << "error: missig required argument: -f" << std::endl;
            exit_code = EXIT_FAILURE;
            command = CliCommand::PRINT_USAGE;
        }
    }

    CliCommand command;
    int exit_code;
    std::string filename;
};

void printUsage(std::ostream & out)
{
    out << "zipsign, Copyright (c) 2019-2025 Falk Werner\n";
    out << R"(Signs and verifies ZIP archives

info: Print info about the signature of zip archive.

Usage:
        zipsign info -f <value> | -h

Arguments:
        -f, --file                      Required. Archive to verify.
        -h, --help                      Print usage.
)";
}

void run(std::string const & filename, std::ostream & out)
{
    zipsign::Zip zip(filename);

    auto commentSize = zip.getCommentStart();
    auto file = zipsign::PartialInputFile::open(filename, commentSize);

    auto comment = zip.getComment();
    if (0 != comment.find(ZIPSIGN_SIGNATURE_PREFIX))
    {
        throw std::runtime_error("missing signature");
    }
    auto signature = comment.substr(std::string(ZIPSIGN_SIGNATURE_PREFIX).size());

    auto cms = openssl::CMS::fromBase64(signature);
    out << cms.toString() << std::endl;
}

}

namespace zipsign
{

int info(int argc, char* argv[], std::ostream & out, std::ostream & err)
{
    InfoContext context(argc, argv, err);

    switch (context.command) {
        case CliCommand::RUN:
            run(context.filename, out);
            break;
        case CliCommand::PRINT_USAGE:
            printUsage(out);
            break;
    }

    return context.exit_code;
}

}