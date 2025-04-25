#ifndef NVRAM_HPP
#define NVRAM_HPP

#include "stm32f7xx_hal.h"

struct NVRAM_StatusRegister {
    uint8_t RDY  : 1;
    uint8_t WEN  : 1;
    uint8_t BP0  : 1;
    uint8_t BP1  : 1;
    uint8_t RSVD1: 1;
    uint8_t RSVD2: 1;
    uint8_t SNL  : 1;
    uint8_t WPEN : 1;
};

typedef struct {
    uint8_t customerID[2];
    uint8_t uniqueSerialNumber[5];
    uint8_t crcChecksum;
    uint8_t readBackCrcChecksum;
} SerialNumber;

class NVRAM {
public:
    NVRAM(SPI_HandleTypeDef* spiHandle, GPIO_TypeDef* csPort, uint16_t csPin, GPIO_TypeDef* holdPort, uint16_t holdPin, GPIO_TypeDef* wpPort, uint16_t wpPin);

    void writeByte(uint32_t address, uint8_t data);
    uint8_t readByte(uint32_t address);
    void writeArray(uint32_t address, uint8_t* data, uint16_t size);
    void readArray(uint32_t address, uint8_t* buffer, uint16_t size);

    void softwareStore();
    void softwareRecall();
    void enableAutoStore();
    void disableAutoStore();

    void writeSerialNumber(SerialNumber* snInfo);
    SerialNumber readSerialNumber();

    uint32_t readDeviceID();
    NVRAM_StatusRegister readStatusRegister();

    void writeEnable();
    void writeDisable();

    void holdEnable();
    void holdDisable();

    void wpEnable();
    void wpDisable();

    uint32_t get_NVRAM_size();
    uint32_t get_DeviceID();

    uint8_t CalculateCRC8(uint8_t* buffer, long length);

private:
    void select();
    void deselect();
    void sendCommand(uint8_t cmd);

    SPI_HandleTypeDef* spiHandle;
    GPIO_TypeDef* csPort;
    uint16_t csPin;
    GPIO_TypeDef* holdPort;
    uint16_t holdPin;
    GPIO_TypeDef* wpPort;
    uint16_t wpPin;

    static constexpr uint8_t CMD_WRITE = 0x02;
    static constexpr uint8_t CMD_READ = 0x03;
    static constexpr uint8_t CMD_WREN = 0x06;
    static constexpr uint8_t CMD_WRDI = 0x04;
    static constexpr uint8_t CMD_RDSR = 0x05;
    static constexpr uint8_t CMD_STORE = 0x3C;
    static constexpr uint8_t CMD_RECALL = 0x60;
    static constexpr uint8_t CMD_AUTO_STORE_EN = 0x59;
    static constexpr uint8_t CMD_AUTO_STORE_DIS = 0x19;
    static constexpr uint8_t CMD_WRSN = 0xC2;
    static constexpr uint8_t CMD_RDSN = 0xC3;
    static constexpr uint8_t CMD_RDID = 0x9F;

    static constexpr uint32_t NVRAM_SIZE = 0x1FFFF;
    static constexpr uint32_t NVRAM_ID = 0x06818820;
    static constexpr uint32_t CRC_DATA_SIZE = 7U;
    static constexpr uint32_t SN_DATA_OFFSET = 1U;
};

#endif // NVRAM_HPP
