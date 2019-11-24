/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef OPENSSL_CERTIFICATE_STACK_HPP
#define OPENSSL_CERTIFICATE_STACK_HPP

#include <openssl/safestack.h>
#include <openssl/x509.h>

namespace openssl
{

class CertificateStack
{
    CertificateStack operator=(CertificateStack const &) = delete;
    CertificateStack(CertificateStack const &) = delete;
public:
    CertificateStack();
    ~CertificateStack();
    operator STACK_OF(X509) *();
    void push(X509 * certificate);
private:
    STACK_OF(X509) * stack;
};

}


#endif
