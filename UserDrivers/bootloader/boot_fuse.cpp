#include "boot_fuse.hpp"
#include "spi_flash.hpp"
#include "main.h"  
#include "string.h"

#include "stm32f7xx_hal.h"

extern SPI_HandleTypeDef hspi1;

static SpiFlash spi(&hspi1, FLASH_CS_GPIO_Port, FLASH_CS_Pin);  // 

static const uint8_t boot_fuse_expected[BOOT_FUSE_SIZE] = {
    BOOT_FUSE_SET_BYTE1,
    BOOT_FUSE_SET_BYTE2,
    BOOT_FUSE_SET_BYTE3
};

static const uint8_t boot_fuse_cleared[BOOT_FUSE_SIZE] = {
    BOOT_FUSE_CLEAR_BYTE1,
    BOOT_FUSE_CLEAR_BYTE2,
    BOOT_FUSE_CLEAR_BYTE3
};

uint8_t READ_BOOT_FUSE(void)
{
    uint8_t read_fuse[BOOT_FUSE_SIZE] = {0};
    spi.readData(BOOT_FUSE_ADDR, read_fuse, BOOT_FUSE_SIZE);

    return (memcmp(read_fuse, boot_fuse_expected, BOOT_FUSE_SIZE) == 0) ? BOOT_FUSE_SET : BOOT_FUSE_CLEAR;
}

bool set_boot_fuse(void)
{
    spi.writeData(BOOT_FUSE_ADDR, boot_fuse_expected, BOOT_FUSE_SIZE);

    uint8_t verify[BOOT_FUSE_SIZE] = {0};
    spi.readData(BOOT_FUSE_ADDR, verify, BOOT_FUSE_SIZE);

    return memcmp(verify, boot_fuse_expected, BOOT_FUSE_SIZE) == 0;
}

bool clear_boot_fuse(void)
{
    spi.writeData(BOOT_FUSE_ADDR, boot_fuse_cleared, BOOT_FUSE_SIZE);

    uint8_t verify[BOOT_FUSE_SIZE] = {0};
    spi.readData(BOOT_FUSE_ADDR, verify, BOOT_FUSE_SIZE);

    return memcmp(verify, boot_fuse_cleared, BOOT_FUSE_SIZE) == 0;
}