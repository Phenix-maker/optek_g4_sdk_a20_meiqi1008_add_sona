/**
 ****************************************************************************************
 *
 * @file hal_sflash.c
 *
 * @brief sflash hal asynchronous read and write data implement.
 *
 ****************************************************************************************
 */

#include "os_config.h"

#include "regmap.h"

#include "printf.h"
#include "interrupt.h"
#include "flash_boot.h"
#include "mem_reloc.h"
#include "hal_sflash.h"
#include "hw_timer.h"
#include "hw_sflash.h"

static U8 tempBuf[FLASH_SECTOR_SIZE];

/**
 ****************************************************************************************
 * @brief HAL layer Flash write function
 *
 * @param[in] flash_addr  Address in the Flash to start writing
 * @param[in] pBuf        Pointer to the data buffer to be written
 * @param[in] len         Length of the data to be written in bytes
 *
 * @return None
 ****************************************************************************************
 */
U32 hal_sflash_write(U32 flash_addr, U8 *pBuf, U32 len) __attribute__ ((section (".internal_ram_1_text")));
U32 hal_sflash_write(U32 flash_addr, U8 *pBuf, U32 len) 
{
    if (!(flash_addr >= FLASH_START_ADDR && flash_addr <= FLASH_END_ADDR))
    {
        DBG_Assert(FALSE);
        return FALSE;
    }

    // Calculate the start sector address
    U32 start_sector_addr = flash_addr & ~(FLASH_SECTOR_SIZE - 1);
    // Calculate the end sector address
    U32 end_sector_addr = (flash_addr + len - 1) & ~(FLASH_SECTOR_SIZE - 1);

    // Iterate through all sectors that need to be written
    for (U32 sector_addr = start_sector_addr; sector_addr <= end_sector_addr; sector_addr += FLASH_SECTOR_SIZE) 
    {
        // Read the current sector's data into the temporary buffer
        //memcpy(tempBuf, sector_addr, FLASH_SECTOR_SIZE);
        
        // Calculate the offset and length of the data to be written in the current sector
        U32 offset = (sector_addr == start_sector_addr) ? (flash_addr - sector_addr) : 0;
        U32 sector_len = FLASH_SECTOR_SIZE - offset;
        if (sector_addr == end_sector_addr) 
        {
            // The last sector may not need to be fully written
            sector_len = (flash_addr + len - sector_addr) - offset;
        }
        
        memcpy(tempBuf, sector_addr, offset);
        memcpy(tempBuf+offset, pBuf, sector_len);
        pBuf += sector_len;
        memcpy(tempBuf+offset+sector_len, sector_addr+offset+sector_len, FLASH_SECTOR_SIZE-(offset+sector_len));

        // Erase the corresponding Sector
        sflash_sector_erase(sector_addr);
        
        // Write the combined data back to the Sector
        sflash_sector_write(sector_addr, tempBuf, FLASH_SECTOR_SIZE);

    }

    return TRUE;
}

U32 hal_sflash_read(U32 flash_addr, U8 *pBuf, U32 len) 
{
    if (!(flash_addr >= FLASH_START_ADDR && flash_addr <= FLASH_END_ADDR) && pBuf != NULL)
    {
        DBG_Assert(FALSE);
        return FALSE;
    }

    memcpy(pBuf, (void *)flash_addr, len);

    return TRUE;
}