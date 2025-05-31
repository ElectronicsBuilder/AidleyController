#ifndef __TEST_POWER_MUX_HPP
#define __TEST_POWER_MUX_HPP

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VOUT_3V3,
    VOUT_5V
} VoutSelect;

void test_power_mux(VoutSelect selection);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_POWER_MUX_HPP */
