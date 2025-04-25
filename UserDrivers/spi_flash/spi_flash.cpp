#include "spi_flash.hpp"
#include "main.h"
#include "stm32f7xx_hal.h"

extern SPI_HandleTypeDef hspi1;



void spi_flash_init() {
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_HOLD_GPIO_Port, FLASH_HOLD_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_WP_GPIO_Port, FLASH_WP_Pin, GPIO_PIN_SET);
}


void spi_flash_write(uint32_t address, const uint8_t *data, size_t length) {
    uint8_t cmd[4] = { 0x02, (uint8_t)(address >> 16), (uint8_t)(address >> 8), (uint8_t)address };
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)data, length, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
}

void spi_flash_read(uint32_t address, uint8_t *buffer, size_t length) {
    uint8_t cmd[4] = { 0x03, (uint8_t)(address >> 16), (uint8_t)(address >> 8), (uint8_t)address };
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, buffer, length, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
}


void spi_flash_write_enable() {
    uint8_t cmd = 0x06; // Write Enable

    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
}


void spi_flash_read_id(uint8_t *id_buf)
{
    uint8_t cmd = 0x9F; // JEDEC ID
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, id_buf, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
}

FlashDeviceInfo spi_flash_get_info() {
    FlashDeviceInfo info = {
        .part_number = "W25Q128JVEIQ",
        .capacity_mbit = 128,
        .page_size = 256,
        .sector_size = 4096,
        .supports_quad = 0
    };
    return info;
}
