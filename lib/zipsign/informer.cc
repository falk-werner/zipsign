/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "zipsign/informer.hpp"

#include "zipsign/zip.hpp"
#include "zipsign/partial_input_file.hpp"
#include "zipsign/signature.hpp"

#include <stdexcept>

using openssl::CMS;

namespace zipsign
{

Informer::Informer()
{

}

Informer::~Informer()
{

}

void Informer::print(std::string const & filename, std::ostream & out)
{
    Zip zip(filename);

    auto commentSize = zip.getCommentStart();
    PartialInputFile partialFile;
    auto file = partialFile.open(filename, commentSize);

    auto comment = zip.getComment();
    if (0 != comment.find(ZIPSIGN_SIGNATURE_PREFIX))
    {
        throw std::runtime_error("missing signature");
    }
    auto signature = comment.substr(std::string(ZIPSIGN_SIGNATURE_PREFIX).size());

    auto cms = CMS::fromBase64(signature);
    out << cms.toString() << std::endl;
}

}
