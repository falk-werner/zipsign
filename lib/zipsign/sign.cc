#include "zipsign/main.hpp"
#include "zipsign/signer.hpp"
#include "zipsign/cli_command.hpp"

#include <getopt.h>

#include <string>
#include <vector>

using zipsign::CliCommand;

namespace
{

struct SignContext
{
    SignContext(int argc, char* argv[], std::ostream & err)
    : command(CliCommand::RUN)
    , exit_code(EXIT_SUCCESS)
    , embed_cert(false)
    , verbose(false)
    {
        option const long_opts[] = {
            {"file", required_argument, nullptr, 'f'},
            {"private-key", required_argument, nullptr, 'p'},
            {"certificate", required_argument, nullptr, 'c'},
            {"intermediate", required_argument, nullptr, 'i'},
            {"embed-certificate", no_argument, nullptr, 'e'},
            {"verbose", no_argument, nullptr, 'v'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, 0, nullptr, 0}
        };

        optind = 0;
        opterr = 0;

        bool done = false;
        while (!done) {
            int option_index = 0;
            int const c = getopt_long(argc, argv, "f:p:c:i:evh", long_opts, &option_index);

            switch (c) {
                case -1:
                    done = true;
                    break;
                case 'f':
                    filename = optarg;
                    break;
                case 'p':
                    key_files.push_back(optarg);
                    break;
                case 'c':
                    cert_files.push_back(optarg);
                    break;
                case 'i':
                    intermediate_certs.push_back(optarg);
                    break;
                case 'e':
                    embed_cert = true;
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
            err << "error: missig required argument: -f" << std::endl;
            exit_code = EXIT_FAILURE;
            command = CliCommand::PRINT_USAGE;
        }

        if (key_files.empty()) {
            err << "error: missig required argument: -p" << std::endl;
            exit_code = EXIT_FAILURE;
            command = CliCommand::PRINT_USAGE;
        }

        if (cert_files.empty()) {
            err << "error: missig required argument: -c" << std::endl;
            exit_code = EXIT_FAILURE;
            command = CliCommand::PRINT_USAGE;
        }

        if (key_files.size() != cert_files.size()) {
            err << "error: count of keys and signer certificated does not match" << std::endl;
            exit_code = EXIT_FAILURE;
            command = CliCommand::PRINT_USAGE;
        }
    }

    CliCommand command; 
    int exit_code;
    bool embed_cert;
    bool verbose;
    std::string filename;
    std::vector<std::string> key_files;
    std::vector<std::string> cert_files;
    std::vector<std::string> intermediate_certs;
};

void printUsage(std::ostream & out)
{
    out << "zipsign, Copyright (c) 2019-2026 Falk Werner\n";
    out << R"(Signs and verifies ZIP archives

sign: Signs a zip archive.

Usage:
        zipsign sign -f <value> -p <value> -c <value> [-i <value>] [-e] [-v] | -h

Arguments:
        -f, --file                      Required. Archive to sign.
        -p, --private-key               Required. Private key to sign. Can be used multiple times.
        -c, --certificate               Required. Certificate of signer. Can be used multiple times.
        -i, --intermediate              Add intermediate certificate.  Can be used multiple times.
        -e, --embed-certificate         Embed signers certificate in signature.
        -v, --verbose                   Enable additional output.
        -h, --help                      Print usage.
)";
}

void run(SignContext const & context)
{
    zipsign::Signer signer(context.key_files[0], context.cert_files[0]);
    signer.setEmbedCerts(context.embed_cert);

    for(auto const & intermediate_cert: context.intermediate_certs) {
        signer.addIntermediate(intermediate_cert);
    }

    for(size_t i = 1; i < context.key_files.size(); ++i) {
        signer.addSigner(context.key_files[i], context.cert_files[i]);
    }

    signer.sign(context.filename);
}

}

namespace zipsign
{

int sign(int argc, char* argv[], std::ostream & out, std::ostream & err)
{
    SignContext context(argc, argv, err);

    switch (context.command)
    {
        case CliCommand::RUN:
            run(context);
            break;
        case CliCommand::PRINT_USAGE:
            printUsage(out);
            break;
    }

    return context.exit_code;
}

}
