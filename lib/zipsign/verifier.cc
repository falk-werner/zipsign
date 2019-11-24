#include "zipsign/verifier.hpp"
#include <iostream>

using openssl::Certificate;

namespace zipsign
{

Verifier::Verifier(std::string const & cert_file)
: cert(Certificate::fromPEM(cert_file))
{

}

Verifier::~Verifier()
{

}

bool Verifier::verify(std::string const & filename, bool is_verbose)
{
    if (is_verbose)
    {
        std::cerr << "not implemeted" << std::endl;
    }
    return false;
}

}