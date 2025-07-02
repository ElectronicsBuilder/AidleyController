#include "display_driver.hpp"
#include "main.h"

extern SPI_HandleTypeDef hspi6;

uint16_t DISPLAY_DONE_UPDATE = 0;

ST7789::Config display_config = {
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

ST7789 display(display_config);
