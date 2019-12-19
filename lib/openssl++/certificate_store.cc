/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "openssl++/certificate_store.hpp"
#include "openssl++/exception.hpp"

namespace openssl
{

CertificateStore::CertificateStore()
: store(X509_STORE_new())
{
    if (nullptr == store)
    {
        throw OpenSSLException("failed to create certifiacte store");
    }
}

CertificateStore::~CertificateStore()
{
    X509_STORE_free(store);
}

CertificateStore & CertificateStore::operator=(CertificateStore && other)
{
    X509_STORE_free(this->store);

    this->store = other.store;
    other.store = nullptr;

    return *this;
}

CertificateStore::CertificateStore(CertificateStore && other)
{
    this->store = other.store;
    other.store = nullptr;
}

CertificateStore::operator X509_STORE *()
{
    return store;
}

void CertificateStore::add(X509 * cert)
{
    int rc = X509_STORE_add_cert(store, cert);
    if (1 != rc)
    {
        throw OpenSSLException("failed to add certificate to store");
    }
}

void CertificateStore::loadFromFile(std::string const & filename)
{
    int rc = X509_STORE_load_locations(store, filename.c_str(), nullptr);
    if (1 != rc)
    {
        throw OpenSSLException("failed to load store from file");
    }
}


}