/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/file.hpp"
#include "openssl++/exception.hpp"
#include <stdexcept>
#include <unistd.h>

namespace zipsign
{

void File::remove(std::string const & filename)
{
    unlink(filename.c_str());
}

File::File(std::string const & name, std::string const & mode)
{
    file = fopen(name.c_str(), mode.c_str());
    if (nullptr == file)
    {
        throw openssl::FileNotFoundException(name);
    }
}

File::~File()
{
    fclose(file);
}

void File::seek(long offset, int whence)
{
    int rc = fseek(file, offset, whence);
    if (0 != rc)
    {
        throw std::runtime_error("fseek failed");
    }
}

long File::tell()
{
    return ftell(file);
}

void File::write(void const * buffer, size_t count)
{
    size_t written = fwrite(buffer, 1, count, file);
    if (written != count)
    {
        throw std::runtime_error("write failed");
    }
}

size_t File::read(void * buffer, size_t count, bool check)
{
    size_t result = fread(buffer, 1, count, file);
    if ((check) && (result != count))
    {
        throw std::runtime_error("read failed");
    }

    return result;
}

void File::truncate(long offset)
{
    int rc = ftruncate(fileno(file), offset);
    if (0 != rc)
    {
        throw std::runtime_error("truncate failed");
    }
}


}