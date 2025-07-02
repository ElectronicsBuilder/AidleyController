#ifndef __BOOT_FUSE_QSPI_FLASH_HPP
#define __BOOT_FUSE_QSPI_FLASH_HPP

#include <stdint.h>
#include "boot_fuse.hpp"

#ifdef __cplusplus
extern "C" {
#endif

 bool qspiFlash_set_fuse();

#ifdef __cplusplus
}
#endif

#endif /* __BOOT_FUSE_QSPI_FLASH_HPP */
