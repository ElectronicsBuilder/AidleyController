#ifndef __QSPI_FLASH_HPP
#define __QSPI_FLASH_HPP


#include "stm32f7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct QFlashDeviceInfo {
    const char* part_number;
    uint32_t capacity_mbit;
    uint32_t page_size;
    uint32_t sector_size;
    bool supports_quad;
};

QFlashDeviceInfo qspi_flash_get_info();   // Declare in header


void qspi_flash_init(void);
void qspi_flash_reset(void);
void qspi_flash_read_id(uint8_t* id_buffer);
void qspi_flash_read_data(uint32_t address, uint8_t* buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* __QSPI_FLASH_HPP */
