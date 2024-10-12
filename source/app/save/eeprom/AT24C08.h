/*
 * AT24C08.h
 *
 *  Created on: 2024/2/19
 *      Author: Windows
 */

#ifndef __AT24C08_H__
#define __AT24C08_H__

#ifdef EEPROM_AT24C08
#define EEPROM_PAGE_SIZE      4
#define EEPROM_PAGE_BYTE      16 //16 bytes

void AT24c08_Init(void);
void AT24c08_Open(void);
void AT24c08_Close(void);
void AT24c08_Finish(void);
U8 AT24c08_ReadPage(BYTE addr, BYTE *pbuf, WORD page_len);
U8 AT24c08_WritePage(BYTE addr, BYTE *pbuf, WORD page_len);
U8 AT24c08_Write(BYTE addr, BYTE *pbuf, WORD len);
U8 AT24c08_Read(BYTE addr, BYTE *pbuf, WORD len);

#define EEPROM_DEV_AT24C08		AT24c08_Init, \
								AT24c08_Open, \
								AT24c08_Close, \
								AT24c08_Finish, \
								AT24c08_ReadPage, \
								AT24c08_WritePage, \
								AT24c08_Read,	\
								AT24c08_Write

#endif

#endif /* __AT24C08_H__ */
