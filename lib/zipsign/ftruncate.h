#ifndef ZIPSIGN_FTRUNCATE_H
#define ZIPSIGN_FTRUNCATE_H

#if !defined(_WIN32)
#include <unistd.h>
#else
#include <io.h>
#define ftruncate(fd, length) _chsize(fd, length)
#endif


#endif