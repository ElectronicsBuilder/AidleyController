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

bool PeripheralsTestComplete = false;

extern TaskHandle_t test_peripheralsTask_TaskHandle;

typedef struct {

    bool test_uart;
    bool test_nvram;
    bool test_spi_flash;
    bool test_qspi_flash;
    bool test_SHT4x; 

} TestConfig;



void test_peripheralsTask(void *argument)
{
    TestConfig *cfg = (TestConfig *)argument;

    HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_RESET);  // ACTIVE

    if (cfg->test_nvram) test_uart_W();                     
    if (cfg->test_nvram) test_nvram_class_driver();          
    if (cfg->test_spi_flash) test_spi_flash_rw();            
    if (cfg->test_qspi_flash) qspi_flash_self_test();      
    if (cfg->test_SHT4x) test_sht4x();  
 
    HAL_GPIO_WritePin(LED_ACTY_GPIO_Port, LED_ACTY_Pin, GPIO_PIN_SET);  // Done

    PeripheralsTestComplete = true;

    osThreadExit();
   
}