#include "os_config.h"

#include "regmap.h"

#include "hw_gpio.h"
#include "hw_sflash.h"

#include "hal_dma.h"

void sflash_sector_erase(U32 addr) __attribute__ ((section (".internal_ram_1_text")));
void sflash_sector_write(U32 addr, U32 *pBuf, U32 len) __attribute__ ((section (".internal_ram_1_text")));
static U8 W25QXX_Page_Write(U32 addr, U32 *pBuf, U32 len) __attribute__ ((section (".internal_ram_1_text")));
static U32 Endian_32_Convert(U32 addr) __attribute__ ((section (".internal_ram_1_text")));

volatile int sflash_in_programming = 0;

static U32 Endian_32_Convert(U32 addr)
{
	U8 *pAddr;
	U8 *pSwapAddr;
	U32 tmp;

	pAddr = (U8 *)&addr;
	pSwapAddr = (U8 *)&tmp;

	*pSwapAddr++ =  *(pAddr + 3);
	*pSwapAddr++ =  *(pAddr + 2);
	*pSwapAddr++ =  *(pAddr + 1);
	*pSwapAddr =  *(pAddr + 0);

	return tmp;
}

void sflash_sector_erase(U32 addr)
{
	u8 rd_status1, rd_status2;
	SPI_FLASH_STATUS1s *pSatus1 = (SPI_FLASH_STATUS1s *) &rd_status1;
	SPI_FLASH_STATUS2s *pSatus2 = (SPI_FLASH_STATUS1s *) &rd_status2;

	while (1)
	{
		rd_status1 = ADDR_SPI_STATUS;
		if (pSatus1->busy == 0)
			break;
	}

	ADDR_SPI_WR_ENABLE = 0x0;
	sflash_in_programming = 1;
	ADDR_SPI_SECTOR_ERASE = Endian_32_Convert(addr);

	U32 cnt = 0;
	while (1)
	{
		rd_status1 = ADDR_SPI_STATUS;
		rd_status2 = ADDR_SPI_STATUS2;

		if 	(pSatus2->sus)
		{
			ADDR_SPI_RESUME = 0;
			sflash_in_programming = 1;
		}
		else if (pSatus1->busy == 0)
		{
			break;
		}
	}
	sflash_in_programming = 0;
}

static U8 W25QXX_Page_Write(U32 addr, U32 *pBuf, U32 len)
{
	int i;
	U32 dummy;
	u8 *p8_dest,*p8_src;
	U32 *p32_dest,*p32_src;
	u8 rd_status1, rd_status2;
	SPI_FLASH_STATUS1s *pSatus1 = (SPI_FLASH_STATUS1s *) &rd_status1;
	SPI_FLASH_STATUS2s *pSatus2 = (SPI_FLASH_STATUS1s *) &rd_status2;

	DBG_Assert(len <= FLASH_PAGE_SIZE);
	while (1)
	{
		rd_status1 = ADDR_SPI_STATUS;
		if (pSatus1->busy == 0)
			break;
	}

	ADDR_SPI_WR_ENABLE = 0x0;

	TX_INTERRUPT_SAVE_AREA;
	
	TX_DISABLE;	

	ADDR_SPI_FAST_PAGE_PROGRAM = Endian_32_Convert(addr);
	
	p32_dest = addr;
	p32_src = pBuf;

	for (i=0;i<len/4;i++) {
		p32_dest[i] = p32_src[i];
	}

	len -= len;

	if (len)
	{
		p8_dest = addr;
		p8_src = pBuf[i];		
		for (i=0;i<len;i++) 
		{
			p8_dest[i] = p8_src[i];
		}
	}

	sflash_in_programming = 1;
	dummy = ADDR_SPI_PROGRAM_START;					//start program		
	TX_RESTORE;

	while (1)
	{
		rd_status1 = ADDR_SPI_STATUS;
		rd_status2 = ADDR_SPI_STATUS2;

		if 	(pSatus2->sus)
		{
			ADDR_SPI_RESUME = 0;
			sflash_in_programming = 1;
		}
		else if (pSatus1->busy == 0)
		{
			break;
		}
	}	
	sflash_in_programming = 0;

	return TRUE;
}

void sflash_sector_write(U32 addr, U32 *pBuf, U32 len)
{
	while (len)
	{
		if (len > FLASH_PAGE_SIZE)
		{
			len -= FLASH_PAGE_SIZE;
			W25QXX_Page_Write(addr,pBuf,FLASH_PAGE_SIZE);
			pBuf += FLASH_PAGE_SIZE/4;
			addr += FLASH_PAGE_SIZE;
		}
		else
		{
			W25QXX_Page_Write(addr,pBuf,len);
			len = 0;
			break;
		}
	}
}
