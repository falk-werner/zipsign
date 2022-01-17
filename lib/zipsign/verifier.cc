/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/verifier.hpp"
#include "zipsign/zip.hpp"
#include "zipsign/partial_input_file.hpp"
#include "zipsign/signature.hpp"

#include <iostream>

using openssl::Certificate;
using openssl::CertificateStore;
using openssl::CertificateStack;
using openssl::CMS;

namespace zipsign
{

Verifier::Verifier(std::string const & cert_file)
{
    addCertificate(cert_file);
}

Verifier::~Verifier()
{

}

void Verifier::addCertificate(std::string const & filename)
{
    signers.push_back(Certificate::fromPEM(filename));
}

bool Verifier::verify(
    std::string const & filename,
    std::string const & keyring_path,
    bool is_verbose,
    bool is_self_signed)
{
    bool result = false;

    try
    {
        Zip zip(filename);

        auto commentSize = zip.getCommentStart();
        PartialInputFile partialFile;
        auto file = partialFile.open(filename, commentSize);

        auto comment = zip.getComment();
        if (0 != comment.find(ZIPSIGN_SIGNATURE_PREFIX))
        {
            throw std::runtime_error("missing signature");
        }
        auto signature = comment.substr(std::string(ZIPSIGN_SIGNATURE_PREFIX).size());

        CertificateStore store;
        if (!keyring_path.empty())
        {
            store.loadFromFile(keyring_path);
        }
        CertificateStack certs;

        for(auto & cert: signers)
        {
            store.add(cert);
            certs.push(cert);
        }

        auto cms = CMS::fromBase64(signature);

        for (auto & cert: signers)
        {
            if (!is_self_signed && !cert.verify(store, nullptr, cms.getCerts()))
            {
                throw std::runtime_error("signers certificate is not valid");
            }
        }

        result = cms.verify(certs, store, file, nullptr,  CMS_DETACHED | CMS_BINARY | CMS_NO_SIGNER_CERT_VERIFY, is_verbose);
    }
    catch(const std::exception& ex)
    {
        if (is_verbose)
        {
            std::cerr << "error: " << ex.what() << std::endl;
        }
    }
    
    

    return result;
}

}