#include "qspi_flash.hpp"
#include "main.h"   
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_qspi.h"

extern QSPI_HandleTypeDef hqspi;


#define QSPI_TIMEOUT_DEFAULT HAL_QSPI_TIMEOUT_DEFAULT_VALUE

void qspi_flash_reset(void)
{
    QSPI_CommandTypeDef sCommand = {0};

    // RESET ENABLE
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = 0x66; // Reset Enable
    sCommand.AddressMode       = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_NONE;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &sCommand, QSPI_TIMEOUT_DEFAULT);

    // RESET MEMORY
    sCommand.Instruction = 0x99; // Reset Memory

    HAL_QSPI_Command(&hqspi, &sCommand, QSPI_TIMEOUT_DEFAULT);
}

void qspi_flash_read_id(uint8_t* id_buffer)
{
    QSPI_CommandTypeDef sCommand = {0};

    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = 0x9F; // Read ID
    sCommand.AddressMode       = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_1_LINE;
    sCommand.DummyCycles       = 0;
    sCommand.NbData            = 3; // 3 bytes: Manufacturer, Memory Type, Capacity
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &sCommand, QSPI_TIMEOUT_DEFAULT);
    HAL_QSPI_Receive(&hqspi, id_buffer, QSPI_TIMEOUT_DEFAULT);
}

void qspi_flash_read_data(uint32_t address, uint8_t* buffer, size_t length)
{
    QSPI_CommandTypeDef sCommand = {0};

    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = 0x03; // Read Data (1-1-1 mode)
    sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address           = address;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_1_LINE;
    sCommand.DummyCycles       = 8; // Typical for slow read, 8 dummy cycles
    sCommand.NbData            = length;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &sCommand, QSPI_TIMEOUT_DEFAULT);
    HAL_QSPI_Receive(&hqspi, buffer, QSPI_TIMEOUT_DEFAULT);
}

void qspi_flash_init(void)
{
    qspi_flash_reset();
    // Optionally: Verify flash ID here if you want
}
