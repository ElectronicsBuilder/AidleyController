#include "nvram.hpp"
#include "log.hpp"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f7xx_hal.h"
#include "task.h"
#include "string.h"

#include "test_nvram.hpp"

extern SPI_HandleTypeDef hspi1;


void nvram_init()
{
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(NVRAM_HOLD_GPIO_Port, NVRAM_HOLD_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(NVRAM_WP_GPIO_Port, NVRAM_WP_Pin, GPIO_PIN_SET);
}

void test_nvram_class_driver()
{

    nvram_init();

    NVRAM nvram(&hspi1, NVRAM_CS_GPIO_Port, NVRAM_CS_Pin, NVRAM_HOLD_GPIO_Port, NVRAM_HOLD_Pin, NVRAM_WP_GPIO_Port, NVRAM_WP_Pin);

    auto info = nvram.getDeviceInfo();

    LOG_INFO("[NVRAM] NVRAM: %s, %lu KB, Max Addr: 0x%05lX, AutoStore: %s",
             info.part_number,
             info.capacity_kbyte,
             info.max_address,
             info.has_autostore ? "Yes" : "No");

         
    
             

    const char* message = "Aidley NVRAM!";
    uint8_t readback[32] = {0};
    LOG_INFO("[NVRAM] NVRAM WRITE: %s", message);


    nvram.writeEnable();
    nvram.writeArray(0x0000, (uint8_t*)message, strlen(message));

    osDelay(1); // or poll RDY bit if STORE/RECALL used

    nvram.readArray(0x0000, readback, strlen(message));
    LOG_INFO("[NVRAM] NVRAM Readback: %s", readback);

    if (memcmp(message, readback, strlen(message)) == 0) {
        LOG_INFO("[NVRAM] NVRAM verification passed");
    } else {
        LOG_ERROR("[NVRAM] NVRAM verification FAILED");
    }
}






