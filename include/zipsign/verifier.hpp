/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
    enum Result {
        Good = 0,
        Bad = 1,
        BadMissingSignature = 2,
        BadInvalidCertificateChain = 3,
        BadInvalidSignature = 4
    };
    Verifier(std::string const & cert_file);
    ~Verifier();
    void addCertificate(std::string const & filename);
    Result verify(
        std::string const & filename,
        std::string const & keyring_path,
        bool is_verbose = false,
        bool is_self_signed = false);
private:
    std::vector<openssl::Certificate> signers;
};

}

#endif
