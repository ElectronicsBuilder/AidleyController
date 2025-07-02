#include "uart.hpp"
#include "log.hpp"
#include "cmsis_os.h"
#include "task.h"
#include "main.h"
#include "test_peripherals.hpp"
#include "test_nvram.hpp"
#include "test_uart.hpp"
#include "test_spi_flash.hpp"
#include "test_qspi_flash.hpp"
#include "test_SHT4x.hpp"
#include "test_bmp581.hpp"
#include "test_i2c.hpp"
#include "test_bno085.hpp"
#include "test_bq27441.hpp"
#include "test_power_mux.hpp"
#include "test_radio_receiverHW.hpp"
#include "test_st7789.hpp"

bool PeripheralsTestComplete = false;
extern TaskHandle_t test_peripheralsTask_TaskHandle;


static TestConfig testConfig = {
    .test_uart = true,
    .test_nvram = true,
    .test_spi_flash = true,
    .test_qspi_flash = true,
    .test_SHT4x = true,
    .test_BMP581 = true,
    .test_i2c2 = true,
    .test_BNO085 = true,
    .test_BQ27441 = true,
    .test_power_mux = true,
    .test_radio_receiverHW = true,
    .test_continous_data = false,
    .test_st7789 = true
};


void set_test_config(const TestConfig* cfg)
{
    if (cfg) {
        testConfig = *cfg;
    }
}

void test_peripheralsTask(void *argument)
{
    HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_RESET);  // ACTIVE
    LOG_INFO("[DRIVER TEST] Peripherals Test Started");

    if (testConfig.test_uart) test_uart_W();
    if (testConfig.test_nvram) test_nvram_class_driver();
    if (testConfig.test_spi_flash) test_spi_flash_rw();
    if (testConfig.test_qspi_flash) qspi_flash_self_test();
    if (testConfig.test_i2c2) test_i2c2_scan();
    if (testConfig.test_SHT4x) test_sht4x();
    if (testConfig.test_BMP581) test_bmp581();
    if (testConfig.test_BNO085) test_bno085();
    if (testConfig.test_BQ27441) test_bq27441(2000);
    if (testConfig.test_power_mux) test_power_mux(VOUT_3V3);
    if (testConfig.test_radio_receiverHW) test_radio_receiverHW();
    if (testConfig.test_st7789) test_st7789_sequence();

    LOG_INFO("[DRIVER TEST] Peripherals Test Ended");

    HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_SET);  // Done
    PeripheralsTestComplete = true;
    osThreadExit();
}
