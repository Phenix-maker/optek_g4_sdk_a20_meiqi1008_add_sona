/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief 
 *
 ****************************************************************************************
 */

#include "os_config.h"

#include "regmap.h"

#include "hw_timer.h"
#include "hw_audio_i2s.h"
#include "hw_hardware_aon.h"
#include "hw_watchdog.h"

#include "hal_uart.h"
#include "hal_gpio.h"

#include "interrupt.h"
#include "console_task.h"
#include "system_task.h"
#include "system_timer.h"
#include "app_entry.h"
#include "app_i2c.h"
#include "app_source.h"
#include "app_string_oem.h"

#include "LCDdevice.h"
#include "cpu_soft_reset.h"
#include "flash_boot.h"


void os_entry(void);

/**
 ****************************************************************************************
 * @brief app_main() - This function is the entry point to the program and is responsible for 
 * initializing the hardware, software, system, and starting the RTOS kernel.
 *
 * @param[in]  argc    		preserve parameter
 * @param[in]  argv     	preserve parameter
 *
 * @return 0, with no special significance
 *
 ****************************************************************************************
 */

enum RESET_TYPE {
	UPGRADE_SUCCESS = 0x55,
	UPGRADE_FAILED,
	UPGRADE_UNKNOWN,
	SW_EXCEPTION,
	HW_RESET,
};

typedef struct {
	U8 reset_type;
	U8 boot_param[15];
}__attribute__ ((packed)) BOOT_PARAM;

extern BOOT_PARAM boot_param;


BOOT_PARAM boot_param;

U8 watchdog_enable_status;

U8 upgrade_status_get(void)
{
	if (sram_boot_config[0] == 'U' && sram_boot_config[1] == 'P' &&
	sram_boot_config[2] == 'G')
	{
		if (sram_boot_config[3] == 'S' && sram_boot_config[4] == 'u' &&
				sram_boot_config[5] == 'c')
		{
			boot_param.reset_type = UPGRADE_SUCCESS;
			//app_main_data.media = MEDIA_UPGRADE;
		}
		else if (sram_boot_config[3] == 'E' && sram_boot_config[4] == 'r' &&
				sram_boot_config[5] == 'r')
		{
			boot_param.reset_type = UPGRADE_FAILED;
			//app_main_data.media = MEDIA_UPGRADE;
		}
		else
		{
			boot_param.reset_type = UPGRADE_UNKNOWN;
			//app_main_data.media = MEDIA_SDMMC;
			asm("nop");

			//boot_upgrade();
			//return;
		}
	}

#ifdef USB_HOST_ENABLE	
	else if (sram_boot_config[0] == 'U' && sram_boot_config[1] == 'S' &&
		sram_boot_config[2] == 'B' && sram_boot_config[3] == 'A' &&
		sram_boot_config[4] == 'P' && sram_boot_config[5] == 'P')
	{
		global_vari.audio_source = SOURCE_USB_MSC;
	}
#endif

#if 1//def BT_HCI_ENABLE
	else if (sram_boot_config[0] == 'H' && sram_boot_config[1] == 'C' &&
	         sram_boot_config[2] == 'I' && sram_boot_config[3] == 'A' &&
	         sram_boot_config[4] == 'P' && sram_boot_config[5] == 'P')
	{
		boot_param.reset_type = SW_EXCEPTION;
		global_vari.audio_source = SOURCE_BT;
	}
#endif

	else if (sram_boot_config[0] == 'A' && sram_boot_config[1] == 'U' &&
		sram_boot_config[2] == 'X' && sram_boot_config[3] == 'A' &&
		sram_boot_config[4] == 'P' && sram_boot_config[5] == 'P')
	{
		boot_param.reset_type = SW_EXCEPTION;
		global_vari.audio_source = SOURCE_AUX;
	}
	else
	{
		boot_param.reset_type = HW_RESET;
		//global_vari.audio_source = app_defalt_poweron_souce();
		global_vari.audio_source = DEFAULT_APP_SOURCE;
	}

	#if 1//def UPGRADE_PROJECT_ENABLE
	if (boot_param.reset_type == UPGRADE_SUCCESS)
	{
		#if 1
		U8 *pDest = sram_boot_config;
		U8 *pSrc = flash_boot_config;

        hw_wdt_disable();
		CFasm_memcpy(pDest, pSrc, sizeof(sram_boot_config));
		#endif

		//global_vari.audio_source = app_defalt_poweron_souce();
		global_vari.audio_source = DEFAULT_APP_SOURCE;
 
		DBG_NO_OS_Printf("upgrade success\n\r");

		#ifdef LCD_MODULE_ENABLE
		app_lcd_device.display_char(STRING_UPGRADE_SUCCESS, 0, 3);
		app_lcd_device.display_char(STRING_NULL, 1, 0);
		#endif

		U8 damp_cnt = 5;
		while(damp_cnt--)
		{
			DBG_NO_OS_Printf("%d\n\r", damp_cnt);
			delayms(1000);
		}

		#if ( (defined WATCH_DOG) && !(defined JTAG_DEBUG_MODE))
        hw_wdt_enable(WDT_TIME_TO_S(7));
        #endif 

		// watchdog_time_reset();

    	#if 1
        CPU_SoftReset_Flag_Setting((U8 *)sram_boot_config, TRUE);
        #endif
	}
	else if (boot_param.reset_type == UPGRADE_FAILED)
	{
		#if 1
		U8 *pDest = sram_boot_config;
		U8 *pSrc = flash_boot_config;

        hw_wdt_disable();
		CFasm_memcpy(pDest, pSrc, sizeof(sram_boot_config));
		#endif

		DBG_NO_OS_Printf("\n\rupgrade failed!, pls try again\n\r");
 
		#ifdef LCD_MODULE_ENABLE
		app_lcd_device.display_char(STRING_UPGRADE_ERROR, 0, 3);
		app_lcd_device.display_char(STRING_NULL, 1, 0);
		#endif
	
		U8 damp_cnt = 5;
		while(damp_cnt--)
		{
			DBG_NO_OS_Printf("%d\n\r", damp_cnt);
			delayms(1000);
		}

		#if ( (defined WATCH_DOG) && !(defined JTAG_DEBUG_MODE))
        hw_wdt_enable(WDT_TIME_TO_S(7));
        #endif 
	}
    else if (boot_param.reset_type == UPGRADE_UNKNOWN)
    {
 		DBG_NO_OS_Printf("\n\rupgrade failed! 1, pls try again\n\r");
 
		#if 1
		U8 *pDest = sram_boot_config;
		U8 *pSrc = flash_boot_config;

        hw_wdt_disable();
		CFasm_memcpy(pDest, pSrc, sizeof(sram_boot_config));
		#endif

		#ifdef LCD_MODULE_ENABLE
		app_lcd_device.display_char(STRING_UPGRADE_ERROR, 0, 3);
		app_lcd_device.display_char(STRING_NULL, 1, 0);
		#endif

		U8 damp_cnt = 5;
		while(damp_cnt--)
		{
			DBG_NO_OS_Printf("%d\n\r", damp_cnt);
			delayms(1000);
		}

		#if ( (defined WATCH_DOG) && !(defined JTAG_DEBUG_MODE))
        hw_wdt_enable(WDT_TIME_TO_S(7));
        #endif    
    }
	#endif

}


int app_main(int argc, char *argv)
{

	#ifdef DSP_SONA_MODULE_ENABLE
	void flash_lib_sona_code_copy(void);
	flash_lib_sona_code_copy();
	#endif

	hw_32k_trim_set(TRIM_32K_PARAM);
	
	hw_pll_clock_int();
	
	hw_cpu_clock_init();
	
	#if defined LDO_VOLTAGE_SEL_3P3V
	hw_ldo_init(LDO_VOLTAGE_3P3V);
	#elif defined LDO_VOLTAGE_SEL_2P5V
	hw_ldo_init(LDO_VOLTAGE_2P5V);
	#else
	DBG_Assert(FALSE);
	#endif

	hw_timer_init();
	
	xt_set_interrupt_handler(21, L1_int21_extlevel, NULL);
	
	#ifdef GPIO_INT_ENABLE
	xt_set_interrupt_handler(0, L1_int0_extlevel, NULL);
    xt_ints_on(LEVEL1_INT0_MASK);
	#endif

	xt_set_interrupt_handler(1, L1_int1_extlevel, NULL);
	
	xt_set_interrupt_handler(3, L1_int3_extlevel, NULL);

	#if ( (defined USB_HOST_ENABLE) || (defined USB_DEV_ENABLE))
	xt_set_interrupt_handler(8, L2_int8_extlevel, NULL);
	#endif

	xt_set_interrupt_handler(9, L3_int9_extlevel, NULL);

#ifdef BTDM52_ENABLE
	xt_set_interrupt_handler(19, L2_int19_extlevel, NULL);//btdm
#endif

#ifdef TIMER1_ENABLE
	xt_set_interrupt_handler(10, L3_int10_timer1, NULL);
	_xt_tick_divisor_init();
#endif


	#ifdef AUDIO_MI2S_RX_ENABLE
	hw_audio_mi2s_rx_init();
	#endif
  
	sdk_common_init();

	app_PreTaskSch_init();

	hw_cpu_clk_set(SYSTEM_CLK_FREQ);

	#ifdef RTOS_ENABLE
	os_entry();
	#endif


	while(1)
	{
		asm("nop");
	}
	
	return 0;
}


/**
 ****************************************************************************************
 * @brief app_main() - This function is the entry point to the program and is responsible for 
 * initializing the hardware, software, system, and starting the RTOS kernel.
 *
 * @param[out]  argc    	preserve parameter
 * @param[out]  argv     	preserve parameter
 *
 * @return 0, with no special significance
 *
 ****************************************************************************************
 */
int main(void)
{
	int argc;
	char argv;

	/*TODO later for different application*/
	argc = 0;
	argv = NULL;

	app_main (argc, argv);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
