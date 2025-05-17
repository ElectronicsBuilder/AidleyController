#include "bmp581.hpp"
#include "bmp5.h"
#include "bmp5_defs.h"

BMP581::BMP581(bmp5_read_fptr_t read_func,
            bmp5_write_fptr_t write_func,
            bmp5_delay_us_fptr_t delay_func,
            void* intf_ptr,
            enum bmp5_intf interface_type)
{
    dev_.read = read_func;
    dev_.write = write_func;
    dev_.delay_us = delay_func;
    dev_.intf_ptr = intf_ptr;
    dev_.intf = interface_type;
}

bool BMP581::initialize() {
    //return bmp5_init(&dev_) == BMP5_OK;

    if (bmp5_init(&dev_) != BMP5_OK)
    return false;

    bmp5_soft_reset(&dev_);
    dev_.delay_us(20000, dev_.intf_ptr);

    bmp5_set_power_mode(BMP5_POWERMODE_NORMAL, &dev_);
    dev_.delay_us(1000, dev_.intf_ptr);

    return true;

}

bool BMP581::softReset() {
    return bmp5_soft_reset(&dev_) == BMP5_OK;
}

bool BMP581::getSensorData(float& temperature, float& pressure) {
    bmp5_sensor_data data{};
    bmp5_osr_odr_press_config cfg{};
    bmp5_get_osr_odr_press_config(&cfg, &dev_);

    if (bmp5_get_sensor_data(&data, &cfg, &dev_) == BMP5_OK) {
        temperature = data.temperature;
        pressure = data.pressure;
        return true;
    }
    return false;
}

bool BMP581::configureSensor(const bmp5_osr_odr_press_config& cfg) {
    return bmp5_set_osr_odr_press_config(&cfg, &dev_) == BMP5_OK;
}

bool BMP581::configureIIR(const bmp5_iir_config& cfg) {
    return bmp5_set_iir_config(&cfg, &dev_) == BMP5_OK;
}

bool BMP581::setPowerMode(enum bmp5_powermode mode) {
    return bmp5_set_power_mode(mode, &dev_) == BMP5_OK;
}

bool BMP581::getPowerMode(enum bmp5_powermode& mode) {
    return bmp5_get_power_mode(&mode, &dev_) == BMP5_OK;
}

bool BMP581::getInterruptStatus(uint8_t& status) {
    return bmp5_get_interrupt_status(&status, &dev_) == BMP5_OK;
}

// bool BMP581::enableInterrupts(const bmp5_int_source_select& cfg) {
//     return ::bmp5_int_source_select(&cfg, &dev_) == BMP5_OK;
// }



bool BMP581::setupInterruptPin(enum bmp5_intr_mode mode,
                                enum bmp5_intr_polarity pol,
                                enum bmp5_intr_drive drive,
                                enum bmp5_intr_en_dis enable)
{
    return bmp5_configure_interrupt(mode, pol, drive, enable, &dev_) == BMP5_OK;
}

bmp5_dev* BMP581::dev() {
    return &dev_;
}
