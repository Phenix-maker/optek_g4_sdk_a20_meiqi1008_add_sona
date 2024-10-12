/*
 * hw_watchdog.h
 *
 *  Created on: 2024/1/29
 *      Author: Windows
 */

#ifndef __HW_WATCHDOG_H__
#define __HW_WATCHDOG_H__

//0.25s/step
#define WDT_TIME_TO_S(x)    (x*4)

void watchdog_time_reset(void);
void hw_wdt_enable(U8 step);
void hw_wdt_disable(void);

#endif /* __HW_WATCHDOG_H__ */
