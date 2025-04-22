#include "main_cpp_entry.h"
#include "log.hpp"
#include "test_uart/test_uart.hpp"
#include "qspi_flash/qspi_flash.hpp"
#include "cmsis_os.h"

#include "main.h"
#include "stm32f767xx.h"



void heartbeatTask(void *argument);




osThreadId_t uartTask_TaskHandle;
const osThreadAttr_t uartTask_attributes = {
	.name = "uart Task",
	.stack_size = 512 * 1,
	.priority = (osPriority_t)osPriorityNormal

};


osThreadId_t heartbeatTask_TaskHandle;
const osThreadAttr_t heartbeatTask_attributes = {
	.name = "heartbeat TASK",
	.stack_size = 512 * 1,
	.priority = (osPriority_t)osPriorityNormal

};



// extern "C" wrapper for main_cpp
void main_cpp(void)
{
    LOG_INFO("\r\n🌟 Aidley Controller Application Started!");

    qspi_flash_init(); 

    uint8_t id[3];
    qspi_flash_read_id(id);

    LOG_INFO("\r\nQSPI Flash ID: 0x%02X 0x%02X 0x%02X\r\n", id[0], id[1], id[2]);


    osKernelInitialize();

    uartTask_TaskHandle         = osThreadNew(uartTask, NULL, &uartTask_attributes);
    heartbeatTask_TaskHandle    = osThreadNew(heartbeatTask, NULL, &heartbeatTask_attributes);

    LOG_INFO("\r\nbout to start FreeRTOS kernel...");
    osKernelStart();
    LOG_ERROR("\r\nosKernelStart() returned unexpectedly!\r\n");

    while (1)
    {
    }
}





 void heartbeatTask(void *argument)
{ 
    for (;;)
    {
        HAL_GPIO_TogglePin(GPIOG, LED_HB_Pin); // Blink LED
        osDelay(500);  // 0.5 second
    }
}



