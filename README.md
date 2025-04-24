# AidleyController Firmware

🛠️ Firmware for the **Aidley Controller**, an STM32F767-based embedded system using modular C++ drivers and FreeRTOS.

---

## Target MCU

- STM32F767ZGT6 (ARM Cortex-M7)
- Peripherals used: UART, QSPI, I2C, SPI...
- RTOS: CMSIS-RTOS v2 (FreeRTOS backend)


---

## 🔧 Build Instructions (Linux & Windows)

See [GIT_CHEATSHEET.md](GIT_CHEATSHEET.md) for full setup, build, and GitHub CI steps across Windows & Linux.

This project uses CMake + Ninja and the **Arm GNU Toolchain**.

### 📂 Clone the Repo

```bash
git clone https://github.com/ElectronicsBuilder/AidleyController.git
cd AidleyController

🐧 Linux Setup

# Install build tools and ARM toolchain
sudo apt update
sudo apt install cmake ninja-build gcc-arm-none-eabi gdb-arm-none-eabi

# Configure & Build
cmake -B build -G Ninja
cmake --build build






🪟 Windows Setup (STM32CubeCLT)

    Install STM32CubeCLT

    Add CMake + Ninja to PATH from STM32CubeCLT

    Open CMD or PowerShell

cd path\to\AidleyController

# Configure & Build
cmake -B build -G Ninja
cmake --build build


✅ Output ELF: build/AidleyController.elf



## 🛠️ GitHub Actions

This project builds automatically using GitHub Actions CI on every push to `main`
