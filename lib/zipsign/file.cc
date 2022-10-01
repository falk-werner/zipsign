/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/file.hpp"
#include "openssl++/exception.hpp"
#include <stdexcept>

#include <sys/stat.h>
#include <sys/types.h>

#if !defined(_WIN32)
#include <sys/sendfile.h>
#include <unistd.h>
#else
#include <io.h>
#endif


#if defined(_WIN32)

namespace
{

int ftruncate(const int fd, const int64_t size)
{
    return _chsize(fd, size);
}

}

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

#if !defined(_WIN32)
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
#else
    constexpr const size_t buf_size = 1024;
    char buf[buf_size];
    size_t length;
    while (0 < (length = fread(buf, 1, buf_size, file)))
    {
        size_t const count = fwrite(buf, 1, length, other.file);
        if (count != length)
        {
            throw std::runtime_error("copy failed");
        }
    }
    if (0 == feof(file))
    {
        throw std::runtime_error("copy failed");
    }
#endif
}

}