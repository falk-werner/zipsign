#include "zipsign/signer.hpp"
#include <stdexcept>

using openssl::PrivateKey;
using openssl::Certificate;

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

void Signer::sign(std::string filename)
{
    throw std::runtime_error("not implemented");
}


}
