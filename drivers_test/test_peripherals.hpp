
#ifndef __TEST_PERIPHERALS_HPP
#define __TEST_PERIPHERALS_HPP


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    bool test_uart;
    bool test_nvram;
    bool test_spi_flash;
    bool test_qspi_flash;
    bool test_SHT4x; 
    bool test_BMP581;
    bool test_i2c2;
    bool test_BNO085;
    bool test_BQ27441;
    bool test_power_mux;
    bool test_radio_receiverHW;
    bool test_continous_data;
    bool test_st7789;
} TestConfig;


void test_peripheralsTask(void *argument);
void set_test_config(const TestConfig* cfg);

#ifdef __cplusplus
}
#endif



#endif /* __TEST_PERIPHERALS_HPP */
