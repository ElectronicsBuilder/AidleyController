#include "uart.hpp"
#include "log.hpp"
#include "cmsis_os.h" 
#include "task.h"
#include "qspi_flash.hpp"
#include "test_qspi_flash.hpp"

void test_qspi_flash_id()
{
    qspi_flash_init(); 

    uint8_t id[3];
    qspi_flash_read_id(id);


    LOG_INFO("QSPI Flash ID: 0x%02X 0x%02X 0x%02X", id[0], id[1], id[2]);

}
   