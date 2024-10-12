/*
 * hw_watchdog.c
 *
 *  Created on: 2024/1/29
 *      Author: Windows
 */
#include "os_config.h"

#include "mem_reloc.h"

#include "hw_gpio.h"
#include "hw_timer.h"
#include "hw_hardware_aon.h"
#include "hw_watchdog.h"


//0.25s one step
void hw_wdt_enable(U8 step)
{
	volatile U32 val;
	AON_CTRL_ST *reg_val_cfg = (AON_CTRL_ST *)&val;

	DBG_Assert(step <= 0x1F);

	if (step > 0x1F)
	{
		step = 0x1F;
		printf_error("err setting wdt step!\n\r");
	}

	//checking watchdog
	REG_WATDOG_UNLOCK = UNLOCK_DWORD;
	REG_WATDOG_RESET = WDT_RESET_DWORD0;

	//write
	val = AON_CTRL_ADDR;
	reg_val_cfg->soc_ramp = 7;       //250us
	reg_val_cfg->soc_wdt_tov = step; //250ms/step
	reg_val_cfg->soc_wdt_en = 1;     //enable, default is enable
	hw_hardware_aonreg_write(&AON_CTRL_ADDR, val);
	delayus(5);

	REG_WATDOG_UNLOCK = UNLOCK_DWORD;
	REG_WATDOG_RESET = WDT_RESET_DWORD0;
}

void hw_wdt_disable(void)
{
	volatile U32 val;
	AON_CTRL_ST *reg_val_cfg = (AON_CTRL_ST *)&val;

	//checking watchdog
	REG_WATDOG_UNLOCK = UNLOCK_DWORD;
	REG_WATDOG_RESET = WDT_RESET_DWORD0;

	//write
	val = AON_CTRL_ADDR;
	reg_val_cfg->soc_ramp = 7;     //250us
	reg_val_cfg->soc_wdt_tov = 8;  //0.5s
	reg_val_cfg->soc_wdt_en = 0;   //enable, default is enable
	hw_hardware_aonreg_write(&AON_CTRL_ADDR, val);
	delayus(5);

	REG_WATDOG_UNLOCK = UNLOCK_DWORD;
	REG_WATDOG_RESET = WDT_RESET_DWORD0;	
}

void watchdog_time_reset(void)
{
#if 1//( (defined WATCH_DOG) || !(defined JTAG_DEBUG_MODE) )
	REG_WATDOG_UNLOCK = UNLOCK_DWORD;
	REG_WATDOG_RESET = WDT_RESET_DWORD0;
	//REG_WATDOG_UNLOCK = UNLOCK_DWORD;
	//REG_WATDOG_RESET = WDT_RESET_DWORD1;
#endif
}
