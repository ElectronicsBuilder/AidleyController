#ifndef __TPS2121_HPP
#define __TPS2121_HPP

#include "stm32f7xx_hal.h"

class TPS2121 {
public:
    TPS2121(GPIO_TypeDef* vselPort, uint16_t vselPin);

    enum class InputSource {
        MAIN,
        USB
    };

    InputSource getSelectedInput();

private:
    GPIO_TypeDef* _vselPort;
    uint16_t _vselPin;
};

#endif
