/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef OPENSSL_CERTIFICATE_STORE_HPP
#define OPENSSL_CERTIFICATE_STORE_HPP

 #include <openssl/x509_vfy.h>
 #include <string>

namespace openssl
{

class CertificateStore
{
    CertificateStore& operator=(CertificateStore const &) = delete;
    CertificateStore(CertificateStore const &) = delete;
public:
    CertificateStore();
    ~CertificateStore();
    CertificateStore & operator=(CertificateStore && other);
    CertificateStore(CertificateStore && other);
    operator X509_STORE *();
    void add(X509 * cert);
private:
    X509_STORE * store;
};

}

#endif
