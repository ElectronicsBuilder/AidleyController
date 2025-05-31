#include "txs0108.hpp"

TXS0108::TXS0108(GPIO_TypeDef* oePort, uint16_t oePin)
    : _oePort(oePort), _oePin(oePin) {}

void TXS0108::enable() {
    HAL_GPIO_WritePin(_oePort, _oePin, GPIO_PIN_SET);
}

void TXS0108::disable() {
    HAL_GPIO_WritePin(_oePort, _oePin, GPIO_PIN_RESET);
}

bool TXS0108::isEnabled() const {
    return HAL_GPIO_ReadPin(_oePort, _oePin) == GPIO_PIN_SET;
}
