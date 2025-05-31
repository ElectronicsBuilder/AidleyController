#ifndef __TXS0108_HPP
#define __TXS0108_HPP

#include "stm32f7xx_hal.h"

class TXS0108 {
public:
    TXS0108(GPIO_TypeDef* oePort, uint16_t oePin);

    void enable();
    void disable();
    bool isEnabled() const;

private:
    GPIO_TypeDef* _oePort;
    uint16_t _oePin;
};

#endif
