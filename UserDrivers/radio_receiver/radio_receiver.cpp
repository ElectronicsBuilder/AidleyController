#include "radio_receiver.hpp"

RadioReceiver::RadioReceiver(GPIO_TypeDef* ppmOePort, uint16_t ppmOePin,
                             GPIO_TypeDef* sbusOePort, uint16_t sbusOePin)
    : _ppmOePort(ppmOePort), _ppmOePin(ppmOePin),
      _sbusOePort(sbusOePort), _sbusOePin(sbusOePin) {}

void RadioReceiver::enablePPM(bool enable)
{
    HAL_GPIO_WritePin(_ppmOePort, _ppmOePin, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void RadioReceiver::enableSBUS(bool enable)
{
    HAL_GPIO_WritePin(_sbusOePort, _sbusOePin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool RadioReceiver::isPPMEnabled() const
{
    return HAL_GPIO_ReadPin(_ppmOePort, _ppmOePin) == GPIO_PIN_RESET; // active Low signal
}

bool RadioReceiver::isSBUSEnabled() const
{
    return HAL_GPIO_ReadPin(_sbusOePort, _sbusOePin) == GPIO_PIN_SET;
}
