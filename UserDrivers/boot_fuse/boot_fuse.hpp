#ifndef __BOOT_FUSE_HPP
#define __BOOT_FUSE_HPP

#include <stdint.h>

#include "boot_fuse.hpp"


#define BOOT_FUSE_SET_BYTE1     0x42
#define BOOT_FUSE_SET_BYTE2     0x54
#define BOOT_FUSE_SET_BYTE3     0x4C

#define BOOT_FUSE_CLEAR_BYTE1   0x43
#define BOOT_FUSE_CLEAR_BYTE2   0x4C
#define BOOT_FUSE_CLEAR_BYTE3   0x52

#define BOOT_FUSE_SET        1
#define BOOT_FUSE_CLEAR      0
#define BOOT_FUSE_SIZE       3

#endif // __BOOT_FUSE_HPP
