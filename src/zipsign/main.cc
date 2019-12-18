
#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "zipsign/zipsign.hpp"
#include "openssl++/openssl++.hpp"
#include "cli/cli.hpp"

using openssl::OpenSSL;
using openssl::CMS;
using openssl::BasicIO;
using openssl::Certificate;
using openssl::PrivateKey;

using zipsign::Signer;
using zipsign::Verifier;

using cli::App;
using cli::Arguments;
using cli::Verb;

namespace
{
    int sign(Arguments const & args)
    {
        auto const & filename = args.get('f');
        auto const & key_file = args.get('p');
        auto const & cert_file = args.get('c');

        int result = EXIT_FAILURE;

        try
        {
            Signer signer(key_file, cert_file);

            if (args.contains('i'))
            {
                for(auto & intermediate: args.getList('i'))
                {
                    signer.addIntermediate(intermediate);
                }
            }

            signer.sign(filename);
            result = EXIT_SUCCESS;
        }
        catch (std::exception const & ex)
        {
            std::cerr << "error: " << ex.what() << std::endl;
        }

        return result;
    }

    int verify(Arguments const & args)
    {
        auto const & filename = args.get('f');
        auto const & cert_file = args.get('c');
        auto is_verbose = args.contains('v');
        std::string keyring_path;
        if (args.contains('k')) 
        {
            keyring_path = args.get('k');
        }

        int result = EXIT_FAILURE;

        try
        {
            Verifier verifier(cert_file);
            bool isValid = verifier.verify(filename, keyring_path, is_verbose);

            if (isValid)
            {
                std::cout << "OK" << std::endl;
                result = EXIT_SUCCESS;
            }
            else
            {
                std::cout << "INVALID" << std::endl;
            }
        }
        catch (std::exception const & ex)
        {
            std::cerr << "error: " << ex.what() << std::endl;
        }
        
        return result;
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
    ;

    app.add("sign", sign)
        .setHelpText("Signs a zip archive.")
        .addArg('f', "file", "Archive to sign.")
        .addArg('p', "private-key", "Private key to sign.")
        .addArg('c', "certificate", "Certificate of signer.")
        .addList('i', "intermediate", "Add intermediate certificate", false)
        .addFlag('v', "verbose", "Enable additionl output")      
    ;

    app.add("verify", verify)
        .setHelpText("Verifies the signature of a zip archive.")
        .addArg('f', "file", "Archive to verify.")
        .addArg('c', "certificate", "Certificate of signer.")
        .addArg('k', "keyring", "Path of keyring file.", false)
        .addFlag('v', "verbose", "Enable additionl output")      
    ;

    return app.run(argc, argv);
}

