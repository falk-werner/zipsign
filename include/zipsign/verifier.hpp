#ifndef ZIPSIGN_VERIFIER_HPP
#define ZIPSIGN_VERIFIER_HPP

#include <openssl++/openssl++.hpp>
#include <string>
#include <vector>
#include <utility>

namespace zipsign
{

class Verifier
{
    Verifier operator=(Verifier const &) = delete;
    Verifier(Verifier const &) = delete;
public:
    Verifier(std::string const & cert_file);
    ~Verifier();
    void addCertificate(std::string const & filename);
    bool verify(
        std::string const & filename,
        std::string const & keyring_path,
        bool is_verbose = false);
private:
    std::vector<openssl::Certificate> signers;
};

}

#endif
