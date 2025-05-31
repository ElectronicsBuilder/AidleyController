#ifndef __BOOT_FUSE_HPP
#define __BOOT_FUSE_HPP

#include <stdint.h>

#define BOOT_FUSE_ADDR             0x00FF000

#define BOOT_FUSE_SET_BYTE1        0xF0
#define BOOT_FUSE_SET_BYTE2        0x00
#define BOOT_FUSE_SET_BYTE3        0x0A

#define BOOT_FUSE_CLEAR_BYTE1      0xFF
#define BOOT_FUSE_CLEAR_BYTE2      0xFF
#define BOOT_FUSE_CLEAR_BYTE3      0xFF

#define BOOT_FUSE_SIZE             3

#define BOOT_FUSE_SET              1U 
#define BOOT_FUSE_CLEAR            0U

#ifdef __cplusplus
extern "C" {
#endif

uint8_t READ_BOOT_FUSE(void);             // HAL_OK or HAL_ERROR
bool set_boot_fuse(void);
bool clear_boot_fuse(void);

#ifdef __cplusplus
}
#endif

#endif // __BOOT_FUSE_HPP
