
#include "main_cpp_entry.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f767xx.h"
#include "main.h"

#include "log.hpp"
#include "qspi_flash.hpp"
#include "test_uart.hpp"

#include "test_nvram.hpp"
#include "test_spi_flash.hpp"
#include "test_peripherals.hpp"

void heartbeatTask(void *argument);
extern bool  qspi_dma_tx_done;

osThreadId_t heartbeatTask_TaskHandle;
const osThreadAttr_t heartbeatTask_attributes = {
	.name = "heartbeat TASK",
	.stack_size = 512 * 1,
	.priority = (osPriority_t)osPriorityNormal

};

osThreadId_t test_peripheralsTask_TaskHandle;
const osThreadAttr_t test_peripheralsTask_attributes = {
	.name = "Test Peripherals TASK",
	.stack_size = 1024 * 8,
	.priority = (osPriority_t)osPriorityNormal

};


extern bool PeripheralsTestComplete;




// extern "C" wrapper for main_cpp
void main_cpp(void)
{
    LOG_INFO("🌟 Aidley Controller Application Started!");

 

    HAL_Delay(2000);
    osKernelInitialize();


    heartbeatTask_TaskHandle            = osThreadNew(heartbeatTask, NULL, &heartbeatTask_attributes);
    test_peripheralsTask_TaskHandle     = osThreadNew(test_peripheralsTask, NULL, &test_peripheralsTask_attributes);


    LOG_INFO("About to start FreeRTOS kernel...");
    osKernelStart();
    LOG_ERROR("osKernelStart() returned unexpectedly!\r\n");

    while (1)
    {
    }
}





 void heartbeatTask(void *argument)
{ 
    HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_SET);// clear Error LED
    
    for (;;)
    {
        HAL_GPIO_TogglePin(GPIOG, LED_HB_Pin); // Blink LED
        osDelay(500);  // 0.5 seconds
    }
}



extern "C" void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
    qspi_dma_tx_done = true; // A global or static volatile flag
}

extern "C" void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
    qspi_dma_tx_done = true;
}



