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
    HAL_GPIO_WritePin(NVRAM_HOLD_GPIO_Port, NVRAM_HOLD_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(NVRAM_WP_GPIO_Port, NVRAM_WP_Pin, GPIO_PIN_SET);
}

void test_nvram_class_driver()
{
    HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_RESET);  // Active

    nvram_init();

    NVRAM nvram(&hspi1, NVRAM_CS_GPIO_Port, NVRAM_CS_Pin, NVRAM_HOLD_GPIO_Port, NVRAM_HOLD_Pin, NVRAM_WP_GPIO_Port, NVRAM_WP_Pin);

    const char* message = "Aidley NVRAM!";
    uint8_t readback[32] = {0};

    nvram.writeEnable();
    nvram.writeArray(0x0000, (uint8_t*)message, strlen(message));

    osDelay(1); // or poll RDY bit if STORE/RECALL used

    nvram.readArray(0x0000, readback, strlen(message));
    LOG_INFO("NVRAM Readback: %s", readback);
    HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_SET);    // Done
}






