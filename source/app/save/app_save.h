/*
 * app_save.h
 *
 *  Created on: 2024/2/19
 *      Author: Windows
 */

#ifndef __APP_SAVE_H__
#define __APP_SAVE_H__


typedef struct {
	U8 pre_source;
	U8 volume;
	U8 mute;
	U8 eq_mode;

	U8 bass;
	U8 treble;
    U8 rev[2];

} APP_SAVE_SYS_DATA;

extern U8 ROM_SAVE_DATA1_START[];
extern U8 ROM_SAVE_DATA2_START[];

#if defined EEPROM_AT24C01

#define APP_PAGE_OFFSET                 0

#elif defined EEPROM_AT24C02

#define APP_PAGE_OFFSET                 0

#elif defined EEPROM_AT24C04

#define APP_PAGE_OFFSET                 0

#elif defined EEPROM_AT24C08

#define BT_EEPROM_OFFSET                (0)

//used 12 pages(12*64=768) in EEPROM_AT24C08:EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE.
#define APP_SAVE_BT_MULTI_LINK_PAGE_ADDR   (BT_EEPROM_OFFSET) //0-11
#define BT_MULTI_LINK_NEED_PAGE_SIZE    (12)

#define APP_PAGE_OFFSET                 (BT_MULTI_LINK_NEED_PAGE_SIZE)

#define APP_SAVE_DATA_PAGE_ADDR         (EEPROM_PAGE_BYTE*(APP_PAGE_OFFSET+0)) //need one page,EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE bytes in address "0"

#define APP_SAVE_SYS_PAGE_ADDR          (EEPROM_PAGE_BYTE*(APP_PAGE_OFFSET+1)) //need one page.

#define TUNER_PROG_PRESET_PAGE_ADDR     (EEPROM_PAGE_BYTE*(APP_PAGE_OFFSET+1+1)) //need two pages.


#ifdef RESUME_SAVE_TO_EEPROM
#define RESUME_INFO_PAGE_ADDR           (EEPROM_PAGE_BYTE*(APP_PAGE_OFFSET+1+1+9+1)) //need one page

#endif

#elif defined EEPROM_AT24C16


#if 1
#define BT_EEPROM_OFFSET                (0)

//used 12 pages(12*64=768) in EEPROM_AT24C08::EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE.
//used 6 pages(6*128=768) in EEPROM_AT24C16:EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE.
#define APP_SAVE_BT_MULTI_LINK_PAGE_ADDR   (BT_EEPROM_OFFSET) //0-5
#define BT_MULTI_LINK_NEDD_PAGE_SIZE    (6)
#endif


#define APP_PAGE_OFFSET                 (0+BT_MULTI_LINK_NEDD_PAGE_SIZE)


#define APP_SAVE_DATA_PAGE_ADDR         (EEPROM_PAGE_BYTE*(APP_PAGE_OFFSET+0)) //0

#define APP_SAVE_SYS_PAGE_ADDR          (EEPROM_PAGE_BYTE*(APP_PAGE_OFFSET+1)) //need 1 page,EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE bytes in address "0"

#define TUNER_PROG_PRESET_PAGE_ADDR     (EEPROM_PAGE_BYTE*(APP_PAGE_OFFSET+1+1)) //need 2 pages,EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE bytes in address "1"


#define DAB_PROG_PRESET_PAGE_ADDR       (EEPROM_PAGE_BYTE*(APP_PAGE_OFFSET+1+1+5)) //need 1 page or two pages , EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE bytes in address "1"


#ifdef RESUME_SAVE_TO_EEPROM
#define RESUME_INFO_PAGE_ADDR           (EEPROM_PAGE_BYTE*(APP_PAGE_OFFSET+1+1+5+1)) //need 1 page, EEPROM_PAGE_BYTE*EEPROM_PAGE_SIZE bytes

#endif



#endif

void app_save_system_data_init(void);
void app_save_system_data(void);
void app_save_system_fix_data(APP_SAVE_SYS_DATA *pSysFixData);

void app_nav_save_bt_data(void);
void app_nav_get_bt_data(void);
#endif /* __APP_SAVE_H__ */
