#ifndef ZIPSIGN_VERIFIER_HPP
#define ZIPSIGN_VERIFIER_HPP

#include <openssl++/openssl++.hpp>
#include <string>

namespace zipsign
{

class Verifier
{
    Verifier operator=(Verifier const &) = delete;
    Verifier(Verifier const &) = delete;
public:
    Verifier(std::string const & cert_file);
    ~Verifier();
    bool verify(std::string const & filename, bool is_verbose = false);
private:
    openssl::Certificate cert;
};

}

#endif
