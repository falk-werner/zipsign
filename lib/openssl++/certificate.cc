/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
 
#include "openssl++/certificate.hpp"
#include "openssl++/basic_io.hpp"
#include "openssl++/exception.hpp"

#include <openssl/pem.h>

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

    return std::move(Certificate(cert));

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

}