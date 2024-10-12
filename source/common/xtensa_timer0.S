#include "os_type.h"


#ifdef TIMER0_ENABLE

//#include "tx_port.h"
//#include "xtensa_rtos.h"
//#include "xtensa_timer.h"

#include <xtensa/config/core.h>

#include "xtensa_context.h"


#define XT_TIMER_INDEX         0
//#define XT_TIMER_INDEX         1


#define XT_CCOMPARE             (CCOMPARE + XT_TIMER_INDEX)
#define XT_TIMER_INTNUM         XCHAL_TIMER_INTERRUPT(XT_TIMER_INDEX)
#define XT_TIMER_INTPRI         XCHAL_INT_LEVEL(XT_TIMER_INTNUM)
#define XT_TIMER_INTEN          (1 << XT_TIMER_INTNUM)


	.text
//	.align	4
/*	.literal_position*/
/*	.section  ._xtensa_timer1_init, "ax"*/
/*	.section  .internal_ram_1_text, "ax"*/

	.align	4
	.global	  xtensa_timer0_init
	.type	  xtensa_timer0_init, @function

	.extern _xt_tick_divisor

xtensa_timer0_init:

	//ENTRY(16)
	entry sp, 16


	/* Set up the periodic tick timer (assume enough time to complete init). */
	movi    a2, _xt_tick_divisor
	l32i    a3, a2, 0

	rsr     a2, CCOUNT              /* current cycle count */
	add     a2, a2, a3              /* time of first timer interrupt */
	wsr     a2, XT_CCOMPARE         /* set the comparator */


	//RET(16)
	retw


	.text
/*	.align	4*/
/*	.literal_position*/
/*	.section  ._xtensa_timer1_init, "ax"*/
	.section  .internal_ram_1_text, "ax"

	.align	4
	.global	  xtensa_timer0_updata
	.type	  xtensa_timer0_updata, @function

xtensa_timer0_updata:

	ENTRY(16)
	//entry sp, 16

	/* Update the timer comparator for the next tick. */
	movi    a3, _xt_tick_divisor
	l32i    a2, a3, 0                   /* a2 = comparator increment */
//	rsr     a3, CCOMPARE1               /* a3 = old comparator value */
	rsr     a3, CCOUNT                  /* current cycle count */

	add     a4, a3, a2                  /* a4 = new comparator value */
/*	wsr     a4, CCOMPARE1*/               /* update comp. and clear interrupt */
	wsr     a4, XT_CCOMPARE
	esync

	//RET(16)
	retw


#endif //TIMER0_ENABLE
