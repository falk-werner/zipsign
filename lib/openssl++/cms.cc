/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "openssl++/cms.hpp"
#include "openssl++/exception.hpp"
#include "openssl++/basic_io.hpp"

#include "base64/base64.h"

#include <iostream>
#include <vector>

namespace openssl
{

CMS CMS::fromBase64(std::string const & data)
{
    size_t decoded_size = base64_decoded_size(data.c_str(), data.size());
    std::vector<uint8_t> decoded(decoded_size);
    base64_decode(data.c_str(), data.size(), decoded.data(), decoded.size());

    auto bio = BasicIO::fromMemory(decoded.data(), decoded.size());
    CMS_ContentInfo * cms = d2i_CMS_bio(bio, nullptr);
    if (NULL == cms)
    {
        throw OpenSSLException("failed to read file");
    }

    return std::move(CMS(cms));

}

CMS CMS::sign(X509 * cert, EVP_PKEY * key, STACK_OF(X509) * certs, BIO * data, unsigned int flags)
{
    CMS_ContentInfo * cms = CMS_sign(cert, key, certs, data, flags);
    if (NULL == cms)
    {
        throw OpenSSLException("unable to sign file");
    }

    return std::move(CMS(cms));
}

CMS::CMS(CMS_ContentInfo * cms_)
: cms(cms_)
{

}

CMS::CMS(CMS && other)
{
    this->cms = other.cms;
    other.cms = nullptr;
}

CMS & CMS::operator=(CMS && other)
{
    CMS_ContentInfo_free(this->cms);

    this->cms = other.cms;
    other.cms = nullptr;

    return *this;
}

CMS::~CMS()
{
    CMS_ContentInfo_free(cms);
}

CMS::operator CMS_ContentInfo*()
{
    return cms;
}

void CMS::saveToBIO(BIO * bio) const
{
    if (!i2d_CMS_bio(bio, cms))
    {
        throw OpenSSLException("failed to convert CMS to DER format");
    }
}

bool CMS::verify(STACK_OF(X509) * certs, X509_STORE * store, BIO * indata, BIO * outdata, unsigned int flags, bool is_verbose)
{
    int rc = CMS_verify(cms, certs, store, indata, outdata, flags);
    bool result = (1 == rc);
    if ((!result) && (is_verbose))
    {
        OpenSSLException ex("verification failed");
        std::cerr << "error: " << ex.what() << std::endl;
    }


    return result;
}

std::string CMS::toBase64() const
{
    auto bio = BasicIO::fromMemory();
    saveToBIO(bio);

    BUF_MEM * buffer;
    BIO_get_mem_ptr(bio, &buffer);

    size_t encoded_size = base64_encoded_size(buffer->length);
    std::vector<char> encoded(encoded_size);
    base64_encode(reinterpret_cast<uint8_t *>(buffer->data), buffer->length, encoded.data(), encoded.size());

    return std::string(encoded.data(), encoded.size());
}

std::string CMS::toString() const
{
    auto bio = BasicIO::fromMemory();
    int rc = CMS_ContentInfo_print_ctx(bio, cms, 0, NULL);

    BUF_MEM * buffer;
    BIO_get_mem_ptr(bio, &buffer);

    return std::string(buffer->data, buffer->length);
}

}