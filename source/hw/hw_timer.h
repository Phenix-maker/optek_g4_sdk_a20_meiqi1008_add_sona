#ifndef _HW_TIMER_H
#define _HW_TIMER_H


#include <xtensa/config/core.h>
#include <xtensa/config/specreg.h>

#include "hw_cpu_clock.h"


/*
 * 	timer related functions
 */


/*
*#define _XTSTR(x) # x
*#define XTSTR(x) _XTSTR(x)
*/


void timer_delayus( U32 xus );
void timer_delayms( U32 xms );

#define delayus(xus) timer_delayus(xus)
#define delayms(xms) timer_delayms(xms)

typedef	struct {
	int sysTick;
	U32	sysCnt;
	U32	sysCmp;
}	TIME;

void timer_get( TIME *time, int hiPrecise );

U32 timer_calUs( TIME *start, TIME *stop );


static __inline__ unsigned int read_ccount()
{
    unsigned int ccount;

    __asm__ __volatile__ (
        "rsr     %0, "XTSTR(CCOUNT)
        : "=a" (ccount) 
        );
    return ccount;
}

static __inline__ unsigned int read_ccompare0()
{
    unsigned int ccompare0;

    __asm__ __volatile__ (
        "rsr     %0, "XTSTR(CCOMPARE_0)
        : "=a" (ccompare0)
        );
    return ccompare0;
}

static __inline__ int read_ccompare1()
{
    unsigned int ccompare1;

    __asm__ __volatile__ (
        "rsr     %0, "XTSTR(CCOMPARE_1)
        : "=a" (ccompare1)
        );
    return ccompare1;
}

static __inline__ unsigned int read_intenable()
{
    unsigned int intenable;

    __asm__ __volatile__ (
        "rsr     %0, "XTSTR(INTENABLE)
        : "=a" (intenable)
        );
    return intenable;
}

static __inline__ void set_ccompare1(int val)
{
    __asm__ __volatile__ (
        "wsr     %0, "XTSTR(CCOMPARE_1)"\n\t"
        "isync\n\t"
        :
        : "a" (val)
        );
}

static __inline__ void set_ccompare2(int val)
{
    __asm__ __volatile__ (
        "wsr     %0, "XTSTR(CCOMPARE_2)"\n\t"
        "isync\n\t"
        :
        : "a" (val)
        );
}

static __inline__ void set_ccompare0(int val)
{
    __asm__ __volatile__ (
        "wsr     %0, "XTSTR(CCOMPARE_0)"\n\t"
        "isync\n\t"
        :
        : "a" (val)
        );
}


//must less than PERI_CLK_HZ and can Aliquot PERI_CLK_HZ. Note that this affects timers and os tick .
#define TIMERs_BASE_CLK_HZ (4*1000*1000)
#define TIMER_CLK_DIVISOR    (PERI_CLK_HZ / TIMERs_BASE_CLK_HZ)

#define TIMERs_BASE_Hus_TICK (TIMERs_BASE_CLK_HZ/(2*1000*1000))

U8 timer_checkTimeout(U32 StartTime, U32 xus);
U32 timer_getCurtime(void);

void hw_timer_2_init(U32 tick_per_sec);
void hw_timer_init(void);

#endif //_HW_TIMER_H
