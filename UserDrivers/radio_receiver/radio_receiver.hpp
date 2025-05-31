#ifndef __RADIO_RECEIVER_HPP
#define __RADIO_RECEIVER_HPP

#include "stm32f7xx_hal.h"

class RadioReceiver {
public:
    RadioReceiver(GPIO_TypeDef* ppmOePort, uint16_t ppmOePin,
                  GPIO_TypeDef* sbusOePort, uint16_t sbusOePin);

    void enablePPM(bool enable);
    void enableSBUS(bool enable);
    bool isPPMEnabled() const;
    bool isSBUSEnabled() const;

private:
    GPIO_TypeDef* _ppmOePort;
    uint16_t _ppmOePin;

    GPIO_TypeDef* _sbusOePort;
    uint16_t _sbusOePin;
};

#endif
