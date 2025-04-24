#ifndef __UART_HPP
#define __UART_HPP

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

void uart_init(void);
void uart_send(const char* message);


#ifdef __cplusplus
}
#endif

#endif /* __UART_HPP */