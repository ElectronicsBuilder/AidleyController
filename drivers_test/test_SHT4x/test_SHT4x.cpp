#include "test_SHT4x.hpp"
#include "log.hpp"

extern I2C_HandleTypeDef hi2c2;
// Create global or static instance
SHT4x sht4x(&hi2c2);

bool test_sht4x()
{
    LOG_INFO("[TEST] Starting SHT4x sensor test...");

    if (!sht4x.init()) {
        LOG_ERROR("[TEST] SHT4x init failed!");
        return false;
    }

    float temperature = 0.0f;
    float humidity = 0.0f;

    if (!sht4x.readTempAndHumidity(temperature, humidity)) {
        LOG_ERROR("[TEST] Failed to read from SHT4x sensor!");
        return false;
    }

    LOG_INFO("[TEST] SHT4x Temperature: %.2f °C", temperature);
    LOG_INFO("[TEST] SHT4x Humidity: %.2f %%", humidity);

    LOG_INFO("[TEST] SHT4x sensor test passed!");
    return true;
}
