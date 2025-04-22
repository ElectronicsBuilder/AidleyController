#include "uart.hpp"
#include "log.hpp"
#include "cmsis_os.h" 
#include "test_uart.hpp"


void uartTask(void *argument)
{
    LOG_INFO("\r\nUART Task Started!");

    uart_init();

    for (;;)
    {

        uart_send("\r\nHello from Aidley Controller FreeRTOS Task!");
        osDelay(1000);  // 1000 ms = 1 sec
    }
}
   