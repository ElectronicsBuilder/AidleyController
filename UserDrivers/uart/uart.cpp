#include "uart.hpp"
#include "stm32f7xx_hal.h"
#include "string.h"

extern UART_HandleTypeDef huart1;

void uart_init(void)
{
    // Normally HAL UART Init is done by CubeMX already.
    // No action needed unless custom config.
}

void uart_send(const char* message)
{
    if (message != NULL)
    {
        HAL_UART_Transmit(&huart1, (uint8_t*)message, (uint16_t)strlen(message), 1000);
    }
}
