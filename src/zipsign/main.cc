
#include <iostream>
#include <cstdlib>

#include "zipsign/zipsign.hpp"
#include "openssl++/openssl++.hpp"
#include "cli/cli.hpp"

using openssl::OpenSSL;
using cli::App;
using cli::Argument;
using cli::Arguments;
using cli::Verb;

namespace
{
    int sign(Arguments args)
    {
        std::cout << "not implemented" << std::endl;
        return EXIT_FAILURE;
    }

    int verify(Arguments args)
    {
        std::cout << "not implemented" << std::endl;
        return EXIT_FAILURE;
    }
}

int main(int argc, char * argv[])
{
    OpenSSL::init();

    App app("zipsign");
    app
        .setCopyright("2019 Falk Werner")
        .setDescription("Signs and verifies ZIP archives")
        .setAdditionalInfo(
            "Examples:\n"
            "\tSign:\n"
            "\t\tzipsign sign -f archive.zip -p key.pem -c cert.pem\n"
            "\tVerify:\n"
            "\t\tzipsign verify -f archive.zip -c cert.pem\n"
        )
        .add(Verb("sign", sign)
            .setHelpText("Signs a zip archive.")
            .add(Argument('f', "file").setHelpText("Archive to sign."))
            .add(Argument('p', "private-key").setHelpText("Private key to sign."))
            .add(Argument('c', "certificate").setHelpText("Certificate of signer."))
            .add(Argument('v', "verbose").setHelpText("Enable additionl output").setFlag().setOptional())      
        )
        .add(Verb("verify", verify)
            .setHelpText("Verifies the signature of a zip archive.")
            .add(Argument('f', "file").setHelpText("Archive to verify."))
            .add(Argument('c', "certificate").setHelpText("Certificate of signer."))
            .add(Argument('v', "verbose").setHelpText("Enable additionl output").setFlag().setOptional())      
        )
    ;

    return app.run(argc, argv);
}

