#ifndef BNO085_HPP
#define BNO085_HPP

#include "sh2.h"
#include "sh2_SensorValue.h"
#include "sh2_err.h"
#include "stm32f7xx_hal.h"
#include <cstdint>

class BNO085 {
public:
    BNO085();
    ~BNO085();

    bool begin_SPI(SPI_HandleTypeDef* spiHandle,
                   GPIO_TypeDef* csPort, uint16_t csPin,
                   GPIO_TypeDef* intPort, uint16_t intPin,
                   GPIO_TypeDef* resetPort, uint16_t resetPin);

    bool enableReport(sh2_SensorId_t sensorId, uint32_t interval_us);
    bool getSensorEvent(sh2_SensorValue_t* value);
    void hardwareReset();
    void debugSpiDump(const char* label, const uint8_t* data, size_t len);
    bool wasReset();

    int spihalOpen(sh2_Hal_t* self);
    void spihalClose(sh2_Hal_t* self);
    int spihalRead(sh2_Hal_t* self, uint8_t* buf, unsigned len, uint32_t* t_us);
    int spihalWrite(sh2_Hal_t* self, uint8_t* buf, unsigned len);
    bool waitForInt();

    static int spihalOpenStatic(sh2_Hal_t* self);
    static void spihalCloseStatic(sh2_Hal_t* self);
    static int spihalReadStatic(sh2_Hal_t* self, uint8_t* buf, unsigned len, uint32_t* t_us);
    static int spihalWriteStatic(sh2_Hal_t* self, uint8_t* buf, unsigned len);

    sh2_ProductIds_t prodIds;
    sh2_Hal_t _HAL;
    sh2_SensorValue_t _sensorValue;
    bool _resetOccurred;

private:
    bool _init();

    SPI_HandleTypeDef* _spi;
    GPIO_TypeDef* _csPort;
    uint16_t _csPin;
    GPIO_TypeDef* _intPort;
    uint16_t _intPin;
    GPIO_TypeDef* _resetPort;
    uint16_t _resetPin;
};

extern "C" uint32_t hal_getTimeUs(sh2_Hal_t* self);
extern "C" void hal_callback(void* cookie, sh2_AsyncEvent_t* pEvent);
extern "C" void sensorHandler(void* cookie, sh2_SensorEvent_t* event);

#endif // BNO085_HPP