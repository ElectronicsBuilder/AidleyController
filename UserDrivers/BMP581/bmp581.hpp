#ifndef BMP581_HPP
#define BMP581_HPP

#include "bmp5.h"
#include "bmp5_defs.h"

#ifdef __cplusplus
extern "C" {
#endif


class BMP581 {
public:
    BMP581(bmp5_read_fptr_t read_func,
        bmp5_write_fptr_t write_func,
        bmp5_delay_us_fptr_t delay_func,
        void* intf_ptr,
        enum bmp5_intf interface_type = BMP5_I2C_INTF);

    bool initialize();
    bool softReset();
    bool getSensorData(float& temperature, float& pressure);
    bool configureSensor(const bmp5_osr_odr_press_config& cfg);
    bool configureIIR(const bmp5_iir_config& cfg);
    bool setPowerMode(enum bmp5_powermode mode);
    bool getPowerMode(enum bmp5_powermode& mode);
    bool getInterruptStatus(uint8_t& status);
    //bool enableInterrupts(const bmp5_int_source_select& cfg);
    bool setupInterruptPin(enum bmp5_intr_mode mode, enum bmp5_intr_polarity pol,
                           enum bmp5_intr_drive drive, enum bmp5_intr_en_dis enable);

    bmp5_dev* dev();  // expose for debug or raw access

private:
    bmp5_dev dev_;
};

#ifdef __cplusplus
}
#endif

#endif // BMP581_HPP
