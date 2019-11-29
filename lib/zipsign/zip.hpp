/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
