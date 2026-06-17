/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/main.hpp"
#include "zipsign/version.hpp"

namespace
{

void printUsage(std::ostream & out)
{
    out << "zipsign v" << zipsign::version() << ", Copyright (c) 2019-2026 Falk Werner\n";
    out << R"(Signs and verifies ZIP archives

Usage:
        zipsign sign <args...> | verify <args...> | info <args...> | -h

Verbs:
        sign                    Signs a zip archive.
        verify                  Verifies the signature of a zip archive.
        info                    Print info about the signature of zip archive.
        -V, --version           Print version.
        -h, --help              Print usage.

Examples:
        Sign:
                zipsign sign -f archive.zip -p key.pem -c cert.pem
        Verify:
                zipsign verify -f archive.zip -c cert.pem
                zipsign verify -f archive.zip -c cert.pem --self-signed
)";
}

}

namespace zipsign
{

int main(int argc, char* argv[], std::ostream & out, std::ostream & err) noexcept
{
    try {
        if (argc > 1) {
            std::string const verb = argv[1];

            if ((verb == "-h") || (verb == "--help")) {
                printUsage(out);
                return EXIT_SUCCESS;
            }
            else if ((verb == "-V") || (verb == "--version")) {
                out << zipsign::version() << '\n';
                return EXIT_SUCCESS;
            }
            else if (verb == "sign") {
                return sign(argc - 1, &argv[1], out, err);
            }
            else if (verb == "verify") {
                return verify(argc - 1, &argv[1], out, err);
            }
            else if (verb == "info") {
                return info(argc - 1, &argv[1], out, err);
            }
            else {
                err << "error: unknown verb: " << verb << std::endl;
                printUsage(out);
                return EXIT_FAILURE;
            }
        }
        else {
            err << "error: missing verb" << std::endl;        
            printUsage(out);
            return EXIT_FAILURE;
        }
    }
    catch (std::exception const & ex) {
        err << "error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        err << "error: unexpected error" << std::endl;
        return EXIT_FAILURE;
    }
}

}
