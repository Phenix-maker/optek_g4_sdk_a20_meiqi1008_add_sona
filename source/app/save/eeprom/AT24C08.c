/*
 * AT24C08.c
 *
 *  Created on: 2024/2/19
 *      Author: Windows
 */
#include "os_config.h"

#include "debug.h"
#include "hw_timer.h"

#ifdef EEPROM_AT24C08
#include "app_i2c.h"
#include "app_save.h"

#include "AT24C08.h"

#define EEPROM_AT24C08_FUNCTION_ENABLE
#define DBG_Printf


#ifdef EMBEDED_HW_I2C
#define AT24c_read_sub    hw_i2c0_read_sub
#define AT24c_write_sub   hw_i2c0_write_sub
#elif (defined I2C0_ENABLE)
#define AT24c_read_sub    I2C0_read_sub
#define AT24c_write_sub   I2C0_write_sub
#elif (defined I2C1_ENABLE)
#define AT24c_read_sub    I2C1_read_sub
#define AT24c_write_sub   I2C1_write_sub
#else
#error have not setting i2c channel
#endif

#define I2C_ADDRESS    0xA8


void AT24c08_Init(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_AT24C08_FUNCTION_ENABLE

    #endif
}
void AT24c08_Open(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_AT24C08_FUNCTION_ENABLE

    #endif
}
void AT24c08_Close(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_AT24C08_FUNCTION_ENABLE

    #endif
}

void AT24c08_Finish(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_AT24C08_FUNCTION_ENABLE

    #endif
}
U8 AT24c08_ReadPage(BYTE addr, BYTE *pbuf, WORD page_len)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_AT24C08_FUNCTION_ENABLE

    #endif
}
U8 AT24c08_WritePage(BYTE addr, BYTE *pbuf, WORD page_len)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_AT24C08_FUNCTION_ENABLE

    #endif
}


U8 AT24c08_Write(BYTE addr, BYTE *pbuf, WORD len)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_AT24C08_FUNCTION_ENABLE
	U8 i;
	U8 j;
	U8 *wbuf;
	U32 r;
	volatile U8 temp;
	U8 i2c_addr = 0;
	U16 len1;

	wbuf = pbuf;
	len1 = len / (EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE);

	for (j=0; j<len1; j++)
	{
		for (i=0; i<EEPROM_PAGE_SIZE; i++)
		{
			i2c_addr = I2C_ADDRESS + (i << 1);
			r = AT24c_write_sub (i2c_addr, addr, wbuf, EEPROM_PAGE_BYTE);
			DBG_Assert(r == I2C_SUCCESS);
			wbuf += EEPROM_PAGE_BYTE;
			//delayms(8); // min delay 4ms
			delayms(4); // min delay 4ms
		}

		//addr += EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE;
		addr += EEPROM_PAGE_BYTE;
	}

	len1 = len - len1*(EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE);

	temp = len1/ EEPROM_PAGE_BYTE;
	for (i=0; i<temp; i++)
	{
		i2c_addr = I2C_ADDRESS + (i << 1);
		r = AT24c_write_sub (i2c_addr, addr, wbuf, EEPROM_PAGE_BYTE);
		DBG_Assert(r == I2C_SUCCESS);
		wbuf += EEPROM_PAGE_BYTE;
		//delayms(8); // min delay 4ms
		delayms(4); // min delay 4ms
	}

	temp = len1%EEPROM_PAGE_BYTE;
	if (temp)
	{
		i2c_addr = I2C_ADDRESS + (i << 1);
		//addr += EEPROM_PAGE_BYTE;
		//wbuf += EEPROM_PAGE_BYTE;

		r = AT24c_write_sub (i2c_addr, addr, wbuf, temp);
		DBG_Assert(r == I2C_SUCCESS);
		//delayms(8); // min delay 4ms
		delayms(4); // min delay 4ms
	}

	if (r == I2C_SUCCESS)
	{
		return TRUE;
	}
	
	return FALSE;

    #else
    return FALSE;
    #endif
}

U8 AT24c08_Read(BYTE addr, BYTE *pbuf, WORD len)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef EEPROM_AT24C08_FUNCTION_ENABLE
	U8 i;
	U8 j;
	U8 *rbuf;
	U32 r = I2C_SUCCESS;
	volatile U8 temp;
	U8 i2c_addr = 0;
	U16 len1;

	rbuf = pbuf;
	len1 = len / (EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE);

	for (j=0; j<len1; j++)
	{
		for (i=0; i<EEPROM_PAGE_SIZE; i++)
		{
			i2c_addr = I2C_ADDRESS + (i << 1);
			r = AT24c_read_sub (i2c_addr, addr, rbuf, EEPROM_PAGE_BYTE);
			DBG_Assert(r == I2C_SUCCESS);
			delayms(8);										// min delay 4ms
			rbuf += EEPROM_PAGE_BYTE;
		}
		
		//addr += EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE;
		addr += EEPROM_PAGE_BYTE;
	}

	len1 = len - len1*(EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE);

	temp = len1/ EEPROM_PAGE_BYTE;
	for (i=0; i<temp; i++)
	{
		i2c_addr = I2C_ADDRESS + (i << 1);
		r = AT24c_read_sub (i2c_addr, addr, rbuf, EEPROM_PAGE_BYTE);
		DBG_Assert(r == I2C_SUCCESS);
		delayms(8);										// min delay 4ms
		rbuf += EEPROM_PAGE_BYTE;
	}

	temp = len1%EEPROM_PAGE_BYTE;
	if (temp)
	{
		i2c_addr = I2C_ADDRESS + (i << 1);
		//addr += EEPROM_PAGE_BYTE;
		//rbuf += EEPROM_PAGE_BYTE;

		r = AT24c_read_sub (i2c_addr, addr, rbuf, temp);
		DBG_Assert(r == I2C_SUCCESS);
		delayms(8);										// min delay 4ms
	}

	if (r == I2C_ERROR)
	{
		memset(pbuf, 0, len);
	}

	if (r == I2C_SUCCESS)
	{
		return TRUE;
	}
	
	return FALSE;

    #else
    return FALSE;
    #endif

}



#endif

