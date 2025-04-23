# AidleyController Firmware

🛠️ Firmware for the **Aidley Controller**, an STM32F767-based embedded system using modular C++ drivers and FreeRTOS.

---

## Target MCU

- STM32F767ZGT6 (ARM Cortex-M7)
- Peripherals used: UART, QSPI, I2C, SPI...
- RTOS: CMSIS-RTOS v2 (FreeRTOS backend)

---

## 🔧 Build Instructions (Linux & Windows)

This project uses CMake + Ninja and the **Arm GNU Toolchain**.

### 📂 Clone the Repo

```bash
git clone https://github.com/ElectronicsBuilder/AidleyController.git
cd AidleyController
