/*
 * eeprom_device.c
 *
 *  Created on: 2024/2/20
 *      Author: Windows
 */
#include "os_config.h"

#ifdef SAVE_MODULE_FROM_EEPROM
#include "eeprom_device.h"

#ifdef EEPROM_AT24C02
#include "AT24C02.h"
const SAVE_EEPROM_MOUDLE eepromDev = {EEPROM_DEV_AT24C02};
#elif (defined EEPROM_AT24C08)
#include "AT24C08.h"
const SAVE_EEPROM_MOUDLE eepromDev = {EEPROM_DEV_AT24C08};
#elif (defined EEPROM_AT24C16)
#include "AT24C16.h"
const SAVE_EEPROM_MOUDLE eepromDev = {EEPROM_DEV_AT24C16};
#else


void eeprom_device_none_void(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE

	#endif
}

U8 eeprom_device_none_ReadPage(BYTE addr, BYTE *pbuf, WORD page_len)
{
	DBG_Printf("%s\n\r"£¬__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE

	#endif
}

U8 eeprom_device_none_WritePage(BYTE addr, BYTE *pbuf, WORD page_len)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE

	#endif
}

U8 eeprom_device_none_Read(BYTE addr, BYTE *pbuf, WORD len)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE

	#endif
}

U8 eeprom_device_none_Write(BYTE addr, BYTE *pbuf, WORD len)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE

	#endif
}

const SAVE_EEPROM_MOUDLE eepromDev = {EEPROM_DEV_NONE};

#endif


#define EEPROM_DEVICE_FUNCTION_ENABLE
//#define DBG_Printf

void eeprom_device_Init(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE
	eepromDev.init();
	#endif
}

void eeprom_device_Open(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE
	eepromDev.open();
	#endif
}

void eeprom_device_Close(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE
	eepromDev.close();
	#endif
}

void eeprom_device_Finish(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE
	eepromDev.fini();
	#endif
}

U8 eeprom_device_ReadPage(BYTE addr, BYTE *pbuf, WORD page_len)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE
	eepromDev.ReadPage(addr, pbuf, page_len);
	#endif
}

U8 eeprom_device_WritePage(BYTE addr, BYTE *pbuf, WORD page_len)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE
	eepromDev.WritePage(addr, pbuf, page_len);
	#endif
}

U8 eeprom_device_Read(BYTE addr, BYTE *pbuf, WORD len)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE
	eepromDev.Read(addr, pbuf, len);
	#endif
}

U8 eeprom_device_Write(BYTE addr, BYTE *pbuf, WORD len)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef EEPROM_DEVICE_FUNCTION_ENABLE
	eepromDev.Write(addr, pbuf, len);
	#endif
}







#endif

