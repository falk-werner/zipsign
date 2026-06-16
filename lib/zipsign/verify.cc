/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/main.hpp"
#include "zipsign/verifier.hpp"
#include "zipsign/cli_command.hpp"

#include <getopt.h>

#include <string>
#include <vector>

using zipsign::CliCommand;

namespace
{

struct VerifyContext
{
    VerifyContext(int argc, char * argv[], std::ostream & err)
    : command(CliCommand::RUN)
    , exit_code(EXIT_SUCCESS)
    , verbose(false)
    , self_signed(false)
    {
        option const long_opts[] = {
            {"file", required_argument, nullptr, 'f'},
            {"certificate", required_argument, nullptr, 'c'},
            {"keyring", required_argument, nullptr, 'k'},
            {"self-signed", no_argument, nullptr, 's'},
            {"verbose", no_argument, nullptr, 'v'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, 0, nullptr, 0}
        };

        optind = 0;
        opterr = 0;

        bool done = false;
        while (!done) {
            int option_index = 0;
            int const c = getopt_long(argc, argv, "f:c:k:svh", long_opts, &option_index);

            switch (c) {
                case -1:
                    done = true;
                    break;
                case 'f':
                    filename = optarg;
                    break;
                case 'c':
                    cert_files.push_back(optarg);
                    break;
                case 'k':
                    keyring = optarg;
                    break;
                case 's':
                    self_signed = true;
                    break;
                case 'v':
                    verbose = true;
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
            err << "error: missing required argument: -f" << std::endl;
            exit_code = EXIT_FAILURE;
            command = CliCommand::PRINT_USAGE;
        }

        if (cert_files.empty()) {
            err << "error: missing required argument: -c" << std::endl;
            exit_code = EXIT_FAILURE;
            command = CliCommand::PRINT_USAGE;
        }
    }

    CliCommand command;
    int exit_code;
    bool verbose;
    bool self_signed;
    std::string filename;
    std::vector<std::string> cert_files;
    std::string keyring;
};

void printUsage(std::ostream & out)
{
    out << "zipsign, Copyright (c) 2019-2025 Falk Werner\n";
    out << R"(Signs and verifies ZIP archives

verify: Verifies the signature of a zip archive.

Usage:
        zipsign verify -f <value> -c <value> [-k <value>] [-v] [-s] | -h

Arguments:
        -f, --file                      Required. Archive to verify.
        -c, --certificate               Required. Certificate of signer. Can be used multiple times.
        -k, --keyring                   Path of keyring file.
        -v, --verbose                   Enable additionl output.
        -s, --self-signed               Allows self signed certificates, skip cert verify.
        -h, --help                      Print usage.
)";
}

std::ostream & operator<<(std::ostream & out, zipsign::Verifier::Result result)
{
    switch (result) {
        case zipsign::Verifier::Good:
            out << "OK";
            break;
        case zipsign::Verifier::BadMissingSignature:
            out << "INVALID_MISSING_SIGNATURE";
            break;
        case zipsign::Verifier::BadInvalidCertificateChain:
            out << "INVALID_CERTIFICATE_CHAIN";
            break;
        case zipsign::Verifier::BadInvalidSignature:
            out << "INVALID_SIGNATURE";
            break;
        case zipsign::Verifier::Bad:
            // fall-through
        default:
            out << "INVALID";
            break;
    }

    return out;
}

void run(VerifyContext & context, std::ostream & out, std::ostream & err)
{
    zipsign::Verifier verifier(context.cert_files[0]);
    for(size_t i = 1; i < context.cert_files.size(); ++i) {
        verifier.addCertificate(context.cert_files[i]);
    }

    auto const result = verifier.verify(context.filename, context.keyring,
        err, context.verbose, context.self_signed);

    out << result << std::endl;
    context.exit_code = static_cast<int>(result);
}

}

namespace zipsign
{

int verify(int argc, char* argv[], std::ostream & out, std::ostream & err)
{
    VerifyContext context(argc, argv, err);

    switch (context.command)
    {
        case CliCommand::RUN:
            run(context, out, err);
            break;
        case CliCommand::PRINT_USAGE:
            printUsage(out);
            break;
    }

    return context.exit_code;
}

}
