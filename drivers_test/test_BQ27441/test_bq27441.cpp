#include "test_bq27441.hpp"
#include "bq27441.hpp"
#include "log.hpp"
#include "BQ27441_Definitions.h"
#include "main.h"
#include "stm32f7xx_hal.h"

#define BATTERY_CAPACITY     2200   // mAh
#define TERMINATE_VOLTAGE    3000   // mV
#define TAPER_CURRENT        15     // mA
#define CONSTRAIN(x, low, high) (((x) < (low)) ? (low) : (((x) > (high)) ? (high) : (x)))

extern I2C_HandleTypeDef hi2c2;


void decodeBQ27441Flags(uint16_t flags)
{


    LOG_INFO("[BQ27441] CONTROL_STATUS: 0x%04X", flags);

    if (flags & BQ27441_FLAG_OT)         LOG_WARN("[BQ27441] ⚠ Over Temperature");
    if (flags & BQ27441_FLAG_UT)         LOG_WARN("[BQ27441] ⚠ Under Temperature");
    if (flags & BQ27441_FLAG_FC)         LOG_INFO("[BQ27441] ✅ Full Charge");
    if (flags & BQ27441_FLAG_CHG)        LOG_INFO("[BQ27441] ⚡ Charging");
    if (flags & BQ27441_FLAG_OCVTAKEN)   LOG_INFO("[BQ27441] 🧪 OCV Taken");
    if (flags & BQ27441_FLAG_ITPOR)      LOG_INFO("[BQ27441] 🔄 ITPOR (Power-On Reset)");
    if (flags & BQ27441_FLAG_CFGUPMODE)  LOG_INFO("[BQ27441] ⚙ Config Update Mode");
    if (flags & BQ27441_FLAG_BAT_DET)    LOG_INFO("[BQ27441] 🔋 Battery Detected");
    if (flags & BQ27441_FLAG_SOC1)       LOG_WARN("[BQ27441] ⚠ SOC1 Warning");
    if (flags & BQ27441_FLAG_SOCF)       LOG_ERROR("[BQ27441] ❗ SOCF Critical");
    if (flags & BQ27441_FLAG_DSG)        LOG_INFO("[BQ27441] 🔋 Discharging");
}



void test_bq27441(uint16_t designCapacity_mAh)
{
    LOG_INFO("[BQ27441] Test Start");

    BQ27441 gauge(&hi2c2);
    if (!gauge.init()) {
        LOG_ERROR("[BQ27441] Initialization failed");
        return;
    }

   gauge.executeControlWord(BQ27441_CONTROL_SOFT_RESET);  // SOFT_RESET
 
   HAL_Delay(50);
 

    uint16_t flags = gauge.flags();  
    decodeBQ27441Flags(flags);    

    bool config_written = false;

   if (gauge.itporFlag()) {
        LOG_INFO("[BQ27441] Writing gauge config...");
        config_written = true;

        gauge.enterConfig();
        gauge.setCapacity(designCapacity_mAh);
        gauge.setDesignEnergy(static_cast<uint16_t>(designCapacity_mAh * 3.7f));
        gauge.setTerminateVoltage(TERMINATE_VOLTAGE);
        gauge.setTaperRate(10 * designCapacity_mAh / TAPER_CURRENT);

        uint8_t set_threshold = CONSTRAIN(10, 0, 100);
        uint8_t clear_threshold = CONSTRAIN(15, 0, 100);
        gauge.setSOC1Thresholds(set_threshold, clear_threshold);

        gauge.setFCThresholds(100, 99);

        gauge.exitConfig();
 }
   else {
    LOG_INFO("[BQ27441] Using existing gauge config");
   }


  
    LOG_INFO("[BQ27441] Voltage: %u mV", gauge.voltage());
    LOG_INFO("[BQ27441] SOC: %u %%", gauge.soc(UNFILTERED));
    LOG_INFO("[BQ27441] Temp INTERNAL: %.2f °C", gauge.temperature(INTERNAL_TEMP));
    LOG_INFO("[BQ27441] Temp BATTERY: %.2f °C", gauge.temperature(BATTERY));
    LOG_INFO("[BQ27441] Current: %d mA", gauge.current(AVG));
    LOG_INFO("[BQ27441] Remaining Capacity: %u mAh", gauge.capacity(REMAIN));
    LOG_INFO("[BQ27441] Full Capacity: %u mAh", gauge.capacity(FULL));
    LOG_INFO("[BQ27441] Design Capacity: %u mAh", gauge.capacity(DESIGN));


    LOG_INFO("[BQ27441] SOC1 Set Threshold: %u %%", gauge.SOC1SetThreshold());
    LOG_INFO("[BQ27441] SOC1 Clear Threshold: %u %%", gauge.SOC1ClearThreshold());

    LOG_INFO("[BQ27441] FC Set Threshold: %d %%", (int8_t)gauge.FCSetThreshold());
    LOG_INFO("[BQ27441] FC Clear Threshold: %d %%", (int8_t)gauge.FCClearThreshold());
    
    LOG_INFO("[BQ27441] TCA Set Threshold: %d %%", (int8_t)gauge.TCASetThreshold());
    LOG_INFO("[BQ27441] TCA Clear Threshold: %d %%", (int8_t)gauge.TCAClearThreshold());

    if (config_written) {

         gauge.setGPOUTFunction(BAT_LOW); 
    }

    LOG_INFO("[BQ27441] Test Complete");
}


