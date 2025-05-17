#ifndef QSPI_FLASH_HPP
#define QSPI_FLASH_HPP


#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f7xx_hal.h"
#include <stdint.h>
#include <stddef.h>

struct QFlashDeviceInfo {
    const char* part_number;
    uint32_t capacity_mbit;
    uint32_t page_size;
    uint32_t sector_size;
    bool supports_quad;
};



class QspiFlash {
public:
    QspiFlash(QSPI_HandleTypeDef* qspiHandle);

    void init();
    void reset();
    void readID(uint8_t* idBuffer);

    void readData(uint32_t address, uint8_t* buffer, size_t size);
    void readDataQuad(uint32_t address, uint8_t* buffer, size_t size);   
    bool readDataQuadDMA(uint32_t address, uint8_t* buffer, size_t size);

    void writeData(uint32_t address, const uint8_t* data, size_t size);
    void writeDataQuad(uint32_t address, const uint8_t* data, size_t size);
    bool writeDataQuadDMA(uint32_t address, const uint8_t* data, size_t size);


    void eraseSector(uint32_t address);
    void eraseChip();

    void setQuadEnable();
    void enableMemoryMappedMode();
    void enableQuadMemoryMappedMode();
    void enableDualMemoryMappedMode();
    void readIDQuad(uint8_t* idBuffer);
    QFlashDeviceInfo getDeviceInfo();
    bool waitDmaComplete(uint32_t timeout_ms);
    void autoPollingMemReady(uint32_t timeout = HAL_QPSI_TIMEOUT_DEFAULT_VALUE);


private:
    QSPI_HandleTypeDef* qspiHandle;
    void inlineWriteEnable();
   // void autoPollingMemReady(uint32_t timeout = HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
    
    uint8_t getStatus();
};


#ifdef __cplusplus
}
#endif


#endif // QSPI_FLASH_HPP
