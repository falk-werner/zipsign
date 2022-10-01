/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef ZIPSIGN_FTRUNCATE_H
#define ZIPSIGN_FTRUNCATE_H

#if !defined(_WIN32)
#include <unistd.h>
#else
#include <io.h>
#define ftruncate(fd, length) _chsize(fd, length)
#endif


#endif