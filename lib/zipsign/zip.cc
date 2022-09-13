/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/zip.hpp"
#include "zipsign/file.hpp"

#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include <vector>

#define MAX_COMMENT_SIZE (64 * 1024)
#define MIN_EOCD_SIZE (22)
#define EOCD_COMMENT_POS (20)
#define MAX_EOCD_SIZE (MIN_EOCD_SIZE + MAX_COMMENT_SIZE)

namespace zipsign
{

Zip::Zip(std::string const & filename_)
: filename(filename_)
{

}

Zip::~Zip()
{

}

void Zip::setComment(std::string const & comment)
{
    if (comment.size() > MAX_COMMENT_SIZE)
    {
        throw std::runtime_error("zip comment too long");
    }

    size_t commentStart = getCommentStart(); 
    File file(filename, "rb+");

    file.seek(commentStart);
    uint8_t buffer[2];
    buffer[0] = (uint8_t) (comment.size() & 0xff);
    buffer[1] = (uint8_t) ((comment.size() >> 8) & 0xff);
    file.write(buffer, 2);
    file.write(comment.data(), comment.size());

    file.truncate(file.tell());
}

std::string Zip::getComment()
{
    size_t commentPos = getCommentStart();
    File file(filename, "rb");

    file.seek(commentPos);
    uint8_t buffer[2];
    file.read(buffer, 2);
    size_t commentLength = (buffer[1] << 8) | buffer[0];

    std::string comment;
    if (commentLength > 0)
    {
        std::vector<char> commentBuffer(commentLength);
        file.read(commentBuffer.data(), commentBuffer.size());
        comment = std::string(commentBuffer.data(), commentBuffer.size());        
    }
    
    return comment;    
}

std::size_t Zip::getCommentStart()
{
    File file(filename, "rb");

    file.seek(0, SEEK_END);
    size_t length = file.tell();
    if (length < MIN_EOCD_SIZE)
    {
        throw std::runtime_error("invalid zip archive (too small)");        
    }

    size_t buffer_size = std::min<size_t>(length, MAX_EOCD_SIZE);
    std::vector<uint8_t> buffer(buffer_size);

    size_t offset = length - buffer_size;
    file.seek(offset);
    file.read(buffer.data(),buffer.size());

    bool found = false;
    size_t pos = buffer_size - MIN_EOCD_SIZE;
    while ((!found) && (pos > 0))
    {
        if (   (buffer[pos + 3] == 0x06)
            && (buffer[pos + 2] == 0x05)
            && (buffer[pos + 1] == 0x4b)
            && (buffer[pos + 0] == 0x50))
        {
            found = true;
        }
        else
        {
            pos--;
        }
    }

    if (!found)
    {
        throw std::runtime_error("invalid zip archive: EOCD not found");        
    }

    return offset + pos + EOCD_COMMENT_POS;
}

}