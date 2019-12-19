/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef ZIPSIGN_INFORMER_HPP
#define ZIPSIGN_INFORMER_HPP

#include <string>
#include <iostream>

namespace zipsign
{

class Informer
{
    Informer operator=(Informer const &) = delete;
    Informer(Informer const &) = delete;
public:
    Informer();
    ~Informer();
    void print(std::string const & filename, std::ostream & out = std::cout);
};

}

#endif
