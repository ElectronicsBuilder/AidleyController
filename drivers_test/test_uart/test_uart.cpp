#include "uart.hpp"
#include "log.hpp"
#include "cmsis_os.h" 
#include "test_uart.hpp"
#include "task.h"
#include "main.h"

#include "test_nvram.hpp"


void test_uart_W()
{
    LOG_INFO("UART Test Started!");

    uart_init();

    HAL_GPIO_WritePin(LED_COMM_GPIO_Port, LED_COMM_Pin, GPIO_PIN_RESET);  // Active
    LOG_INFO("[TEST] Hello from Aidley Controller FreeRTOS Task!");
    HAL_GPIO_WritePin(LED_COMM_GPIO_Port, LED_COMM_Pin, GPIO_PIN_SET);  // Done
}