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

bool PeripheralsTestComplete = false;

extern TaskHandle_t test_peripheralsTask_TaskHandle;

typedef struct {

    bool test_uart;
    bool test_nvram;
    bool test_spi_flash;
    bool test_qspi;

} TestConfig;



void test_peripheralsTask(void *argument)
{
    TestConfig *cfg = (TestConfig *)argument;

    if (cfg->test_nvram) test_uart_W();
    if (cfg->test_nvram) test_nvram_class_driver();
    if (cfg->test_spi_flash) test_spi_flash_rw();
    if (cfg->test_qspi) test_qspi_flash_id();

    PeripheralsTestComplete = true;
    
    osThreadExit();
   
}