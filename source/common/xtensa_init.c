#include "os_type.h"



/*******************************************************************************
 Copyright (c) 2006-2007 by Tensilica Inc.  ALL RIGHTS RESERVED.
 These coded instructions, statements, and computer programs are the
 copyrighted works and confidential proprietary information of Tensilica Inc.
 They may not be modified, copied, reproduced, distributed, or disclosed to
 third parties in any manner, medium, or form, in whole or in part, without
 the prior written consent of Tensilica Inc.
 --------------------------------------------------------------------------------

 XTENSA INITIALIZATION ROUTINES CODED IN C

 This header is a place to put miscellaneous Xtensa RTOS-generic initialization
 functions that are implemented in C.

 This header contains definitions and macros for use primarily by Xtensa
 RTOS assembly coded source files. It includes and uses the Xtensa hardware
 abstraction layer (HAL) to deal with config specifics. It may also be
 included in C source files.

 *******************************************************************************/

#ifndef XTENSA_INIT_H
#define XTENSA_INIT_H

#include "os_config.h"
#include "interrupt.h"
#include "hw_cpu_clock.h"
#define XT_CLOCK_FREQ     SYS_CLK_FREQ_DEFAULT

unsigned int _xt_tick_divisor = 0;  /* cached number of cycles per tick */

#ifdef TIMER1_ENABLE
unsigned int _xt_tick1_divisor = 0;
#endif


/*
 Compute and initialize at run-time the tick divisor (the number of
 processor clock cycles in an RTOS tick, used to set the tick timer).
 Called when the processor clock frequency is not known at compile-time.
 */
void _xt_tick_divisor_init(void)
{
#ifdef TIMER0_ENABLE

#ifdef XT_BOARD
	_xt_tick_divisor = xtbsp_clock_freq_hz() / XT_TICK_PER_SEC;
#else
	_xt_tick_divisor = XT_CLOCK_FREQ / TIMER0_INT_FREQ;
#endif

#ifndef RTOS_ENABLE
	//if timer0 is initialized by OS,don't nedd the code below.
	xtensa_timer0_init();

#if defined THREAD_RTOS
	enable_ints( LEVEL1_INT6_MASK );
#elif defined FREE_RTOS
	xt_ints_on(LEVEL1_INT6_MASK);
#endif

#endif //RTOS_ENABLE

#endif //TIMER0_ENABLE


#ifdef TIMER1_ENABLE

/*
 *	Fcpu:24 mHz, 20 ms/interrupt
 *	_xt_tick1_divisor = (24 * 1000000 * 20)/1000;
 *
 *	Fcpu:24 mHz, 10 s/interrupt
 *	_xt_tick1_divisor = (24 * 1000000 * 10);
 *
 *	Fcpu:24 mHz, 20 s/interrupt
 *	_xt_tick1_divisor = (24 * 1000000 * 20);
 *
 *	Fcpu:24 mHz, 30 s/interrupt
 *	_xt_tick1_divisor = (24 * 1000000 * 30);
 *
 *	Fcpu:32 kHz, 10 s/interrupt
 *	_xt_tick1_divisor = (32 * 1000 * 10);
 *
 *	Fcpu:32 kHz, 20 s/interrupt
 *	_xt_tick1_divisor = (32 * 1000 * 20);
 *
 */

#ifdef HDMI_CEC_BY_OPTEK
	_xt_tick1_divisor = XT_CLOCK_FREQ / 8000;
#else
	//_xt_tick1_divisor = XT_CLOCK_FREQ / 1; // 1s 
	_xt_tick1_divisor = XT_CLOCK_FREQ / 10;  //100ms
#endif

#if defined FREE_RTOS
	xtensa_timer1_init();
	xt_ints_on(LEVEL3_INT10_MASK);
#endif

#endif //TIMER1_ENABLE
}

#endif // XTENSA_INIT_H