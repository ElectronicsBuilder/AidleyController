#include "spi_flash.hpp"
#include "log.hpp"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f7xx_hal.h"
#include "task.h"
#include "string.h"
#include "test_spi_flash.hpp"

extern SPI_HandleTypeDef hspi1;

void test_spi_flash_rw()
{
    const uint32_t addr = 0x0000;
    const char* msg = "Aidley!";
    uint8_t buf[16] = {0};

    FlashDeviceInfo info = spi_flash_get_info();
    
    LOG_INFO("SPI Flash: %s, %lu Mbit", info.part_number, info.capacity_mbit);
    LOG_INFO("Page Size: %lu bytes, Sector Size: %lu bytes, Quad: %s",
             info.page_size,
             info.sector_size,
             info.supports_quad ? "Yes" : "No");

    // Erase
    spi_flash_write_enable();
    uint8_t erase_cmd[4] = { 0x20, 0x00, 0x00, 0x00 };
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, erase_cmd, 4, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);

    // Poll for erase completion
    uint8_t cmd = 0x05, status = 0x01;
    do {
        HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
        HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi1, &status, 1, HAL_MAX_DELAY);
        HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
    } while (status & 0x01);

    // Write
    spi_flash_write_enable();
    spi_flash_write(addr, (const uint8_t*)msg, strlen(msg));

    osDelay(2);  // Optional, or poll again if you want to be safe

    // Read
    spi_flash_read(addr, buf, strlen(msg));
    LOG_INFO("Read from Flash: %s", buf);
}


// void test_spi_flash_task(void *argument)
// {
//     spi_flash_init();

//     osDelay(2); 

//     uint8_t id[3] = {0};
//     spi_flash_read_id(id);

//     LOG_INFO("\r\nSPI Flash ID: 0x%02X 0x%02X 0x%02X", id[0], id[1], id[2]);

//     HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_RESET);  // Active
//     osDelay(500);
//     HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_SET);    // Done

//     test_spi_flash_rw();

//     vTaskDelete(NULL);
// }
