#ifndef ZIPSIGN_SIGNER_HPP
#define ZIPSIGN_SIGNER_HPP

#include <openssl++/openssl++.hpp>
#include <string>

namespace zipsign
{

class Signer
{
    Signer operator=(Signer const &) = delete;
    Signer(Signer const &) = delete;
public:
    Signer(std::string const & key_file, std::string const & cert_file);
    ~Signer();
    void sign(std::string filename);
private:
    openssl::PrivateKey key;
    openssl::Certificate cert;
};

}


#endif
