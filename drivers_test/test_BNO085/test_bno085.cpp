#include "test_bno085.hpp"

extern "C" {
#include "sh2.h"
#include "sh2_SensorValue.h"
#include "sh2_err.h"
}

#include "stm32f7xx_hal.h"
#include <cstdint>
#include "log.hpp"
#include "main.h"
#include "BNO085.hpp"

extern SPI_HandleTypeDef hspi3;

// Helper to log raw packets
void logRawPacket(const uint8_t* buf, uint16_t packet_size) {
    uint16_t length = (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
    uint8_t channel = buf[2];
    uint8_t seq = buf[3];

    log_info("Raw packet → Len: %u  Channel: %u  Seq: %u", length, channel, seq);

    for (uint16_t i = 0; i < packet_size; ++i) {
        if (i % 16 == 0) {
            log_info("\n%04X: ", i);
        }
        log_info("%02X ", buf[i]);
    }
    log_info("\n");
}

void test_bno085() {
    BNO085 imu;

    log_info("[BNO085] Starting BNO085 SPI test!");

    if (!imu.begin_SPI(&hspi3,
                       DOF_CS_GPIO_Port, DOF_CS_Pin,
                       DOF_INT_GPIO_Port, DOF_INT_Pin,
                       DOF_NRST_GPIO_Port, DOF_NRST_Pin)) {
        log_error("[BNO085] Failed to find BNO085 over SPI");
        return;
    }

    log_info("[BNO085] BNO085 Found over SPI!");

    for (int n = 0; n < imu.prodIds.numEntries; n++) {
        log_info("[BNO085] Part %d: Version %d.%d.%d Build %d",
                 imu.prodIds.entry[n].swPartNumber,
                 imu.prodIds.entry[n].swVersionMajor,
                 imu.prodIds.entry[n].swVersionMinor,
                 imu.prodIds.entry[n].swVersionPatch,
                 imu.prodIds.entry[n].swBuildNumber);
    }

    if (!imu.enableReport(SH2_GAME_ROTATION_VECTOR, 10000)) {
        log_error("[BNO085] Could not enable game vector");
    }

    log_info("[BNO085] Reading events...");

    int noEventCounter = 0;
    int reportNumber = 0;
    do {
        osDelay(10);

        if (imu.wasReset()) {
            log_info("[BNO085] Sensor was reset, reconfiguring reports...");
            imu.enableReport(SH2_ACCELEROMETER, 10000);
        }


            sh2_SensorValue_t value;

            if (imu.getSensorEvent(&value)) {
                noEventCounter = 0;
                ++reportNumber;
                log_info("[BNO085] Received sensorId: %d (0x%02X)", value.sensorId, value.sensorId);

                switch (value.sensorId) {
                    case SH2_ACCELEROMETER:
                        log_info("[BNO085] Accelerometer - x: %.2f y: %.2f z: %.2f",
                                 value.un.accelerometer.x,
                                 value.un.accelerometer.y,
                                 value.un.accelerometer.z);
                        break;
                    case SH2_GYROSCOPE_CALIBRATED:
                        log_info("[BNO085] Gyroscope - x: %.2f y: %.2f z: %.2f",
                                 value.un.gyroscope.x,
                                 value.un.gyroscope.y,
                                 value.un.gyroscope.z);
                        break;
                    case SH2_GAME_ROTATION_VECTOR:
                        log_info("[BNO085] Game Rotation Vector - r: %.2f i: %.2f j: %.2f k: %.2f",
                                 value.un.gameRotationVector.real,
                                 value.un.gameRotationVector.i,
                                 value.un.gameRotationVector.j,
                                 value.un.gameRotationVector.k);
                        break;
                    case SH2_RAW_GYROSCOPE:
                        log_info("[BNO085] Gyro Integrated RV - r: %.2f i: %.2f j: %.2f k: %.2f",
                                 value.un.gyroIntegratedRV.real,
                                 value.un.gyroIntegratedRV.i,
                                 value.un.gyroIntegratedRV.j,
                                 value.un.gyroIntegratedRV.k);
                        break;
                    default:
                        log_warn("[BNO085] Unhandled sensorId: %d", value.sensorId);
                        break;
                }
            } else {
                noEventCounter++;
            }


        if (noEventCounter > 100) {  // ~1 second 
            log_warn("[BNO085] Warning: No events detected for 1 second. INT may be stuck high.");
            noEventCounter = 0;
        }
    }while(reportNumber < 20);
}
