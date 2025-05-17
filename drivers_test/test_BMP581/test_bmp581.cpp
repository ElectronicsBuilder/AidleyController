#include "test_bmp581.hpp"
#include "bmp581.hpp"
#include "bmp5.h"
#include "bmp5_defs.h"
#include "log.hpp"
#include "main.h"
#include "stm32f7xx_hal.h"

extern I2C_HandleTypeDef hi2c2;

static int8_t bmp581_i2c_read(uint8_t reg_addr, uint8_t* data, uint32_t len, void* intf_ptr) {
    uint8_t addr = *(reinterpret_cast<uint8_t*>(intf_ptr));
    return HAL_I2C_Mem_Read(&hi2c2, addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 100) == HAL_OK ? 0 : -1;
}

static int8_t bmp581_i2c_write(uint8_t reg_addr, const uint8_t* data, uint32_t len, void* intf_ptr) {
    uint8_t addr = *(reinterpret_cast<uint8_t*>(intf_ptr));
    return HAL_I2C_Mem_Write(&hi2c2, addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, const_cast<uint8_t*>(data), len, 100) == HAL_OK ? 0 : -1;
}

static void bmp581_delay_us(uint32_t us, void*) {
    HAL_Delay((us + 999) / 1000);  // round up to 1 ms if < 1ms
}

void test_bmp581()
{
    LOG_INFO("[BMP581] BMP581 Test Starting");

    static uint8_t bmp581_addr = 0x47;  // Use 0x47 if SDO is pulled high


    BMP581 bmp(bmp581_i2c_read, bmp581_i2c_write, bmp581_delay_us, &bmp581_addr, BMP5_I2C_INTF);
 
    if (!bmp.initialize()) {
        LOG_ERROR("[BMP581] Initialization failed");
        return;
    }
    
    bmp581_delay_us(20000, nullptr);
    
    uint8_t chip_id = 0;

    int8_t rslt = bmp5_get_regs(BMP5_REG_CHIP_ID, &chip_id, 1, bmp.dev());
    if (rslt == BMP5_OK) {
        //LOG_INFO("CHIP_ID: 0x%02X", chip_id);
    } else {
        LOG_ERROR("bmp5_get_regs failed: %d", rslt);
    }


   

    if (bmp5_get_regs(BMP5_REG_CHIP_ID, &chip_id, 1, bmp.dev()) == BMP5_OK) {
        LOG_INFO("[BMP581] Chip ID: 0x%02X", chip_id);

        if (chip_id == BMP5_CHIP_ID_PRIM || chip_id == BMP5_CHIP_ID_SEC) {
            LOG_INFO("[BMP581] Chip ID is valid");
        } else {
            LOG_WARN("[BMP581] Chip ID unexpected");
        }
    } else {
        LOG_ERROR("[BMP581] Failed to read chip ID");
    }

    bmp581_delay_us(20000, nullptr); // Wait for stabilization

    bmp5_osr_odr_press_config sensor_config;
    sensor_config.osr_t = BMP5_OVERSAMPLING_4X;
    sensor_config.osr_p = BMP5_OVERSAMPLING_4X;
    sensor_config.odr = BMP5_ODR_25_HZ;
    sensor_config.press_en = BMP5_ENABLE;

    if (bmp5_set_osr_odr_press_config(&sensor_config, bmp.dev()) != BMP5_OK) {
        LOG_ERROR("[BMP581] Sensor configuration failed");
        return;
    }

    bmp581_delay_us(5000, nullptr);  // Allow new config to take effect

    float temp = 0.0f, pressure = 0.0f;
    if (bmp.getSensorData(temp, pressure)) {
        LOG_INFO("[BMP581] Temperature: %.2f °C", temp);
        LOG_INFO("[BMP581] Pressure: %.2f Pa", pressure);

        const float seaLevelPressure = 101325.0f; // Pa
        float altitude = 44330.0f * (1.0f - powf(pressure / seaLevelPressure, 0.1903f));

        LOG_INFO("[BMP581] Estimated Altitude: %.2f meters", altitude);
    } else {
        LOG_ERROR("[BMP581] Failed to read sensor data");
    }

}
