#ifndef _HAL_SFLASH_H_
#define _HAL_SFLASH_H_
#include "hw_sflash.h"

__inline sflash_in_programming_check(void)
{
	if (sflash_in_programming)
	{
		ADDR_SPI_SUSPEND = 0;

		u8 rd_status1;
		SPI_FLASH_STATUS1s *pSatus1 = (SPI_FLASH_STATUS1s *) &rd_status1;

		do{
			rd_status1 = ADDR_SPI_STATUS;			
		} while(pSatus1->busy);

		sflash_in_programming = 0;
	}
}


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
U32 hal_sflash_write(U32 flash_addr, U8 *pBuf, U32 len);


/**
 ****************************************************************************************
 * @brief HAL layer Flash read function
 *
 * @param[in] flash_addr  Address in the Flash to start reading
 * @param[in] pBuf        Pointer to the data buffer to be read
 * @param[in] len         Length of the data to be read in bytes
 *
 * @return None
 ****************************************************************************************
 */
U32 hal_sflash_read(U32 flash_addr, U8 *pBuf, U32 len);

#endif //_APP_UART_H_
