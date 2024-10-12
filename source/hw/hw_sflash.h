#ifndef _HW_SFLASH_H_
#define _HW_SFLASH_H_

//256 Bytes
#define FLASH_PAGE_SIZE            64//128//256

//4 kBytes
#define FLASH_SECTOR_SIZE          (1024*4)  //min erasing sector is 4k. 

//64 kBytes
#define FLASH_BLOCK_SIZE           (1024*64)

extern volatile int sflash_in_programming;
void sflash_sector_erase(U32 addr);
void sflash_sector_write(U32 addr, U32 *pBuf, U32 len);

#endif //_HW_SFLASH_H_

