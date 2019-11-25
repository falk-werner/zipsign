#ifndef ZIPSIGN_ZIP_HPP
#define ZIPSIGN_ZIP_HPP

#include <string>
#include <cstddef>

namespace zipsign
{
class Zip
{
public:
    Zip(std::string const & filename_);
    ~Zip();
    void setComment(std::string const & comment);
    std::string getComment();
    std::size_t getCommentStart();
private:
    std::string filename;
};
}


#endif
