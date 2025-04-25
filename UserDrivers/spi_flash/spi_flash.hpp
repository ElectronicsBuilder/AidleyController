// File: UserDrivers/spi_flash/spi_flash.hpp
#ifndef SPI_FLASH_HPP
#define SPI_FLASH_HPP

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct FlashDeviceInfo {
    const char* part_number;
    uint32_t capacity_mbit;
    uint32_t page_size;
    uint32_t sector_size;
    bool supports_quad;
};

FlashDeviceInfo spi_flash_get_info();   // Declare in header


void spi_flash_init(void);
void spi_flash_reset(void);

void spi_flash_write(uint32_t address, const uint8_t *data, size_t length);
void spi_flash_read(uint32_t address, uint8_t *buffer, size_t length);

void spi_flash_read_id(uint8_t *buffer);
void spi_flash_write_enable();

#ifdef __cplusplus
}
#endif

#endif /* SPI_FLASH_HPP */
