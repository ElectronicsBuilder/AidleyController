#ifndef ST7789_HPP
#define ST7789_HPP
#include "stm32f7xx_hal.h"
// #include <cstdint>
// #include <cstddef>

enum st7789_scan_dir_t {
    ST7789_SCAN_DIR_HORIZONTAL,
    ST7789_SCAN_DIR_VERTICAL
};

class ST7789 {
public:
    struct Config {
        SPI_HandleTypeDef *hspi;
        GPIO_TypeDef *cs_port;
        uint16_t cs_pin;
        GPIO_TypeDef *dc_port;
        uint16_t dc_pin;
        GPIO_TypeDef *reset_port;
        uint16_t reset_pin;
        GPIO_TypeDef *bkl_port;
        uint16_t bkl_pin;
        st7789_scan_dir_t scan_dir;
    };

    explicit ST7789(const Config& cfg);

    void init();
    void drawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
    void fillScreen(uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void setRotation(uint8_t m);
    void setBacklight(bool on);

private:
    Config cfg_;
    uint16_t width_;
    uint16_t height_;

    void reset();
    void setDir(st7789_scan_dir_t dir);
    void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void sendCommand(uint8_t c);
    void sendData(uint8_t c);
    void sendBuffer(const uint8_t *data, size_t size);
    void sendColorData(const uint16_t *data, uint32_t length);
    void writeInitSequence();
};
#endif // ST7789_HPP
