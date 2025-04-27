#include "spi_flash.hpp"
#include "w25q128.h"

SpiFlash::SpiFlash(SPI_HandleTypeDef* spiHandle, GPIO_TypeDef* csPort, uint16_t csPin)
    : spiHandle(spiHandle), csPort(csPort), csPin(csPin) {}

void SpiFlash::select() {
    HAL_GPIO_WritePin(csPort, csPin, GPIO_PIN_RESET);
}

void SpiFlash::deselect() {
    HAL_GPIO_WritePin(csPort, csPin, GPIO_PIN_SET);
}

void SpiFlash::sendCommand(uint8_t cmd) {
    HAL_SPI_Transmit(spiHandle, &cmd, 1, HAL_MAX_DELAY);
}

void SpiFlash::init() {
    deselect();
}

void SpiFlash::writeEnable() {
    select();
    sendCommand(0x06); // Write Enable
    deselect();
}

void SpiFlash::writeData(uint32_t address, const uint8_t* data, size_t length) {
    writeEnable();
    select();

    uint8_t cmd[4] = {
        0x02, // Page Program
        static_cast<uint8_t>(address >> 16),
        static_cast<uint8_t>(address >> 8),
        static_cast<uint8_t>(address)
    };

    HAL_SPI_Transmit(spiHandle, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Transmit(spiHandle, const_cast<uint8_t*>(data), length, HAL_MAX_DELAY);

    deselect();
    pollBusy();  // <-- Wait for write to finish
}

void SpiFlash::readData(uint32_t address, uint8_t* buffer, size_t length) {
    select();

    uint8_t cmd[4] = {
        0x03, // Read Data
        static_cast<uint8_t>(address >> 16),
        static_cast<uint8_t>(address >> 8),
        static_cast<uint8_t>(address)
    };

    HAL_SPI_Transmit(spiHandle, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(spiHandle, buffer, length, HAL_MAX_DELAY);

    deselect();
}

void SpiFlash::eraseSector(uint32_t address) {
    writeEnable();
    select();

    uint8_t cmd[4] = {
        0x20, // Sector Erase
        static_cast<uint8_t>(address >> 16),
        static_cast<uint8_t>(address >> 8),
        static_cast<uint8_t>(address)
    };

    HAL_SPI_Transmit(spiHandle, cmd, 4, HAL_MAX_DELAY);

    deselect();
    pollBusy(); // <-- Wait until erase completes
}

void SpiFlash::pollBusy() {
    uint8_t cmd = 0x05; // Read Status Register
    uint8_t status = 0x01; // Assume busy initially

    do {
        select();
        HAL_SPI_Transmit(spiHandle, &cmd, 1, HAL_MAX_DELAY);
        HAL_SPI_Receive(spiHandle, &status, 1, HAL_MAX_DELAY);
        deselect();
    } while (status & 0x01);
}

uint32_t SpiFlash::readDeviceID() {
    uint8_t id[3] = {};

    select();
    sendCommand(0x9F); // JEDEC ID
    HAL_SPI_Receive(spiHandle, id, 3, HAL_MAX_DELAY);
    deselect();

    return (id[0] << 16) | (id[1] << 8) | id[2];
}

FlashDeviceInfo SpiFlash::getDeviceInfo() {
    return {
        "W25Q128JVEIQ",
        128,
        256,
        4096,
        false
    };
}

void SpiFlash::reset() {
    // Reserved for future use
}
