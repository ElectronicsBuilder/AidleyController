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
bool PeripheralsTestComplete = false;

extern TaskHandle_t test_peripheralsTask_TaskHandle;

typedef struct {

    bool test_uart;
    bool test_nvram;
    bool test_spi_flash;
    bool test_qspi_flash;
    bool test_SHT4x; 
    bool test_BMP581;
    bool test_i2c2;
    bool test_BNO085;
    bool test_BQ27441;
    bool test_power_mux;
    bool test_radio_receiverHW;
} TestConfig;



void test_peripheralsTask(void *argument)
{
    TestConfig *cfg = (TestConfig *)argument;

    HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_RESET);  // ACTIVE

    LOG_INFO("[DRIVER TEST] Peripherals Test Started");

    if (cfg->test_nvram) test_uart_W();                     
    if (cfg->test_nvram) test_nvram_class_driver();          
    if (cfg->test_spi_flash) test_spi_flash_rw();            
    if (cfg->test_qspi_flash) qspi_flash_self_test();   
    if (cfg->test_i2c2) test_i2c2_scan();   
    if (cfg->test_SHT4x) test_sht4x();  
    if (cfg->test_BMP581) test_bmp581();
    if (cfg->test_BNO085) test_bno085();
    if (cfg->test_BQ27441) test_bq27441(2000);
    if (cfg->test_power_mux) test_power_mux(VOUT_3V3);
    if (cfg->test_radio_receiverHW) test_radio_receiverHW();
    

    LOG_INFO("[DRIVER TEST] Peripherals Test Ended");

    HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_SET);  // Done

    PeripheralsTestComplete = true;

    osThreadExit();
   
}