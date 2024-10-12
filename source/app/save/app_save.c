/*
 * app_save.c
 *
 *  Created on: 2024/2/19
 *      Author: Windows
 */
#include "os_config.h"

#include "common.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "codec.h"


#include "hw_timer.h"

#include "interrupt.h"
#include "hal_sflash.h"

#ifdef SAVE_MODULE_ENABLE
#include "audio_device.h"

#ifdef EEPROM_AT24C02
#include "AT24C02.h"
#endif

#ifdef EEPROM_AT24C08
#include "AT24C08.h"
#endif

#ifdef SAVE_MODULE_FROM_EEPROM
#include "eeprom_device.h"
#endif

#include "app_source.h"
#include "app_save.h"



#define SAVE_MODULE_FUNCTION_ENABLE
//#define SAVE_MODULE_FUNCTION_DEBUG
//#define DBG_Printf


U8 app_flash_program_read_data(U32 addr, U32 *buf, U32 byte_len)
{

	U8 ret = FALSE;

	ret = hal_sflash_read(addr, buf, byte_len);

	return ret;
}

U8 app_flash_program_user_data(U32 addr, U32 *buf, U32 byte_len)
{

	U8 ret = FALSE;

	ret = hal_sflash_write(addr, buf, byte_len);

	return ret;
}


void app_save_system_data_init(void)
{
	DBG_Printf("%s\n\r", __func__);

	#ifdef SAVE_MODULE_FUNCTION_ENABLE
	U8 re_saving = FALSE;
	U8 len;
	U32 addr;
	APP_SAVE_SYS_DATA app_save_sys_data;
	APP_SAVE_SYS_DATA *pSysData = (APP_SAVE_SYS_DATA *)&app_save_sys_data;

	len = sizeof (APP_SAVE_SYS_DATA);

	#ifdef SAVE_MODULE_FROM_EEPROM
	addr = APP_SAVE_SYS_PAGE_ADDR;
	eeprom_device_Read(addr, (U8 *)pSysData, len);
	#else
	addr = (U32)ROM_SAVE_DATA1_START;
	app_flash_program_read_data(addr, (U32 *)pSysData, len);
	#endif


	DBG_Printf("volume:%d\n\r", app_save_sys_data.volume);

	DBG_Printf("bass:%d\n\r", app_save_sys_data.bass);

	DBG_Printf("treble:%d\n\r", app_save_sys_data.treble);

	DBG_Printf("pre_source:%d\n\r", app_save_sys_data.pre_source);

	if (app_save_sys_data.volume > VOLUME_MAX)
	{
		app_save_sys_data.volume = VOLUME_DEFAULT;
		re_saving = TRUE;
	}

	if ((audio_vari.eq_mode > EQ_POP)||(audio_vari.eq_mode < EQ_FLAT))
	{
		app_save_sys_data.eq_mode = EQ_DEFAULT;
		re_saving = TRUE;
	}	

	if (app_save_sys_data.bass > BASS_MAX)
	{
		app_save_sys_data.bass = BASS_DEFAULT;
		re_saving = TRUE;
	}

	if (app_save_sys_data.treble > TREBLE_MAX)
	{
		app_save_sys_data.treble = TREBLE_DEFAULT;
		re_saving = TRUE;
	}	

	if ((app_save_sys_data.pre_source >= MEDIA_LAST)||(app_save_sys_data.pre_source <= SOURCE_UNKNOWN))
	{
		app_save_sys_data.pre_source = DEFAULT_APP_SOURCE;
		re_saving = TRUE;
	}	


	if (re_saving)
	{
		#ifdef SAVE_MODULE_FROM_EEPROM
		eeprom_device_Write(addr, (U8 *)pSysData, len);
		#ifdef SAVE_MODULE_FUNCTION_DEBUG
		eeprom_device_Read(addr, (U8 *)pSysData, len);
		#endif
		#else
        app_flash_program_user_data(addr, (U32 *)pSysData, len);
        #ifdef SAVE_MODULE_FUNCTION_DEBUG
        app_flash_program_read_data(addr, (U32 *)pSysData, len);
        #endif
		#endif
	}


	/*
	* first_power_on
	* */
	audio_vari.volume = app_save_sys_data.volume;

	global_vari.audio_source = app_save_sys_data.pre_source;

	audio_vari.bass = app_save_sys_data.bass;

	audio_vari.treble = app_save_sys_data.treble;

	DBG_Printf("audio_vari.volume:%d\n\r", audio_vari.volume);

	DBG_Printf("audio_vari.bass:%d\n\r", audio_vari.bass);

	DBG_Printf("audio_vari.treble:%d\n\r", audio_vari.treble);
	
	DBG_Printf("global_vari.audio_source:%d\n\r", global_vari.audio_source);

	asm("nop");
	#endif
}

void app_save_system_data(void)
{
	DBG_Printf("%s\n\r", __func__);
	#ifdef SAVE_MODULE_FUNCTION_ENABLE
	U8 re_saving = FALSE;
	U8 len;
	U32 addr;
	APP_SAVE_SYS_DATA app_save_sys_data;
	APP_SAVE_SYS_DATA *pSysData = (APP_SAVE_SYS_DATA *)&app_save_sys_data;

	len = sizeof (APP_SAVE_SYS_DATA);

	#ifdef SAVE_MODULE_FROM_EEPROM
	addr = APP_SAVE_SYS_PAGE_ADDR;
	eeprom_device_Read(addr, (U8 *)pSysData, len);
	#else
	addr = (U32)ROM_SAVE_DATA1_START;
	app_flash_program_read_data(addr, (U32 *)pSysData, len);
	#endif

	DBG_Printf("save addr:0x%x,len=%d\n\r", addr, len);

	if (audio_vari.volume != app_save_sys_data.volume)
	{
		if (audio_vari.volume <= VOLUME_DEFAULT)
		{
			app_save_sys_data.volume = audio_vari.volume;
			re_saving = TRUE;
		}
	}

	if (audio_vari.mute != app_save_sys_data.mute)
	{
		app_save_sys_data.mute = audio_vari.mute;
		re_saving = TRUE;
	}

	if (audio_vari.eq_mode != app_save_sys_data.eq_mode)
	{
		if ((audio_vari.eq_mode <= EQ_POP)&&(audio_vari.eq_mode >= EQ_FLAT))
		{
			app_save_sys_data.eq_mode = audio_vari.eq_mode;
			re_saving = TRUE;
		}
	}

	if (audio_vari.bass != app_save_sys_data.bass)
	{
		if ((audio_vari.bass <= BASS_MAX)&&(audio_vari.bass >= BASS_MIN))
		{
			app_save_sys_data.bass = audio_vari.bass;
			re_saving = TRUE;
		}
	}

	if (audio_vari.treble != app_save_sys_data.treble)
	{
		if ((audio_vari.treble <= TREBLE_DEFAULT)&&(audio_vari.treble >= TREBLE_MIN))
		{
			app_save_sys_data.treble = audio_vari.treble;
			re_saving = TRUE;
		}
	}	

	if (global_vari.audio_source != app_save_sys_data.pre_source)
	{
		app_save_sys_data.pre_source = global_vari.audio_source;
		re_saving = TRUE;
	}


	if (re_saving)
	{
		#ifdef SAVE_MODULE_FROM_EEPROM
		eeprom_device_Write(addr, (U8 *)pSysData, len);
		#ifdef SAVE_MODULE_FUNCTION_DEBUG
		eeprom_device_Read(addr, (U8 *)pSysData, len);
		#endif
		#else
		app_flash_program_user_data(addr, (U32 *)pSysData, len);
		delayms(5);
		DBG_Printf("save data: \npre_source:%d,pre_vol:%d,eq_mode:%d,bass:%d,treble:%d\n\r", app_save_sys_data.pre_source, app_save_sys_data.volume, app_save_sys_data.eq_mode, app_save_sys_data.bass, app_save_sys_data.treble);
		#ifdef SAVE_MODULE_FUNCTION_DEBUG
		app_flash_program_read_data(addr, (U32 *)pSysData, len);
		delayms(5);
		DBG_Printf("save data: \npre_source:%d,pre_vol:%d,eq_mode:%d,bass:%d,treble:%d\n\r", app_save_sys_data.pre_source, app_save_sys_data.volume, app_save_sys_data.eq_mode, app_save_sys_data.bass, app_save_sys_data.treble);
		#endif
		#endif
	}

	asm("nop");
	#endif
}

void app_save_system_fix_data(APP_SAVE_SYS_DATA *pSysFixData)
{
	DBG_Printf("%s\n\r", __func__);
	#ifdef SAVE_MODULE_FUNCTION_ENABLE
	U8 re_saving = FALSE;
	U8 len;
	U32 addr;
	APP_SAVE_SYS_DATA app_save_sys_data;
	APP_SAVE_SYS_DATA *pSysData = (APP_SAVE_SYS_DATA *)&app_save_sys_data;

	len = sizeof (APP_SAVE_SYS_DATA);

	#ifdef SAVE_MODULE_FROM_EEPROM
	addr = APP_SAVE_SYS_PAGE_ADDR;
	eeprom_device_Read(addr, (U8 *)pSysData, len);
	#else
	addr = (U32)ROM_SAVE_DATA1_START;
	app_flash_program_read_data(addr, (U32 *)pSysData, len);
	#endif

	DBG_Printf("save fix data: addr:0x%x,len=%d\n\r", addr, len);

	if (pSysFixData->volume != app_save_sys_data.volume)
	{
		if (pSysFixData->volume <= VOLUME_DEFAULT)
		{
			app_save_sys_data.volume = pSysFixData->volume;
			re_saving = TRUE;
		}
	}

	// if (pSysFixData->mute != app_save_sys_data.mute)
	// {
	// 	app_save_sys_data.mute = pSysFixData->mute;
	// 	re_saving = TRUE;
	// }

	if (pSysFixData->eq_mode != app_save_sys_data.eq_mode)
	{
		if ((pSysFixData->eq_mode <= EQ_POP)&&(pSysFixData->eq_mode >= EQ_FLAT))
		{
			app_save_sys_data.eq_mode = pSysFixData->eq_mode;
			re_saving = TRUE;
		}
	}

	if (pSysFixData->bass != app_save_sys_data.bass)
	{
		if ((pSysFixData->bass <= BASS_MAX)&&(pSysFixData->bass >= BASS_MIN))
		{
			app_save_sys_data.bass = pSysFixData->bass;
			re_saving = TRUE;
		}
	}

	if (pSysFixData->treble != app_save_sys_data.treble)
	{
		if ((pSysFixData->treble <= TREBLE_DEFAULT)&&(pSysFixData->treble >= TREBLE_MIN))
		{
			app_save_sys_data.treble = pSysFixData->treble;
			re_saving = TRUE;
		}
	}	

	if (pSysFixData->pre_source != app_save_sys_data.pre_source)
	{
		app_save_sys_data.pre_source = pSysFixData->pre_source;
		re_saving = TRUE;
	}


	if (re_saving)
	{
		#ifdef SAVE_MODULE_FROM_EEPROM
		eeprom_device_Write(addr, (U8 *)pSysData, len);
		#ifdef SAVE_MODULE_FUNCTION_DEBUG
		eeprom_device_Read(addr, (U8 *)pSysData, len);
		#endif
		#else
		app_flash_program_user_data(addr, (U32 *)pSysData, len);
		delayms(5);
		DBG_Printf("save data: \npre_source:%d,pre_vol:%d,eq_mode:%d,bass:%d,treble:%d\n\r", app_save_sys_data.pre_source, app_save_sys_data.volume, app_save_sys_data.eq_mode, app_save_sys_data.bass, app_save_sys_data.treble);
		#ifdef SAVE_MODULE_FUNCTION_DEBUG
		app_flash_program_read_data(addr, (U32 *)pSysData, len);
		delayms(5);
		DBG_Printf("save data: \npre_source:%d,pre_vol:%d,eq_mode:%d,bass:%d,treble:%d\n\r", app_save_sys_data.pre_source, app_save_sys_data.volume, app_save_sys_data.eq_mode, app_save_sys_data.bass, app_save_sys_data.treble);
		#endif
		#endif
	}

	asm("nop");
	#endif
}
#endif


