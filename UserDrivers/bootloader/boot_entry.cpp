#include "boot_fuse.hpp"
#include "stm32f7xx_hal.h"
#include "main.h"
#include "stm32f7xx_hal.h"

typedef void (*pFunction)(void);


pFunction Jump_To_Application;

#define BOOTLOADER_ADDRESS 0x08000000U

void check_and_jump_to_bootloader() {
    


    uint8_t fuse_status = READ_BOOT_FUSE();

    if (fuse_status == BOOT_FUSE_SET) {

        typedef void (*pFunction)(void);
        uint32_t jumpAddress = *(__IO uint32_t*)(BOOTLOADER_ADDRESS + 4);
        pFunction jumpToBootloader = (pFunction)jumpAddress;

        HAL_RCC_DeInit();
        HAL_DeInit();

        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL = 0;

        __disable_irq();
        SCB->VTOR = BOOTLOADER_ADDRESS;
        __set_MSP(*(__IO uint32_t*)BOOTLOADER_ADDRESS);
        jumpToBootloader();
    }
}