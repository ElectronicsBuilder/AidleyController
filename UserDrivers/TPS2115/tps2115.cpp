#include "tps2115.hpp"

TPS2115::TPS2115(GPIO_TypeDef* statPort, uint16_t statPin,
                 GPIO_TypeDef* d1Port, uint16_t d1Pin)
    : _statPort(statPort), _statPin(statPin),
      _d1Port(d1Port), _d1Pin(d1Pin) {}

void TPS2115::setD1(bool level)
{
    HAL_GPIO_WritePin(_d1Port, _d1Pin, level ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool TPS2115::getD1State() const
{
    return HAL_GPIO_ReadPin(_d1Port, _d1Pin) == GPIO_PIN_SET;
}

TPS2115::InputSource TPS2115::getActiveInput()
{
    GPIO_PinState state = HAL_GPIO_ReadPin(_statPort, _statPin);
    return (state == GPIO_PIN_SET) ? InputSource::IN2 : InputSource::IN1;
}
