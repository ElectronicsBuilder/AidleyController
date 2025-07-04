cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM, C and C++ languages
enable_language(C CXX ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_HAL_DRIVER 
	STM32F767xx 
	STM32_THREAD_SAFE_STRATEGY=4
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    ../../TouchGFX/App
    ../../TouchGFX/target/generated
    ../../TouchGFX/target
    ../../Drivers/STM32F7xx_HAL_Driver/Inc
    ../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
    ../../Middlewares/Third_Party/FreeRTOS/Source/include
    ../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
    ../../Drivers/CMSIS/Device/ST/STM32F7xx/Include
    ../../Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c
    ../../Core/Src/freertos.c
    ../../Core/Src/stm32f7xx_it.c
    ../../Core/Src/stm32f7xx_hal_msp.c
    ../../Core/Src/stm32f7xx_hal_timebase_tim.c
    ../../TouchGFX/target/TouchGFXHAL.cpp
    ../../TouchGFX/target/STM32TouchController.cpp
    ../../TouchGFX/target/TouchGFXGPIO.cpp
    ../../TouchGFX/target/generated/TouchGFXConfiguration.cpp
    ../../TouchGFX/target/generated/TouchGFXGeneratedHAL.cpp
    ../../TouchGFX/target/generated/STM32DMA.cpp
    ../../TouchGFX/target/generated/OSWrappers.cpp
    ../../TouchGFX/App/app_touchgfx.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_exti.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc_ex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2s.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rng.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi_ex.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c
    ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart_ex.c
    ../../Core/Src/system_stm32f7xx.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/list.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../startup_stm32f767xx.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()

# Validate that STM32CubeMX code is compatible with C++ standard
if(CMAKE_CXX_STANDARD LESS 11)
    message(ERROR "Generated code requires C++11 or higher")
endif()
