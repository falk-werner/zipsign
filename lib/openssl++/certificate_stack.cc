/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <openssl++/certificate_stack.hpp>
#include <openssl++/exception.hpp>

namespace openssl
{

CertificateStack::CertificateStack()
: stack(sk_X509_new_null())
{
    if (nullptr == stack)
    {
        throw OpenSSLException("failed to create CertificateStack");
    }
}

CertificateStack::~CertificateStack()
{
    sk_X509_free(stack);
}

CertificateStack::operator STACK_OF(X509) *()
{
    return stack;
}

void CertificateStack::push(X509 * certificate)
{
    int rc = sk_X509_push(stack, certificate);
    if (0 == rc)
    {
        throw OpenSSLException("failed to push certificate to stack");
    }
}

}
