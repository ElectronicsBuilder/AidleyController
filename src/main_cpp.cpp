
#include "main_cpp_entry.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f767xx.h"
#include "main.h"

#include "log.hpp"
#include "uart.hpp"
#include "qspi_flash.hpp"
#include "test_uart.hpp"


#include "test_nvram.hpp"
#include "test_spi_flash.hpp"
#include "test_peripherals.hpp"

extern volatile bool _intFlag;
extern "C" {
    #include "sh2.h"
    }

void heartbeatTask(void *argument);
void UART_Task(void *argument);

extern bool  qspi_dma_tx_done;

osThreadId_t heartbeatTask_TaskHandle;
const osThreadAttr_t heartbeatTask_attributes = {
	.name = "heartbeat TASK",
	.stack_size = 2048 * 1,
	.priority = (osPriority_t)osPriorityNormal

};

osThreadId_t UARTTask_TaskHandle;
const osThreadAttr_t UARTTask_attributes = {
	.name = "UART TASK",
	.stack_size = 2048 * 1,
	.priority = (osPriority_t)osPriorityNormal

};

osThreadId_t test_peripheralsTask_TaskHandle;
const osThreadAttr_t test_peripheralsTask_attributes = {
	.name = "Test Peripherals TASK",
	.stack_size = 2048 * 5,
	.priority = (osPriority_t)osPriorityNormal

};


extern bool PeripheralsTestComplete;




// extern "C" wrapper for main_cpp
void main_cpp(void)
{
    LOG_INFO("🌟 Aidley Controller Application Started!");

 

    HAL_Delay(100);



 


    osKernelInitialize();


    heartbeatTask_TaskHandle            = osThreadNew(heartbeatTask, NULL, &heartbeatTask_attributes);
    UARTTask_TaskHandle                 = osThreadNew(UART_Task, NULL, &UARTTask_attributes);
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
    HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_SET); // Clear Error LED


    for (;;) {
  
        osDelay(500);  // Run every ~500 ms
        HAL_GPIO_TogglePin(LED_HB_GPIO_Port, LED_HB_Pin);  
    
    }
}

void UART_Task(void *argument)
{
   osDelay(1000);
   uart_init_rx_dma(); 


    for (;;) {
  
        osDelay(1);  // Run every ~1 ms
        uart_dma_poll(); 
    
    }
}



// extern "C" void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
// {
//     qspi_dma_tx_done = true; // A global or static volatile flag
// }

// extern "C" void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef *hqspi)
// {
//     qspi_dma_tx_done = true;
// }





extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == DOF_INT_Pin)
    {
        _intFlag = true;
    }
}