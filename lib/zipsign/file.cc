/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/file.hpp"
#include "openssl++/exception.hpp"
#include <stdexcept>

#include <sys/stat.h>
#include <sys/types.h>

#ifdef __linux__
#include <sys/sendfile.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <io.h>
#endif

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

#if defined(_WIN32)
    int ftruncate(const int fd, const int64_t size)
    {
        return _chsize(fd, size);
    }
#endif

void File::truncate(long offset)
{
    int rc = ftruncate(fileno(file), offset);
    if (0 != rc)
    {
        throw std::runtime_error("truncate failed");
    }
}

void File::copyTo(File & other)
{
    struct stat buffer;
    int rc = fstat(fileno(file), &buffer);
    if (0 != rc)
    {
        throw std::runtime_error("stat failed");
    }

    size_t remaining = buffer.st_size;

#ifdef __linux__
    while (remaining > 0)
    {
        ssize_t count = sendfile(fileno(other.file), fileno(file), nullptr, remaining);
        if (0 <= count)
        {
            remaining -= count;
        }
        else
        {
            throw std::runtime_error("copy failed");
        }
    }
#elif defined(_WIN32)
    char buff[1024];
    int len;
    while ((len = fread(buff, 1, sizeof(buff), other.file)))
    {
        fwrite(buff, 1, len, file);
    }
#endif
}

}