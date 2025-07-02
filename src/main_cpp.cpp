
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
#include "display_driver.hpp"
#include "app_touchgfx.h"

#include "test_nvram.hpp"
#include "test_spi_flash.hpp"
#include "test_peripherals.hpp"


 


void heartbeatTask(void *argument);
void UART_Task(void *argument);
void GUI_task(void *argument);
void DISPLAY_task(void *argument);


extern volatile bool _intFlag;
extern "C" {
    #include "sh2.h"
    void touchgfx_signalVSyncTimer(void);  // C-linkage for C code
    }

 extern ST7789 display;

extern bool PeripheralsTestComplete;
extern bool  qspi_dma_tx_done;
extern QSPI_HandleTypeDef hqspi;

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
	.stack_size = 2048 * 1,
	.priority = (osPriority_t)osPriorityNormal

};

osThreadId_t GUI_TaskHandle;
const osThreadAttr_t GUITask_attributes = {
	.name = "GUI TASK",
	.stack_size = 2048 * 1,
	.priority = (osPriority_t)osPriorityNormal

};

osThreadId_t DISPLAY_TaskHandle;
const osThreadAttr_t DISPLAYTask_attributes = {
	.name = "DISPLAY TASK",
	.stack_size = 1024 * 1,
	.priority = (osPriority_t)osPriorityNormal

};

bool TouchGFX_init = false;
 int tickCounter;
 int digitalHours;
 int digitalMinutes;
 int digitalSeconds;
 uint16_t digitalDays;

uint32_t g_tickCounter;
int g_digitalSeconds;

QspiFlash flash(&hqspi);






// extern "C" wrapper for main_cpp
void main_cpp(void)
{
    LOG_INFO("🌟 Aidley Controller Application Started!");

 

    HAL_Delay(100);

    


    osKernelInitialize();


    heartbeatTask_TaskHandle            = osThreadNew(heartbeatTask, NULL, &heartbeatTask_attributes);
    UARTTask_TaskHandle                 = osThreadNew(UART_Task, NULL, &UARTTask_attributes);
    test_peripheralsTask_TaskHandle     = osThreadNew(test_peripheralsTask, NULL, &test_peripheralsTask_attributes);

    GUI_TaskHandle                      = osThreadNew(GUI_task, NULL, &GUITask_attributes);
    DISPLAY_TaskHandle                  = osThreadNew(DISPLAY_task, NULL, &DISPLAYTask_attributes);

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



void DISPLAY_task(void *argument)
{
    uint32_t tick = 0;




while (!TouchGFX_init || !PeripheralsTestComplete) {
    osDelay(10);
}
   
    LOG_INFO("[ST7789] Switching Display Driver to TouchGFX");
    display.init();

    for (;;) {
        
       tick++;
        if (tick >= 16) {
            touchgfx_signalVSyncTimer();
            tick = 0;
        }
        osDelay(1); 
    }
}

void GUI_task(void *argument)
{

    while (!PeripheralsTestComplete) {
    osDelay(10);
}
    
        flash.enableQuadMemoryMappedMode();   

        MX_TouchGFX_Init();
	    /* Call PreOsInit function */
		MX_TouchGFX_PreOSInit();
        TouchGFX_init = true;
		MX_TouchGFX_Process();
    
   
    for (;;) {
      

    }
}




