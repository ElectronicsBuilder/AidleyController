#include "SHT4x.hpp"
#include "log.hpp" 

#define I2C_TIMEOUT_MS 1000

SHT4x::SHT4x(I2C_HandleTypeDef* i2cHandle)
    : i2cHandle(i2cHandle),
      _temperature(0.0f),
      _humidity(0.0f),
      _precision(SHT4X_HIGH_PRECISION),
      _heater(SHT4X_NO_HEATER)
{}

SHT4x::~SHT4x() {}

bool SHT4x::init()
{
    if (!reset()) {
        LOG_ERROR("[SHT4x] Reset failed during init");
        return false;
    }

    HAL_Delay(10); // <-- Add small 10ms delay after reset

    setPrecision(SHT4X_HIGH_PRECISION);
    setHeater(SHT4X_NO_HEATER);

    return true;
}

bool SHT4x::reset()
{
    return writeCommand(SHT4X_CMD_SOFTRESET);
}

bool SHT4x::readTempAndHumidity(float& temp, float& humidity)
{
    uint8_t buffer[6] = {0};

    if (!triggerMeasurement(SHT4X_CMD_MEASURE_HIGH_PRECISION)) {
        LOG_ERROR("[SHT4x] Failed to start measurement");
        return false;
    }

    HAL_Delay(10); // <-- small delay before reading data!

    if (!readData(buffer, sizeof(buffer))) {
        LOG_ERROR("[SHT4x] Failed to read measurement data");
        return false;
    }

    uint16_t temp_raw = (buffer[0] << 8) | buffer[1];
    uint16_t humidity_raw = (buffer[3] << 8) | buffer[4];

    temp = -45.0f + 175.0f * (float)temp_raw / 65535.0f;
    humidity = 100.0f * (float)humidity_raw / 65535.0f;

    _temperature = temp;
    _humidity = humidity;

    return true;
}

void SHT4x::setPrecision(sht4x_precision_t prec)
{
    _precision = prec;
}

sht4x_precision_t SHT4x::getPrecision() const
{
    return _precision;
}

void SHT4x::setHeater(sht4x_heater_t heat)
{
    _heater = heat;
}

sht4x_heater_t SHT4x::getHeater() const
{
    return _heater;
}

bool SHT4x::writeCommand(uint16_t cmd)
{
    if (HAL_I2C_Master_Transmit(i2cHandle, SHT4X_DEFAULT_ADDR, reinterpret_cast<uint8_t*>(&cmd), 1, I2C_TIMEOUT_MS) != HAL_OK) {
        LOG_ERROR("[SHT4x] Failed to write command 0x%02X", cmd);
        return false;
    }
    return true;
}

bool SHT4x::readData(uint8_t* buffer, uint8_t num_bytes)
{
    if (HAL_I2C_Master_Receive(i2cHandle, SHT4X_DEFAULT_ADDR, buffer, num_bytes, I2C_TIMEOUT_MS) != HAL_OK) {
        LOG_ERROR("[SHT4x] Failed to read data");
        return false;
    }
    return true;
}

bool SHT4x::triggerMeasurement(uint8_t cmd)
{
    return writeCommand(cmd);
}
