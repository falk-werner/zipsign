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
: key(PrivateKey::fromPEM(key_file))
, cert(Certificate::fromPEM(cert_file))
, embedCerts(false)
{

}

Signer::~Signer()
{

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

    int flags = CMS_DETACHED | CMS_BINARY;
    if (!embedCerts)
    {
        flags |= CMS_NOCERTS;
    }

    auto commentStart = zip.getCommentStart();
    auto file = partialFile.open(filename, commentStart);
    auto cms = CMS::sign(cert, key, intermetiate_certs, file, flags);
    auto signature = cms.toBase64();

    return signature;
}

}
