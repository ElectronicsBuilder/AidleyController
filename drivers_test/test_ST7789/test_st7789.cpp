#include "test_st7789.hpp"
#include "st7789.hpp"
#include "st7789_defs.h"
#include "main.h"
#include "log.hpp"

extern SPI_HandleTypeDef hspi6;

void test_st7789_sequence()
{
    LOG_INFO("[ST7789] Starting test sequence");

    ST7789::Config cfg = {
        .hspi = &hspi6,
        .cs_port = TFT_CS_GPIO_Port,
        .cs_pin = TFT_CS_Pin,
        .dc_port = TFT_DC_GPIO_Port,
        .dc_pin = TFT_DC_Pin,
        .reset_port = TFT_RESET_GPIO_Port,
        .reset_pin = TFT_RESET_Pin,
        .bkl_port = BKL_PWM_GPIO_Port,
        .bkl_pin = BKL_PWM_Pin,
        .scan_dir = ST7789_SCAN_DIR_VERTICAL
    };

    ST7789 display(cfg);
    display.init();

    LOG_INFO("[ST7789] Fill RED");
    display.fillScreen(RED);
    HAL_Delay(500);

    LOG_INFO("[ST7789] Fill GREEN");
    display.fillScreen(GREEN);
    HAL_Delay(500);

    LOG_INFO("[ST7789] Fill BLUE");
    display.fillScreen(BLUE);
    HAL_Delay(500);

    LOG_INFO("[ST7789] Test complete");
}