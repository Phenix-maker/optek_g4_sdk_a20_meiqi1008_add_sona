/*
 * rotary_used_gpio.h
 *
 *  Created on: 2024/4/13
 *      Author: Windows
 */

#ifndef __ROTARY_USED_GPIO_H__
#define __ROTARY_USED_GPIO_H__


enum {
	VOL_STEADY,
	VOL_INCREASING,
	VOL_DECREASING
};

enum {
	ROTARY_1,
	ROTARY_2,
	ROTARY_3,
};


#define ROTARY_UNMOVED    0
#define ROTARY_FORWARD    1
#define ROTARY_REVERSE    2

#define ROTART_1_STEP    0x00
#define ROTART_2_STEP    0x10
#define ROTART_3_STEP    0x20


extern I8 rotary_type;


void rotary_used_gpio_init(void);
void rotary_used_gpio_process(U32 ticks);
void app_nav_rotary_key_handler(U32 data);


#endif /* __ROTARY_USED_GPIO_H__ */
