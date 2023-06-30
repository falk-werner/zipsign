/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/signer.hpp"
#include "zipsign/zip.hpp"
#include "zipsign/partial_input_file.hpp"
#include "zipsign/signature.hpp"

#include <stdexcept>

using openssl::PrivateKey;
using openssl::Certificate;
using openssl::CertificateStack;
using openssl::CMS;

namespace zipsign
{

Signer::Signer(std::string const & key_file, std::string const & cert_file)
: embedCerts(false)
{
    addSigner(key_file, cert_file);
}

Signer::~Signer()
{

}

void Signer::addSigner(std::string const key_file, std::string const & cert_file)
{
    auto key = PrivateKey::fromPEM(key_file);
    auto cert = Certificate::fromPEM(cert_file);

    keys.push_back(std::move(key));
    certs.push_back(std::move(cert));
}

void Signer::addIntermediate(std::string const & filename)
{
    intermediates.push_back(Certificate::fromPEM(filename));
}

void Signer::sign(std::string const & filename)
{
    auto signature = createSignature(filename);
    auto comment = ZIPSIGN_SIGNATURE_PREFIX + signature;

    Zip zip(filename);
    zip.setComment(comment);
}

void Signer::setEmbedCerts(bool value)
{
    embedCerts = value;
}

std::string Signer::createSignature(std::string const & filename) 
{
    PartialInputFile partialFile;
    Zip zip(filename);

    CertificateStack intermetiate_certs;
    for(auto & intermediate: intermediates) 
    {
        intermetiate_certs.push(intermediate);
    }

    unsigned int flags = CMS_DETACHED | CMS_BINARY;
    if (!embedCerts)
    {
        flags |= CMS_NOCERTS;
    }

    auto commentStart = zip.getCommentStart();
    auto file = partialFile.open(filename, commentStart);
    auto cms = CMS::sign(nullptr, nullptr, intermetiate_certs, file, flags | CMS_PARTIAL );
    for (size_t i = 0; i < certs.size(); ++i)
    {
        cms.addSigner(certs[i], keys[i], nullptr, flags | CMS_PARTIAL);
    }
    cms.final(file, nullptr, flags);
    
    auto signature = cms.toBase64();

    return signature;
}

}
