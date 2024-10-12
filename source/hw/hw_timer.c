#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hw_hardware_aon.h"

#include "interrupt.h"



#define CLOCKS_PER_MS           ( SYSTEM_CLK_FREQ / 1000 )
#define CLOCKS_PER_US           ( CLOCKS_PER_MS / 1000 )

void timer_delayus(U32 xus)
{
	U32 StartTime;
	U32 CurTime;
	U32 EndTime;
	U8 bTimeOut;

	StartTime = read_ccount();
	EndTime = StartTime + (xus*CLOCKS_PER_US);

	while (1)
	{
		CurTime = read_ccount();

		if (StartTime <= EndTime)
		{
			bTimeOut = ((CurTime >= StartTime) && (CurTime < EndTime))? FALSE : TRUE;
			if (bTimeOut)
			{
				break;
			}
		}
		else
		{
			bTimeOut = ((CurTime >= StartTime) || (CurTime < EndTime))? FALSE : TRUE;
			if (bTimeOut)
			{
				break;
			}
		}
	}
}

void timer_delayms(U32 xms)
{
	U32 i;

	for (i=0; i<xms; i++)
	{
		timer_delayus(1000);
		watchdog_time_reset();
	}
}

U8 timer_checkTimeout(U32 StartTime, U32 xus)
{
	U32 CurTime;
	U32 EndTime;
	U8 bTimeOut;

	//uSecod
	CurTime = read_ccount();
	EndTime = StartTime + (xus*CLOCKS_PER_US);
	
	if ( StartTime <= EndTime)
	{
		bTimeOut = ((CurTime >= StartTime) && (CurTime < EndTime))? FALSE : TRUE;
	}
	else
	{
		bTimeOut = ((CurTime >= StartTime) || (CurTime < EndTime))? FALSE : TRUE;
	}
	
	return bTimeOut;
}

U32 timer_getCur_cpuTick(void)
{
	return read_ccount();
}


void timer_get( TIME *time, int hiPrecise )
{
}

U32 timer_calUs( TIME *start, TIME *stop )
{
	return 0;
}


/**
 ****************************************************************************************
 * @brief initialize timer 2 use for os tick
 *
 ****************************************************************************************
 */
void hw_timer_2_init(U32 tick_per_sec)
{
	u16 val_16b;
	TIMER_CTRLs *pTimer_cntl = (TIMER_CTRLs *)&val_16b;
	TIMER_IDEL_LEV_INT_MASKs *pTimer_int_mask = (TIMER_IDEL_LEV_INT_MASKs *)&val_16b;

	ADDR_TIMER_2_VAL = (TIMERs_BASE_CLK_HZ / tick_per_sec + 1);

	val_16b = ADDR_TIMER_IDLE_LEV;
	pTimer_int_mask->timer_2_int_mask = 1;
	ADDR_TIMER_IDLE_LEV = val_16b;

	val_16b = ADDR_TIMER_CTRL;
	pTimer_cntl->timer_2_en = 1;
	ADDR_TIMER_CTRL = val_16b;

	xt_ints_on(LEVEL1_INT21_MASK);
}

/**
 ****************************************************************************************
 * @brief initialize timer
 *
 ****************************************************************************************
 */
void hw_timer_init(void)
{
	U32 val;
	MISC_CLKGATE_CONTROLs *pGate;

	val = REG_MISC_CLKGATE;
	pGate = (MISC_CLKGATE_CONTROLs *) &val;
	pGate->u_timer_gate = 1;						//enable timer main colck
	REG_MISC_CLKGATE = val;	

	ADDR_TIMER_PRESCALE = TIMER_CLK_DIVISOR-1;
}
