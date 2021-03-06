/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef OPENSSL_CMS_HPP
#define OPENSSL_CMS_HPP

#include <openssl/cms.h>
#include <string>

namespace openssl
{

class CMS
{
    CMS & operator=(CMS const &) = delete;
    CMS(CMS const &) = delete;
public:
    static CMS fromBase64(std::string const & data);
    static CMS sign(X509 * cert, EVP_PKEY * key, STACK_OF(X509) * certs, BIO * data, unsigned int flags);
    explicit CMS(CMS_ContentInfo * cms_);
    CMS(CMS && other);
    CMS & operator=(CMS && other);
    ~CMS();
    operator CMS_ContentInfo*();
    void addSigner(X509 * cert, EVP_PKEY * key, EVP_MD const * md, unsigned int flags);
    void final(BIO * data, BIO * dcont, unsigned int flags);
    void saveToBIO(BIO * bio) const;
    bool verify(STACK_OF(X509) * certs, X509_STORE * store, BIO * indata, BIO * outdata, unsigned int flags, bool is_verbose = false);
    STACK_OF(X509) * getCerts();
    std::string toBase64() const;
    std::string toString() const;
private:
    CMS_ContentInfo * cms;
};

}

#endif
