/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/partial_input_file.hpp"
#include <stdexcept>
#include <cstdio>
#include <algorithm>

using openssl::BasicIO;
using openssl::OpenSSLException;

extern "C"
{

struct zipsign_PartialInputFile
{
    FILE * file;
    size_t limit;
    size_t curPos;
};

static int zipsign_PartialInputFile_create(BIO * bio)
{
    auto * data = new zipsign_PartialInputFile({nullptr, 0, 0});
    BIO_set_data(bio, reinterpret_cast<void*>(data));

    return 1;
}

static int zipsign_PartialInputFile_destory(BIO * bio)
{
    auto * data = reinterpret_cast<zipsign_PartialInputFile*>(BIO_get_data(bio));
    fclose(data->file);
    delete data;

    return 1;
}

static int zipsign_PartialInputFile_read(BIO * bio, char * buffer, int count)
{
    auto * data = reinterpret_cast<zipsign_PartialInputFile*>(BIO_get_data(bio));

    size_t n = std::min<size_t>(count, data->limit - data->curPos);
    size_t bytesRead = fread(buffer, 1, n, data->file);
    data->curPos += bytesRead;
    return bytesRead;
}

}

namespace zipsign
{

PartialInputFile::PartialInputFile()
{
    int type = BIO_get_new_index();

    method = BIO_meth_new(type, "ZipSign_PartialInputFile");
    if (nullptr == method)
    {
        throw OpenSSLException("failed to register BIO");
    }

    BIO_meth_set_create(method, zipsign_PartialInputFile_create);
    BIO_meth_set_destroy(method, zipsign_PartialInputFile_destory);
    BIO_meth_set_read(method, zipsign_PartialInputFile_read);
}

PartialInputFile::~PartialInputFile()
{
    BIO_meth_free(method);
}

BasicIO PartialInputFile::open(std::string const & filename, std::size_t upperLimit)
{
    FILE * file = fopen(filename.c_str(), "rb");
    if (nullptr)
    {
        throw std::runtime_error("failed to open file");
    }

    BIO * bio = BIO_new(method);
    if (nullptr == bio)
    {
        throw OpenSSLException("failed to create file;");
    }

    auto * data = reinterpret_cast<zipsign_PartialInputFile*>(BIO_get_data(bio));
    data->file = file;
    data->limit = upperLimit;
    data->curPos= 0;

    BIO_set_init(bio, 1);

    return std::move(BasicIO(bio));
}


}