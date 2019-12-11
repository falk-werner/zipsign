/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef ZIPSIGN_FILE_HPP
#define ZIPSIGN_FILE_HPP

#include <string>
#include <cstdio>

namespace zipsign
{

class File
{
    File operator=(File const &) = delete;
    File(File const &) = delete;
public:
    static void remove(std::string const & filename);
    File(std::string const & name, std::string const & mode);
    ~File();
    void seek(long offset, int whence = SEEK_SET);
    long tell();
    void write(void const * buffer, size_t count);
    size_t read(void * buffer, size_t count, bool check = true);
    void truncate(long offset);
    void copyTo(File & other);
private:
    FILE * file;
};

}

#endif