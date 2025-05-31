#include "tps2121.hpp"

TPS2121::TPS2121(GPIO_TypeDef* vselPort, uint16_t vselPin)
    : _vselPort(vselPort), _vselPin(vselPin) {}

TPS2121::InputSource TPS2121::getSelectedInput() {
    GPIO_PinState state = HAL_GPIO_ReadPin(_vselPort, _vselPin);
    return (state == GPIO_PIN_SET) ? InputSource::USB : InputSource::MAIN;
}
