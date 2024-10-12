#include "os_config.h"


void vApplicationStackOverflowHook( void *xTask, char *pcTaskName )
{
	static U8 stack_overflow_flag = 0;
	int i = 0;

	/* Look at pxCurrentTCB to see which task overflowed its stack. */

#ifdef XT_SIMULATOR
	exit(0);
#else
	for( ;; )
    {
		i++;
		asm("nop");
	
	#ifdef DEBUG_MODE
		asm("nop");
		asm("break 1,1");
	#else
		if (stack_overflow_flag == 0)
		{
			stack_overflow_flag = 1;
			printf_fatal("stack overflow:%s\n\r", pcTaskName);
		}

		if (i >= 1000000*100)
		{
			asm("nop");
		}
	#endif
    }
#endif
}


void vApplicationTickHook( void ) __attribute__ ((section (".internal_ram_1_text")));
void vApplicationTickHook( void )
{

}



#ifdef FREE_RTOS_DEBUG

void freeRTOS_xTaskIncrementTick_callback(void)
{
	DBG_Assert(FALSE);
	DBG_Printf("%s\n\r", __func__);
}

unsigned int freeRTOSRunTimeTick;
unsigned int over_cnt;
unsigned int last_tick;

/*
 * SHIFT_BITS
 * 9  ->  48MHz
 * 13 -> 800MHz
 */
#define SHIFT_BITS    9//for 48mhz



void freeRTOS_timer_run_time_tick_status(void)
{
	last_tick = xthal_get_ccount();
	freeRTOSRunTimeTick = last_tick >> SHIFT_BITS;
	over_cnt = 0;
}


unsigned int freeRTOS_get_run_time_counter_value(void)
{
	uint32_t tmp;
	uint32_t diff_tick;

	tmp = xthal_get_ccount();

	if (tmp < last_tick)
		over_cnt++;

	last_tick = tmp;

	diff_tick = over_cnt*(0x100000000>>SHIFT_BITS) + (last_tick>>SHIFT_BITS);

	return diff_tick;
}
#endif
