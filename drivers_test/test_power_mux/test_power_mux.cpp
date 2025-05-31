#include "test_power_mux.hpp"
#include "tps2115.hpp"
#include "tps2121.hpp"
#include "txs0108.hpp"
#include "log.hpp"
#include "main.h"

extern TIM_HandleTypeDef htim1;

// Replace with actual GPIO ports/pins from your CubeMX config
#define TPS2115_STAT_PORT   VPWM_STAT_GPIO_Port
#define TPS2115_STAT_PIN    VPWM_STAT_Pin

#define TPS2115_D1_PORT     VPWM_D1_GPIO_Port
#define TPS2115_D1_PIN      VPWM_D1_Pin

#define TPS2121_VSEL_PORT   VSEL_OUT_GPIO_Port
#define TPS2121_VSEL_PIN    VSEL_OUT_Pin

#define TXS0108_OE_PORT     VPWM_OE_GPIO_Port
#define TXS0108_OE_PIN      VPWM_OE_Pin

void test_power_mux(VoutSelect selection)
{
    LOG_INFO("[TEST] Power Mux + Level Shifter Test Begin");

    LOG_INFO("[TEST] ESC PWM Output Init");
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1500);
    LOG_INFO("[PWM TEST] TIM1 CH1 started @ 50Hz, pulse = 1.5 ms");

    TPS2115 powerSelector(TPS2115_STAT_PORT, TPS2115_STAT_PIN,
                          TPS2115_D1_PORT, TPS2115_D1_PIN);
    TPS2121 usbMux(TPS2121_VSEL_PORT, TPS2121_VSEL_PIN);
    TXS0108 levelShifter(TXS0108_OE_PORT, TXS0108_OE_PIN);

    // Apply voltage selection
    bool d1Value = (selection == VOUT_3V3);
    powerSelector.setD1(d1Value);

    HAL_Delay(50);

    const char* targetV = d1Value ? "3.3V" : "5V";
    LOG_INFO("[TPS2115] Forcing output to %s (D1 = %d)", targetV, d1Value ? 1 : 0);
    LOG_INFO("[TPS2115] D1 State: %s", powerSelector.getD1State() ? "HIGH (3.3V)" : "LOW (5V)");

    auto source = powerSelector.getActiveInput();
    LOG_INFO("[TPS2115] Output Source: %s", source == TPS2115::InputSource::IN1 ? "IN1 (3.3V)" : "IN2 (5V)");

    auto selected = usbMux.getSelectedInput();
    LOG_INFO("[TPS2121] Output Connected To: %s", selected == TPS2121::InputSource::MAIN ? "3V3_MAIN" : "3V3_USB");

    LOG_INFO("[TXS0108] OE: %s", levelShifter.isEnabled() ? "ENABLED" : "DISABLED");

    levelShifter.disable();
    HAL_Delay(10);
    LOG_INFO("[TXS0108] Disabled");

    levelShifter.enable();
    HAL_Delay(10);
    LOG_INFO("[TXS0108] Enabled");

    LOG_INFO("[TEST] Power Mux + Level Shifter Test Complete");
}

