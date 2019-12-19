#ifndef ZIPSIGN_SIGNER_HPP
#define ZIPSIGN_SIGNER_HPP

#include <openssl++/openssl++.hpp>
#include <string>
#include <vector>

namespace zipsign
{

class Signer
{
    Signer operator=(Signer const &) = delete;
    Signer(Signer const &) = delete;
public:
    Signer(std::string const & key_file, std::string const & cert_file);
    ~Signer();
    void addSigner(std::string const key_file, std::string const & cert_file);
    void addIntermediate(std::string const & filename);
    void sign(std::string const & filename);
    void setEmbedCerts(bool value = true);
private:
    std::string createSignature(std::string const & filename);
    std::vector<openssl::PrivateKey> keys;
    std::vector<openssl::Certificate> certs;
    std::vector<openssl::Certificate> intermediates;
    bool embedCerts;
};

}


#endif
