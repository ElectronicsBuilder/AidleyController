cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME AidleyController)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)




# ---------------------------
# ADD THIS: Custom sources
# ---------------------------



file(GLOB_RECURSE PROJECT_SOURCES
    "${CMAKE_SOURCE_DIR}/Core/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Core/Src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/drivers_test/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/drivers_test/*.cpp"
    "${CMAKE_SOURCE_DIR}/UserDrivers/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/UserDrivers/**/*.c"
    "${CMAKE_SOURCE_DIR}/logs/*.cpp"
    "${CMAKE_SOURCE_DIR}/app_defs/*.cpp"
    "${CMAKE_SOURCE_DIR}/app_defs/*.c"

    "${CMAKE_SOURCE_DIR}/TouchGFX/generated/fonts/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/TouchGFX/generated/gui_generated/src/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/TouchGFX/generated/images/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/TouchGFX/generated/texts/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/TouchGFX/gui/src/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/TouchGFX/target/*.cpp"
    "${CMAKE_SOURCE_DIR}/TouchGFX/target/*.c"

)

message(STATUS "PROJECT_SOURCES = ${PROJECT_SOURCES}")

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCES}
)



# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/drivers
    ${CMAKE_SOURCE_DIR}/drivers_test
    ${CMAKE_SOURCE_DIR}/logs
    ${CMAKE_SOURCE_DIR}/app_defs 
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include

    ${CMAKE_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
    

    ${CMAKE_SOURCE_DIR}/UserDrivers/uart
    ${CMAKE_SOURCE_DIR}/UserDrivers/spi_flash
    ${CMAKE_SOURCE_DIR}/UserDrivers/nvram
    ${CMAKE_SOURCE_DIR}/UserDrivers/qspi_flash
    ${CMAKE_SOURCE_DIR}/UserDrivers/SHT4x
    ${CMAKE_SOURCE_DIR}/UserDrivers/BMP581
    ${CMAKE_SOURCE_DIR}/UserDrivers/BNO085
    ${CMAKE_SOURCE_DIR}/UserDrivers/BQ27441
    ${CMAKE_SOURCE_DIR}/UserDrivers/TPS2115
    ${CMAKE_SOURCE_DIR}/UserDrivers/TPS2121
    ${CMAKE_SOURCE_DIR}/UserDrivers/TXS0108
    ${CMAKE_SOURCE_DIR}/UserDrivers/radio_receiver
    ${CMAKE_SOURCE_DIR}/UserDrivers/boot_fuse 
    ${CMAKE_SOURCE_DIR}/UserDrivers/ST7789 

    ${CMAKE_SOURCE_DIR}/drivers_test/test_uart
    ${CMAKE_SOURCE_DIR}/drivers_test/test_spi_flash
    ${CMAKE_SOURCE_DIR}/drivers_test/test_nvram
    ${CMAKE_SOURCE_DIR}/drivers_test/test_qspi_flash
    ${CMAKE_SOURCE_DIR}/drivers_test/test_SHT4x
    ${CMAKE_SOURCE_DIR}/drivers_test/test_BMP581
    ${CMAKE_SOURCE_DIR}/drivers_test/test_I2C
    ${CMAKE_SOURCE_DIR}/drivers_test/test_BNO085
    ${CMAKE_SOURCE_DIR}/drivers_test/test_BQ27441
    ${CMAKE_SOURCE_DIR}/drivers_test/test_power_mux
    ${CMAKE_SOURCE_DIR}/drivers_test/test_radio_receiverHW
    ${CMAKE_SOURCE_DIR}/drivers_test/test_ST7789


    ${CMAKE_SOURCE_DIR}/TouchGFX/gui/include
    ${CMAKE_SOURCE_DIR}/TouchGFX/App
    ${CMAKE_SOURCE_DIR}/TouchGFX/target
    ${CMAKE_SOURCE_DIR}/TouchGFX/target/generated


    ${CMAKE_SOURCE_DIR}/Middlewares/ST/touchgfx/framework/include
    ${CMAKE_SOURCE_DIR}/TouchGFX/generated/fonts/include
    ${CMAKE_SOURCE_DIR}/TouchGFX/generated/gui_generated/include
    ${CMAKE_SOURCE_DIR}/TouchGFX/generated/images/include
    ${CMAKE_SOURCE_DIR}/TouchGFX/generated/texts/include
    ${CMAKE_SOURCE_DIR}/TouchGFX/generated/videos/include

    ${CMAKE_SOURCE_DIR}/TouchGFX/gui/include
    ${CMAKE_SOURCE_DIR}/TouchGFX/gui/include/gui/common 
    ${CMAKE_SOURCE_DIR}/TouchGFX/gui/include/gui/model
    ${CMAKE_SOURCE_DIR}/TouchGFX/gui/include/gui/splashscreen_screen
    ${CMAKE_SOURCE_DIR}/TouchGFX/gui/include/gui/presplashscreen_screen
    
)

target_include_directories(AidleyController
    PRIVATE
    UserDrivers/components/w25q128
)

   # ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/DSP/Include  # <- Add this for arm_math.h
   # ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/NN/Include  # <- Add this for arm_math.h

   




# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
      ${CMAKE_SOURCE_DIR}/Middlewares/ST/touchgfx/lib/core/cortex_m7/gcc/libtouchgfx-float-abi-hard.a
)

target_link_options(AidleyController PRIVATE -u _printf_float)

# === Post-build ELF to BIN conversion ===
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD

    # Full image (internal + external sections) — mostly for ST-Link flashing or debug
    COMMAND ${CMAKE_OBJCOPY} -O binary
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
            ${CMAKE_CURRENT_BINARY_DIR}/AidleyController.bin

    # External assets only (QSPI target)
    COMMAND ${CMAKE_OBJCOPY} -O binary
            --only-section=FontFlashSection
            --only-section=TextFlashSection
            --only-section=ExtFlashSection
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
            ${CMAKE_CURRENT_BINARY_DIR}/AidleyControllerExternalFlash.bin

    # Internal app only (no assets — for bootloader or production)
    COMMAND ${CMAKE_OBJCOPY} -O binary
            --remove-section=ExtFlashSection
            --remove-section=FontFlashSection
            --remove-section=TextFlashSection
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
            ${CMAKE_CURRENT_BINARY_DIR}/AidleyControllerInternalFlash.bin

    COMMENT "Generating InternalFlash.bin and ExternalFlash.bin from ELF"
)