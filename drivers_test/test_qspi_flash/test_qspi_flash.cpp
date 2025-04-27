#include <stdio.h>
#include <string.h>

#include "uart.hpp"
#include "log.hpp"
#include "cmsis_os.h"
#include "task.h"
#include "qspi_flash.hpp"
#include "test_qspi_flash.hpp"

extern QSPI_HandleTypeDef hqspi;


#define QSPI_MEM_BASE_ADDR  ((uint8_t*)0x90000000)



void qspi_flash_self_test()
{
    uint8_t id[3] = {0};
    

    QspiFlash flash(&hqspi);

    flash.readID(id);

    LOG_INFO("[TEST] Quad Read JEDEC ID: 0x%02X 0x%02X 0x%02X", id[0], id[1], id[2]);

    QFlashDeviceInfo info = flash.getDeviceInfo();
    LOG_INFO("QSPI Flash: %s, %lu Mbit", info.part_number, info.capacity_mbit);
    LOG_INFO("Page Size: %lu bytes, Sector Size: %lu bytes, Quad: %s",
             info.page_size,
             info.sector_size,
             info.supports_quad ? "Yes" : "No");


    constexpr uint32_t test_addr = 0x0000;
    const char initial_data[] = "QSPI Quad Test OK";
    const char memory_mapped_data[] = "QSPI Quad Test Memory Mapped OK";
    uint8_t read_buffer[64] = {0};

    /* Erase sector before writing */
    flash.eraseSector(test_addr);

    /* Write initial test data using Quad Write */
    flash.writeDataQuad(test_addr, reinterpret_cast<const uint8_t*>(initial_data), sizeof(initial_data));
    LOG_INFO("[TEST] Written data (Quad Write 1): %s", initial_data);

    /* Read back using Quad Read command */
    flash.readDataQuad(test_addr, read_buffer, sizeof(initial_data));
    LOG_INFO("[TEST] Read back data (Quad Command Read 1): %s", read_buffer);

    /* Verify Quad Command Read */
    if (memcmp(read_buffer, initial_data, sizeof(initial_data)) != 0) {
        LOG_ERROR("[ERROR] Quad Command Readback Failed (First Write)!");
    }

    /* Overwrite with new data */
    flash.eraseSector(test_addr); // Re-erase before overwrite
    flash.writeDataQuad(test_addr, reinterpret_cast<const uint8_t*>(memory_mapped_data), sizeof(memory_mapped_data));
    LOG_INFO("[TEST] Memory-Mapped Written data : %s", memory_mapped_data);

    /* Enable Memory Mapped Quad Read Mode */
    flash.enableQuadMemoryMappedMode();

    /* Direct memory access (assumes 0x90000000 mapped) */
    const uint8_t* mem_mapped_ptr = reinterpret_cast<const uint8_t*>(0x90000000 + test_addr);
    LOG_INFO("[TEST] Memory-Mapped Read: %s", mem_mapped_ptr);

    /* Verify Memory Mapped Read */
    if (memcmp(mem_mapped_ptr, memory_mapped_data, sizeof(memory_mapped_data)) != 0) {
        LOG_ERROR("[ERROR] Memory-Mapped Readback Failed!");
    }

    LOG_INFO("[PASS] QSPI Flash Quad Write, Read and MemoryMapped Passed");
}
