#include "boot_fuse_qspiFlash.hpp"
#include "app_defs.hpp"
#include "boot_fuse.hpp"
#include "qspi_flash.hpp"
#include <cstring>


extern QspiFlash flash;

static const uint8_t boot_fuse_expected[BOOT_FUSE_SIZE] = {
    BOOT_FUSE_SET_BYTE1,
    BOOT_FUSE_SET_BYTE2,
    BOOT_FUSE_SET_BYTE3
};

static const uint8_t boot_fuse_cleared[BOOT_FUSE_SIZE] = {
    BOOT_FUSE_CLEAR_BYTE1,
    BOOT_FUSE_CLEAR_BYTE2,
    BOOT_FUSE_CLEAR_BYTE3
};

static void qspi_init()
{
    flash.init(); 
}


 bool qspiFlash_set_fuse()
{
    flash.disableMemoryMappedMode();

    boot_fuse_metadata_t meta = {
        .magic = BOOT_FUSE_MAGIC,
        .version = BOOT_FUSE_VERSION,
        .len = BOOT_FUSE_DATA_LEN,
    };
    memcpy(meta.fuse_data, boot_fuse_expected, BOOT_FUSE_SIZE);
    meta.crc8 = boot_crc8(reinterpret_cast<uint8_t*>(&meta), offsetof(boot_fuse_metadata_t, crc8));

            flash.eraseSector(QSPI_FUSE_ADDR);
            flash.writeDataQuad(QSPI_FUSE_ADDR, reinterpret_cast<uint8_t*>(&meta), sizeof(meta));

            return true;

}

