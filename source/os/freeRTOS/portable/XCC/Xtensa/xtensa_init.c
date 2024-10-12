#include "os_type.h"


#ifdef FREE_RTOS_LIB_BUILD
#ifdef FREE_RTOS
#undef FREE_RTOS
#endif
#endif


#if ((defined FREE_RTOS) && (defined XTENSA_INIT_ENABLE))
/*******************************************************************************
// Copyright (c) 2003-2015 Cadence Design Systems, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--------------------------------------------------------------------------------

        XTENSA INITIALIZATION ROUTINES CODED IN C

This file contains miscellaneous Xtensa RTOS-generic initialization functions
that are implemented in C.

*******************************************************************************/

#ifndef XTENSA_INIT_H
#define XTENSA_INIT_H

#ifdef XT_BOARD
#include <xtensa/xtbsp.h>
#endif

#include "xtensa_rtos.h"

#include "os_config.h"

#include "interrupt.h"





#ifdef TIMER0_ENABLE
unsigned int _xt_tick_divisor = 0;  /* cached number of cycles per tick */
#endif

#ifdef TIMER1_ENABLE
unsigned int _xt_tick1_divisor = 0;
#endif

#ifdef TIMER2_ENABLE
unsigned int _xt_tick2_divisor = 0;
#endif

#ifndef RTOS_ENABLE
volatile void * volatile pxCurrentTCB = NULL;
#endif


/*
 * Compute and initialize at run-time the tick divisor (the number of 
 * processor clock cycles in an RTOS tick, used to set the tick timer).
 * Called when the processor clock frequency is not known at compile-time.
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
	_xt_tick1_divisor = XT_CLOCK_FREQ / TIMER1_INT_FREQ;
#endif

#if defined FREE_RTOS
	xtensa_timer1_init();
	xt_ints_on(LEVEL3_INT10_MASK);
#endif

#endif //TIMER1_ENABLE
}


#endif // XTENSA_INIT_H


#endif //FREE_RTOS
