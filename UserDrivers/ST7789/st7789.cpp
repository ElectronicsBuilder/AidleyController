#include "st7789.hpp"
#include "st7789_defs.h"
#include "main.h"
#include <cstring>

ST7789::ST7789(const Config& cfg) : cfg_(cfg), width_(0), height_(0) {}

void ST7789::init() {
    setBacklight(false);
    HAL_Delay(100);
    setBacklight(true);
    reset();
    HAL_Delay(100);
    setDir(cfg_.scan_dir);
    writeInitSequence();
}

void ST7789::reset() {
    HAL_GPIO_WritePin(cfg_.reset_port, cfg_.reset_pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(cfg_.reset_port, cfg_.reset_pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(cfg_.reset_port, cfg_.reset_pin, GPIO_PIN_SET);
    HAL_Delay(150);
}

void ST7789::setBacklight(bool on) {
    HAL_GPIO_WritePin(cfg_.bkl_port, cfg_.bkl_pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void ST7789::sendCommand(uint8_t c) {
    HAL_GPIO_WritePin(cfg_.dc_port, cfg_.dc_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(cfg_.cs_port, cfg_.cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(cfg_.hspi, &c, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(cfg_.cs_port, cfg_.cs_pin, GPIO_PIN_SET);
}

void ST7789::sendData(uint8_t c) {
    HAL_GPIO_WritePin(cfg_.dc_port, cfg_.dc_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(cfg_.cs_port, cfg_.cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(cfg_.hspi, &c, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(cfg_.cs_port, cfg_.cs_pin, GPIO_PIN_SET);
}

void ST7789::sendBuffer(const uint8_t *data, size_t size) {
    HAL_GPIO_WritePin(cfg_.cs_port, cfg_.cs_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(cfg_.dc_port, cfg_.dc_pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(cfg_.hspi, (uint8_t*)data, size, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(cfg_.cs_port, cfg_.cs_pin, GPIO_PIN_SET);
}

void ST7789::sendColorData(const uint16_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        uint8_t bytes[2] = { (uint8_t)(data[i] >> 8), (uint8_t)(data[i] & 0xFF) };
        sendBuffer(bytes, 2);
    }
}

void ST7789::setDir(st7789_scan_dir_t dir) {
    uint8_t memAccess = 0x00;
    if (dir == ST7789_SCAN_DIR_HORIZONTAL) {
        width_ = LCD_HEIGHT;
        height_ = LCD_WIDTH;
        memAccess = 0x70;  // Optional: adjust based on specific rotation config
    } else {
        width_ = LCD_WIDTH;
        height_ = LCD_HEIGHT;
        memAccess = 0x00;
    }
    sendCommand(ST7789_MADCTL);
    sendData(memAccess);
}

void ST7789::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    const uint16_t X_OFFSET = 35;
    x0 += X_OFFSET;
    x1 += X_OFFSET;
    sendCommand(ST7789_CASET);
    sendData(x0 >> 8); sendData(x0 & 0xFF);
    sendData(x1 >> 8); sendData(x1 & 0xFF);

    sendCommand(ST7789_PASET);
    sendData(y0 >> 8); sendData(y0 & 0xFF);
    sendData(y1 >> 8); sendData(y1 & 0xFF);

    sendCommand(ST7789_RAMWR);
}

void ST7789::drawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data) {
    if ((x >= width_) || (y >= height_)) return;
    if ((x + w - 1) >= width_) w = width_ - x;
    if ((y + h - 1) >= height_) h = height_ - y;
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    sendColorData(data, w * h);
}

void ST7789::fillScreen(uint16_t color) {
    fillRect(0, 0, width_, height_, color);
}

void ST7789::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if ((x >= width_) || (y >= height_)) return;
    if ((x + w - 1) >= width_) w = width_ - x;
    if ((y + h - 1) >= height_) h = height_ - y;
    setAddrWindow(x, y, x + w - 1, y + h - 1);

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    HAL_GPIO_WritePin(cfg_.cs_port, cfg_.cs_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(cfg_.dc_port, cfg_.dc_pin, GPIO_PIN_SET);
    for (int i = 0; i < w * h; i++) {
        HAL_SPI_Transmit(cfg_.hspi, &hi, 1, HAL_MAX_DELAY);
        HAL_SPI_Transmit(cfg_.hspi, &lo, 1, HAL_MAX_DELAY);
    }
    HAL_GPIO_WritePin(cfg_.cs_port, cfg_.cs_pin, GPIO_PIN_SET);
}

void ST7789::setRotation(uint8_t m) {
    sendCommand(ST7789_MADCTL);
    switch (m) {
        case 0: sendData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB); break;
        case 1: sendData(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB); break;
        case 2: sendData(ST7789_MADCTL_RGB); break;
        case 3: sendData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB); break;
    }
}

void ST7789::writeInitSequence() {
    sendCommand(ST7789_COLMOD); sendData(ST7789_COLOR_MODE_16bit);

    sendCommand(ST7789_PORCH_CTRL_CMD);
    {
        uint8_t buf[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
        sendBuffer(buf, sizeof(buf));
    }

    sendCommand(ST7789_GATE_CTRL_CMD);   sendData(ST7789_GATE_CTRL_DATA);
    sendCommand(ST7789_VCOM_CMD);        sendData(ST7789_VCOM_DATA);
    sendCommand(ST7789_PWR_CTRL1_CMD);   sendData(ST7789_PWR1_DATA);
    sendCommand(ST7789_PWR_CTRL2_CMD);   sendData(ST7789_PWR2_DATA);
    sendCommand(ST7789_VRH_VDV_CTRL_CMD);sendData(ST7789_VRH_VDV_DATA);
    sendCommand(ST7789_VDV_SETTING_CMD); sendData(ST7789_VDV_SETTING_DATA);
    sendCommand(ST7789_FR_CTRL_CMD);     sendData(ST7789_FR_CTRL_DATA);
    sendCommand(ST7789_PWR_CTRL3_CMD);   sendData(ST7789_PWR3_DATA1); sendData(ST7789_PWR3_DATA2);

    sendCommand(ST7789_GAMMA_POS_CMD);
    {
        uint8_t pos[] = {0xF0, 0x00, 0x04, 0x04, 0x04, 0x05, 0x29, 0x33, 0x3E, 0x38, 0x12, 0x12, 0x28, 0x30};
        sendBuffer(pos, sizeof(pos));
    }

    sendCommand(ST7789_GAMMA_NEG_CMD);
    {
        uint8_t neg[] = {0xF0, 0x07, 0x0A, 0x0D, 0x0B, 0x07, 0x28, 0x33, 0x3E, 0x36, 0x14, 0x14, 0x29, 0x32};
        sendBuffer(neg, sizeof(neg));
    }

    sendCommand(ST7789_INVON);
    setRotation(ST7789_ROTATION);
    sendCommand(ST7789_SLPOUT);
    HAL_Delay(120);
    sendCommand(ST7789_DISPON);
    HAL_Delay(50);
}
