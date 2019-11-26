#include "zipsign/signer.hpp"
#include "zipsign/zip.hpp"
#include "zipsign/partial_input_file.hpp"
#include "zipsign/signature.hpp"

#include <stdexcept>

using openssl::PrivateKey;
using openssl::Certificate;
using openssl::CMS;

namespace zipsign
{

Signer::Signer(std::string const & key_file, std::string const & cert_file)
: key(PrivateKey::fromPEM(key_file))
, cert(Certificate::fromPEM(cert_file))
{

}

Signer::~Signer()
{

}

void Signer::sign(std::string const & filename)
{
    auto signature = createSignature(filename);
    auto comment = ZIPSIGN_SIGNATURE_PREFIX + signature;

    Zip zip(filename);
    zip.setComment(comment);
}

std::string Signer::createSignature(std::string const & filename) 
{
    PartialInputFile partialFile;
    Zip zip(filename);

    auto commentStart = zip.getCommentStart();
    auto file = partialFile.open(filename, commentStart);
    auto cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);
    auto signature = cms.toBase64();

    return signature;
}

}
