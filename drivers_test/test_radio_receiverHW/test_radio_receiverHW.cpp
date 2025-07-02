#include "test_radio_receiverHW.hpp"
#include "radio_receiver.hpp"
#include "log.hpp"
#include "main.h"

// GPIOs defined via CubeMX
#define PPM_OE_PORT     PPM_OE_GPIO_Port
#define PPM_OE_PIN      PPM_OE_Pin

#define SBUS_OE_PORT    SBUS_OE_GPIO_Port
#define SBUS_OE_PIN     SBUS_OE_Pin

extern TIM_HandleTypeDef htim1;

void test_radio_receiverHW()
{
    LOG_INFO("[TEST] Radio Receiver Test Begin");


    // Create radio receiver object
    RadioReceiver receiver(PPM_OE_PORT, PPM_OE_PIN,
                           SBUS_OE_PORT, SBUS_OE_PIN);

    // Enable PPM output to MCU
    receiver.enablePPM(true); // Active Low
    HAL_Delay(50);
    LOG_INFO("[PPM] PPM Output Enabled: %s", receiver.isPPMEnabled() ? "YES" : "NO");

    // Optional SBUS enable
    receiver.enableSBUS(true);
    HAL_Delay(10);
    LOG_INFO("[SBUS] SBUS Output Enabled: %s", receiver.isSBUSEnabled() ? "YES" : "NO");

    LOG_INFO("[TEST] Radio Receiver Test Complete");
}