
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "zipsign/zipsign.hpp"
#include "openssl++/openssl++.hpp"
#include "cli/cli.hpp"

using openssl::OpenSSL;

using zipsign::Signer;
using zipsign::Verifier;
using zipsign::Informer;

using cli::App;
using cli::Arguments;
using cli::Verb;

namespace
{
    int sign(Arguments const & args)
    {
        auto const & filename = args.get('f');
        auto const & key_files = args.getList('p');
        auto const & cert_files = args.getList('c');
        auto embed_cert = args.contains('e');

        int result = EXIT_FAILURE;

        try
        {
            if (key_files.size() != cert_files.size())
            {
                throw std::runtime_error("count of keys and signer certificated does not match");
            }

            Signer signer(key_files[0], cert_files[0]);
            signer.setEmbedCerts(embed_cert);
            
            if (args.contains('i'))
            {
                for(auto & intermediate: args.getList('i'))
                {
                    signer.addIntermediate(intermediate);
                }
            }

            for(size_t i = 1; i < key_files.size(); ++i)
            {
                signer.addSigner(key_files[i], cert_files[i]);
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
        auto const & cert_files = args.getList('c');
        auto is_verbose = args.contains('v');
        auto is_self_signed = args.contains('s');
        std::string keyring_path;
        if (args.contains('k')) 
        {
            keyring_path = args.get('k');
        }

        Verifier::Result result = Verifier::Bad;

        try
        {
            Verifier verifier(cert_files[0]);
            for(size_t i = 1; i < cert_files.size(); ++i)
            {
                verifier.addCertificate(cert_files[1]);
            }
            result = verifier.verify(filename, keyring_path, is_verbose, is_self_signed);

            switch (result) {
                case Verifier::Good:
                    std::cout << "OK" << std::endl;
                    break;
                case Verifier::BadMissingSignature:
                    std::cout << "INVALID_MISSING_SIGNATURE" << std::endl;
                    break;
                case Verifier::BadInvalidCertificateChain:
                    std::cout << "INVALID_CERTIFICATE_CHAIN" << std::endl;
                    break;
                case Verifier::BadInvalidSignature:
                    std::cout << "INVALID_SIGNATURE" << std::endl;
                    break;
                case Verifier::Bad:
                    // fall-through
                default:
                    std::cout << "INVALID" << std::endl;
                    break;
            }
        }
        catch (std::exception const & ex)
        {
            std::cerr << "error: " << ex.what() << std::endl;
        }
        
        return static_cast<int>(result);
    }

    int info(Arguments const & args)
    {
        int result = EXIT_FAILURE;
        auto const & filename = args.get('f');

        try
        {
            Informer informer;
            informer.print(filename);

            result = EXIT_SUCCESS;
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
            "\t\tzipsign verify -f archive.zip -c cert.pem --self-signed\n"
        )
    ;

    app.add("sign", sign)
        .setHelpText("Signs a zip archive.")
        .addArg('f', "file", "Archive to sign.")
        .addList('p', "private-key", "Private key to sign.")
        .addList('c', "certificate", "Certificate of signer.")
        .addList('i', "intermediate", "Add intermediate certificate", false)
        .addFlag('e', "embed-certificate", "Embed signers certificate in signature")      
        .addFlag('v', "verbose", "Enable additional output")      
    ;

    app.add("verify", verify)
        .setHelpText("Verifies the signature of a zip archive.")
        .addArg('f', "file", "Archive to verify.")
        .addArg('c', "certificate", "Certificate of signer.")
        .addArg('k', "keyring", "Path of keyring file.", false)
        .addFlag('v', "verbose", "Enable additionl output")      
        .addFlag('s', "self-signed", "Allows self signed certificates, skip cert verify")
    ;

    app.add("info", info)
        .setHelpText("Print info about the signature of zip archive.")
        .addArg('f', "file", "Archive to verify.")
    ;

    return app.run(argc, argv);
}

