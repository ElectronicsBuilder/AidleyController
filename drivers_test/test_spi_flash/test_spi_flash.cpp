#include "spi_flash.hpp"
#include "log.hpp"
#include "cmsis_os.h"
#include "task.h"
#include "string.h"
#include "test_spi_flash.hpp"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

void test_spi_flash_rw()
{
    SpiFlash spiFlash(&hspi1, FLASH_CS_GPIO_Port, FLASH_CS_Pin);

    spiFlash.init();

    const uint32_t addr = 0x0000;
    const char* msg = "Aidley!";
    uint8_t buf[16] = {0};

    FlashDeviceInfo info = spiFlash.getDeviceInfo();
    LOG_INFO("SPI Flash: %s, %lu Mbit", info.part_number, info.capacity_mbit);
    LOG_INFO("Page Size: %lu bytes, Sector Size: %lu bytes, Quad: %s",
             info.page_size,
             info.sector_size,
             info.supports_quad ? "Yes" : "No");

    // --- Erase Sector ---
    spiFlash.eraseSector(addr);

    // --- Write Data ---
    spiFlash.writeData(addr, reinterpret_cast<const uint8_t*>(msg), strlen(msg));

    // --- Read Data ---
    spiFlash.readData(addr, buf, strlen(msg));
    LOG_INFO("Read from Flash: %s", buf);
}
