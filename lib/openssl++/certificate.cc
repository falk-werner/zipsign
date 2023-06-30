/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
 
#include "openssl++/certificate.hpp"
#include "openssl++/basic_io.hpp"
#include "openssl++/exception.hpp"

#include <openssl/pem.h>
#include <openssl/x509_vfy.h>

#include <memory>

namespace openssl
{

Certificate Certificate::fromPEM(std::string const & filename)
{
    auto cert_file = BasicIO::openInputFile(filename);

    X509 * cert = PEM_read_bio_X509(cert_file, NULL, NULL, NULL);
    if (NULL == cert)
    {
        throw OpenSSLException("failed to parse certificate file");
    }

    return Certificate(cert);

}

Certificate::Certificate(X509 * cert_)
: cert(cert_)
{

}
Certificate & Certificate::operator=(Certificate && other)
{
    X509_free(this->cert);

    this->cert = other.cert;
    other.cert = nullptr;

    return *this;
}

Certificate::Certificate(Certificate && other)
{
    this->cert = other.cert;
    other.cert = nullptr;
}

Certificate::~Certificate()
{
    X509_free(cert);
}

Certificate::operator X509*()
{
    return cert;
}

bool Certificate::verify(X509_STORE * store, STACK_OF(X509) * chain, STACK_OF(X509) * untrusted)
{
    std::unique_ptr<X509_STORE_CTX, void(*)(X509_STORE_CTX *)> context(X509_STORE_CTX_new(), &X509_STORE_CTX_free);
    int rc = X509_STORE_CTX_init(context.get(), store, cert, chain);
    if (1 != rc)
    {
        throw OpenSSLException("X509_STORE_CTX_init");
    }

    X509_STORE_CTX_set0_untrusted(context.get(), untrusted);

    rc = X509_verify_cert(context.get());
    return (1 == rc);
}


}