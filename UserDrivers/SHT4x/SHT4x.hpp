#ifndef SHT4X_HPP
#define SHT4X_HPP

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f7xx_hal.h"

#define SHT4X_DEFAULT_ADDR (0x44 << 1)
#define SHT4X_CMD_SOFTRESET 0x94
#define SHT4X_CMD_MEASURE_HIGH_PRECISION 0xFD

typedef enum {
    SHT4X_HIGH_PRECISION,
    SHT4X_MED_PRECISION,
    SHT4X_LOW_PRECISION
} sht4x_precision_t;

typedef enum {
    SHT4X_NO_HEATER,
    SHT4X_HIGH_HEATER_1S,
    SHT4X_HIGH_HEATER_100MS,
    SHT4X_MED_HEATER_1S,
    SHT4X_MED_HEATER_100MS,
    SHT4X_LOW_HEATER_1S,
    SHT4X_LOW_HEATER_100MS
} sht4x_heater_t;

class SHT4x {
public:
    SHT4x(I2C_HandleTypeDef* i2cHandle);
    ~SHT4x();

    bool init();
    bool reset();
    bool readTempAndHumidity(float& temp, float& humidity);

    void setPrecision(sht4x_precision_t prec);
    sht4x_precision_t getPrecision() const;
    void setHeater(sht4x_heater_t heat);
    sht4x_heater_t getHeater() const;

private:
    I2C_HandleTypeDef* i2cHandle;
    float _temperature;
    float _humidity;
    sht4x_precision_t _precision;
    sht4x_heater_t _heater;

    bool writeCommand(uint16_t cmd);
    bool readData(uint8_t* buffer, uint8_t num_bytes);
    bool triggerMeasurement(uint8_t cmd);
};


#ifdef __cplusplus
}
#endif

#endif // SHT4X_HPP
