#pragma once

#include <stdint.h>

void uart_init(void);
void uart_send(const char* message);
