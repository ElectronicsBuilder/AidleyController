#include <stdio.h>
#include <string.h>

#include "uart.hpp"
#include "log.hpp"
#include "cmsis_os.h"
#include "task.h"
#include "qspi_flash.hpp"
#include "test_qspi_flash.hpp"
#include "app_defs.hpp"


extern QSPI_HandleTypeDef hqspi;
extern bool qspi_dma_tx_done;


//#define QSPI_MEM_BASE_ADDR  ((uint8_t*)QSPI_USERDATA_WRITE_ADDR)



void qspi_flash_self_test()
{
     QspiFlash flash(&hqspi);
     
    uint8_t id[3] = {0};

    //QspiFlash flash(&hqspi);

    /* Read and display JEDEC ID */
    flash.readID(id);
    LOG_INFO("[QSPI] Quad Read JEDEC ID: 0x%02X 0x%02X 0x%02X", id[0], id[1], id[2]);

    /* Get and display device info */
    QFlashDeviceInfo info = flash.getDeviceInfo();
    LOG_INFO("[QSPI] QSPI Flash: %s, %lu Mbit", info.part_number, info.capacity_mbit);
    LOG_INFO("[QSPI] Page Size: %lu bytes, Sector Size: %lu bytes, Quad: %s",
             info.page_size,
             info.sector_size,
             info.supports_quad ? "Yes" : "No");

    constexpr uint32_t test_addr = QSPI_USERDATA_WRITE_ADDR;
    const char initial_data[] = "QSPI Quad Test OK";
    const char dma_data[] = "QSPI DMA Write Test OK";
    const char memory_mapped_data[] = "QSPI Quad Test Memory Mapped OK";
    uint8_t read_buffer[64] = {0};

    /* Erase sector before writing */
    flash.eraseSector(test_addr);

    /* Write initial test data using Quad Write */
    flash.writeDataQuad(test_addr, reinterpret_cast<const uint8_t*>(initial_data), sizeof(initial_data));
    LOG_INFO("[QSPI] Written data (Quad Write 1): %s", initial_data);

    /* Read back using Quad Read command */
    flash.readDataQuad(test_addr, read_buffer, sizeof(initial_data));
    LOG_INFO("[TEST] Read back data (Quad Command Read 1): %s", read_buffer);

    /* Verify Quad Command Read */
    if (memcmp(read_buffer, initial_data, sizeof(initial_data)) != 0) {
        LOG_ERROR("[ERROR] Quad Command Readback Failed (First Write)!");
    }

    /* Erase sector again for DMA Write */
    flash.eraseSector(test_addr);

    /* Write using DMA */
    flash.writeDataQuadDMA(test_addr, reinterpret_cast<const uint8_t*>(dma_data), sizeof(dma_data));
    LOG_INFO("[QSPI] Started DMA Write: %s", dma_data);

    /* Wait for DMA completion  */

    if (!flash.waitDmaComplete(1000)) {
        LOG_ERROR("[ERROR] DMA Wait Timeout!");
        return;
    }

    /* After DMA complete, make sure flash is ready */
    flash.autoPollingMemReady();

    /* Read back after DMA Write */

    flash.readDataQuadDMA(test_addr, read_buffer, sizeof(dma_data));

    if (!flash.waitDmaComplete(1000)) {
        LOG_ERROR("[ERROR] DMA Read Timeout!");
        return;
    }

    LOG_INFO("[QSPI] Read back data (After DMA Read): %s", read_buffer);



    /* Verify DMA Write result */
    if (memcmp(read_buffer, dma_data, sizeof(dma_data)) != 0) {
        LOG_ERROR("[ERROR] DMA Write Readback Failed!");
    }

    /* Erase sector again for Memory Mapped test */
    flash.eraseSector(test_addr);

    /* Write using normal Quad Write again */
    flash.writeDataQuad(test_addr, reinterpret_cast<const uint8_t*>(memory_mapped_data), sizeof(memory_mapped_data));
    LOG_INFO("[QSPI] Memory-Mapped Written data : %s", memory_mapped_data);

    /* Enable Memory Mapped Quad Read Mode */
    flash.enableQuadMemoryMappedMode();

    /* Direct memory access (assumes QSPI_USERDATA_WRITE_ADDR) */
    const uint8_t* mem_mapped_ptr = reinterpret_cast<const uint8_t*>(QSPI_USERDATA_ADDR);
    LOG_INFO("[QSPI] Memory-Mapped Read: %s", mem_mapped_ptr);

    /* Verify Memory Mapped Read */
    if (memcmp(mem_mapped_ptr, memory_mapped_data, sizeof(memory_mapped_data)) != 0) {
        LOG_ERROR("[ERROR] Memory-Mapped Readback Failed!");
    }

    LOG_INFO("[QSPI] QSPI Flash Quad Write, DMA Write, Read and MemoryMapped Passed");

    flash.disableMemoryMappedMode();
}
