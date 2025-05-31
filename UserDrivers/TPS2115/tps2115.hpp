#ifndef __TPS2115_HPP
#define __TPS2115_HPP

#include "stm32f7xx_hal.h"

class TPS2115 {
public:
    enum class InputSource {
        IN1, // 3.3V
        IN2  // 5V
    };

    TPS2115(GPIO_TypeDef* statPort, uint16_t statPin,
            GPIO_TypeDef* d1Port, uint16_t d1Pin);

    void setD1(bool level);                 // true = 3.3V, false = 5V
    bool getD1State() const;               // read back D1 pin state
    InputSource getActiveInput();          // from STAT pin

private:
    GPIO_TypeDef* _statPort;
    uint16_t _statPin;
    GPIO_TypeDef* _d1Port;
    uint16_t _d1Pin;
};

#endif
