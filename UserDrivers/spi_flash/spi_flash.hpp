#ifndef SPI_FLASH_HPP
#define SPI_FLASH_HPP


#ifdef __cplusplus
extern "C" {
#endif



#include "stm32f7xx_hal.h"
#include <stdint.h>
#include <stddef.h>

struct FlashDeviceInfo {
    const char* part_number;
    uint32_t capacity_mbit;
    uint32_t page_size;
    uint32_t sector_size;
    bool supports_quad;
};

class SpiFlash {
public:
    SpiFlash(SPI_HandleTypeDef* spiHandle, GPIO_TypeDef* csPort, uint16_t csPin);

    void init();
    void writeEnable();
    void reset(); // Reserved for future use

    void writeData(uint32_t address, const uint8_t* data, size_t length);
    void readData(uint32_t address, uint8_t* buffer, size_t length);
    void eraseSector(uint32_t address);  // <-- NEW METHOD

    uint32_t readDeviceID();
    FlashDeviceInfo getDeviceInfo();

private:
    void select();
    void deselect();
    void sendCommand(uint8_t cmd);
    void pollBusy();  // <-- NEW HELPER

    SPI_HandleTypeDef* spiHandle;
    GPIO_TypeDef* csPort;
    uint16_t csPin;
};


#ifdef __cplusplus
}
#endif


#endif // SPI_FLASH_HPP
