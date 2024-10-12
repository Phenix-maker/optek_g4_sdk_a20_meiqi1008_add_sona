/*
 * cpu_soft_reset.c
 *
 *  Created on: 2024/1/29
 *      Author: Windows
 */

#include "os_config.h"

#include "regmap.h"

#include "hw_timer.h"
#include "hw_watchdog.h"

#include "cpu_soft_reset.h"
#include "app_string_oem.h"
#include "app_main.h"

#include "flash_boot.h"
#include "LCDdevice.h"
#include "bt_common.h"
#include "hal_key_adc.h"
#include "hw_pwm.h"

#define UPGRADE_SFLASH_SAVE_FLAG

extern U8 UPGRADE_HW_SET_RODATA_ADDR[];

//extern UPGRADE_HW_SET_DATA upgrade_hw_set_data;
extern const UPGRADE_HW_SET_DATA upgrade_hw_set_data;


#ifndef UPGRADE_PROJECT_ENABLE
void CPU_SoftReset_Flag_Setting(U8 *pBuf, U8 reset) __INTERNAL_RAM_TEXT;
#endif
void CPU_SoftReset_Flag_Setting(U8 *pBuf, U8 reset)
{
#if 1
	U8 *pSrc = pBuf;
	U8 *pDest = sram_boot_config;
	U8 len;
	TX_INTERRUPT_SAVE_AREA;

	#ifdef UPGRADE_PROJECT_ENABLE
	DBG_Printf("softReset:%s\n\r", pSrc);
	delayms(20);
	#endif

	#ifdef BOOT_FROM_FLASH
	#if 0
	len = strlen((const char *)pSrc);
	#else
	len = 16;
	#endif

	CFasm_memcpy(pDest, pSrc, len);

	if (reset)
	{
		sram_boot_config[15] = 0;

		#ifdef UPGRADE_PROJECT_ENABLE
		DBG_Printf("sram_boot_config0:%s\n\r", sram_boot_config);
		delayms(10);

		DBG_Printf("Waiting for watchdog\n\r");
		delayms(10);
		#endif

		#ifdef UPGRADE_PROJECT_ENABLE
		//watchdog reset
		TX_DISABLE;
		#endif

		//watchdog_init(FALSE, 0);
        hw_wdt_enable(1);

		while (1);
		//TX_RESTORE;
	}
	#endif
#endif
}

void app_nav_upgrade_project(void)
{
	U8 i;
	U8 len;
	U8 ret;
	#ifndef BT_BLE_UPGRADE_ENABLE
	#ifdef UPGRADE_SFLASH_SAVE_FLAG
	UPGRADE_HW_SET_DATA upgrade_set_data;
	#endif
	#endif
	//U8 program_flag = FALSE;
	// bt_status.bt_main_status = enBT_UPGRAGE;q

	hal_gpio_output_level_set(BAT1_LED_G,GPIO_LOW);
	hal_gpio_output_level_set(BAT2_LED_G,GPIO_LOW);

	hal_gpio_output_level_set(F_LED_CTL,GPIO_HIGH);
	// hw_pwm_set(0xfff); //0V
	delayms(2000);
	DBG_Printf( "%s\n\r", __func__);
	delayms(5);

	#ifdef LCD_MODULE_ENABLE
	app_lcd_device.display_char(STRING_UPGRADE, 0, 3);
	app_lcd_device.display_char(STRING_NULL, 1, 0);
	#endif

	#if 0//def BT_BLE_FUNCTION_ENABLE
	kBtHCI_adv_disable();
	#endif


	decode_exit_req_set();
	decode_exit_wait_forever_msgSend();
	//delayms(5);

	//TX_DISABLE;
	//vTaskSuspendAll(); 


	#if 0//ndef BT_BLE_UPGRADE_ENABLE
	#ifdef UPGRADE_SFLASH_SAVE_FLAG
	UPGRADE_HW_SET_DATA *pDet = (UPGRADE_HW_SET_DATA *)&upgrade_set_data; //in dram
	memset(pDet, 0, sizeof (UPGRADE_HW_SET_DATA));

	#if 1//def UPGRADE_GPIO_FIXED_BY_TABLE
	CFasm_memcpy(pDet, &upgrade_hw_set_data, sizeof(upgrade_hw_set_data));
	#endif

	#if 1
	pDet->sflash_size  = SFLASH_BIT_SIZE;
//	pDet->gpio0_din    = REG_GPIO0_DIN;
	pDet->gpio0_dout   = REG_GPIO0_DOUT;
	pDet->gpio0_doe    = REG_GPIO0_DOE;
	pDet->gpio0_func   = REG_GPIO0_FUNC;
	pDet->gpio0_ie     = REG_GPIO0_IE;

	pDet->gpio3_func   = REG_GPIO3_FUNC;

	#if defined UPGRADE_BY_USB
	pDet->upgrade_media = enUSB_DEV_UPGRADE;
	#elif defined UPGRADE_BY_BT
	pDet->upgrade_media = enBT_DEV_UPGRADE;
	#else
	#error Must Define upgrading media.
	#endif

	pDet->sd_detect_pin_index = (11);
	#endif
	
	#if 0
	CFasm_memcpy(pDet->bt_name,bt_name, sizeof(bt_name));
	CFasm_memcpy(pDet->bt_init_name,bt_init_name, sizeof(bt_init_name));
	CFasm_memcpy(pDet->bt_upgrade_file_name, bt_upgrade_file_name, sizeof(bt_upgrade_file_name));
	#endif

	if (memcmp(UPGRADE_HW_SET_RODATA_ADDR, pDet, sizeof(UPGRADE_HW_SET_DATA)) !=0)
	{
		DBG_Printf( "upgrade parameter changed\n\r");
        #ifdef SAVE_MODULE_ENABLE
		app_flash_program_user_data((U32)UPGRADE_HW_SET_RODATA_ADDR, pDet, sizeof(UPGRADE_HW_SET_DATA));
        #endif
	}
	else
	{
		DBG_Printf( "upgrade parameter not changed\n\r");
	}
	
	pDet = (UPGRADE_HW_SET_DATA *)UPGRADE_HW_SET_RODATA_ADDR;
	DBG_Printf( "bt_name1:%s\n\r", pDet->bt_name);
	delayms(20);

	DBG_Printf( "bt_init_name1:%s\n\r", pDet->bt_init_name);
	delayms(20);

	DBG_Printf( "bt_upgrade_file_name1:%s\n\r", pDet->bt_upgrade_file_name);
	delayms(20);
	#endif //UPGRADE_SFLASH_SAVE_FLAG
	#endif //BT_BLE_FUNCTION_ENABLE


	#if 0//def UPGRADE_FLASH_ALL

	#if defined UPGRADE_BY_USB
	CPU_SoftReset_Flag_Setting((U8 *)STRING_USB_UPGRADE_ALL, TRUE); //Please use the usb_sd_upgrade_base.bin
	#elif defined UPGRADE_BY_BT
	CPU_SoftReset_Flag_Setting((U8 *)STRING_BT_UPGRADE_ALL, TRUE); ////Please use the bt_upgrade_base.bin
	#else
	#error Must Define upgrading media.
	#endif
	#else
	
	#if defined UPGRADE_BY_USB
	CPU_SoftReset_Flag_Setting((U8 *)STRING_USB_UPGRADE_PARTIAL, TRUE); //Please use the usb_sd_upgrade_base.bin
	#elif defined UPGRADE_BY_BT
	CPU_SoftReset_Flag_Setting((U8 *)STRING_BT_UPGRADE_PARTIAL, TRUE);  //Please use the bt_upgrade_base.bin
	#else
	#error Must Define upgrading media.
	#endif
	
	#endif //UPGRADE_FLASH_ALL


	//TX_RESTORE;
	//while (1);
}

