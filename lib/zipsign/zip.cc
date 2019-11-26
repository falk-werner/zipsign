#include "zipsign/zip.hpp"

#include <cstdio>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include <unistd.h>

#define MAX_COMMENT_SIZE (64 * 1024)
#define MIN_EOCD_SIZE (22)
#define EOCD_COMMENT_POS (20)
#define MAX_EOCD_SIZE (MIN_EOCD_SIZE + MAX_COMMENT_SIZE)

namespace
{



}

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
    FILE * file = fopen(filename.c_str(), "rb+");
    if (nullptr == file)
    {
        throw std::runtime_error("failed to open file");        
    }

    fseek(file, commentStart, SEEK_SET);
    uint8_t buffer[2];
    buffer[0] = (uint8_t) (comment.size() & 0xff);
    buffer[1] = (uint8_t) ((comment.size() >> 8) & 0xff);
    fwrite(buffer, 1, 2, file);
    fwrite(comment.data(), 1, comment.size(), file);

    ftruncate(fileno(file), ftell(file));
    fclose(file);
}

std::string Zip::getComment()
{
    size_t commentPos = getCommentStart();
    FILE * file = fopen(filename.c_str(), "rb");
    if (nullptr == file)
    {
        throw std::runtime_error("failed to open file");
    }

    fseek(file, commentPos, SEEK_SET);
    uint8_t buffer[2];
    fread(buffer, 1, 2, file);
    size_t commentLength = (buffer[1] << 8) | buffer[0];

    std::string comment;
    if (commentLength > 0)
    {
        std::vector<char> commentBuffer(commentLength);
        fread(commentBuffer.data(), 1, commentBuffer.size(), file);
        comment = std::string(commentBuffer.data(), commentBuffer.size());        
    }
    
    fclose(file);

    return comment;    
}

std::size_t Zip::getCommentStart()
{
    FILE * file = fopen(filename.c_str(), "rb");
    if (nullptr == file)
    {
        throw std::runtime_error("failed to open file");
    }

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    if (length < MIN_EOCD_SIZE)
    {
        fclose(file);
        throw std::runtime_error("invalid zip archive (too small)");        
    }

    size_t buffer_size = std::min<size_t>(length, MAX_EOCD_SIZE);
    std::vector<uint8_t> buffer(buffer_size);

    size_t offset = length - buffer_size;
    fseek(file, offset, SEEK_SET);
    fread(buffer.data(), 1, buffer.size(), file);

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

    fclose(file);

    if (!found)
    {
        throw std::runtime_error("invalid zip archive: EOCD not found");        
    }

    return offset + pos + EOCD_COMMENT_POS;
}

}