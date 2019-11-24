/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef OPENSSL_INIT_HPP
#define OPENSSL_INIT_HPP

namespace openssl
{

class OpenSSL
{
public:
    //-----------------------------------------------------------------------------
    /// Initialized OpenSSL library.
    ///
    /// \ņote The application will be terminated, if initialization fails.
    //-----------------------------------------------------------------------------
    static void init(void);
};

}

#endif
