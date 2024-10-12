/*
 * remote.c
 *
 *  Created on: 2019-9-24
 *      Author: 
 */

#include "c_def.h"
#include "debug.h"
#include "sdk_config.h"


#ifdef REMOTE_ENABLE
#include "regmap.h"

#include "mem_reloc.h"

#include "remote.h"


void remote_ir_recive(U32 ir_value) __INTERNAL_RAM_TEXT;


/*
 *Max: REMOTE_HIGH_LEVLE_WIDTH    0x1FF
 *Max: REMOTE_LOW_LEVLE_WIDTH     0x1FF
*/
#define REMOTE_HIGH_LEVLE_WIDTH    0x40 //about 2ms
//#define REMOTE_HIGH_LEVLE_WIDTH    0x140 //about 10ms
#define REMOTE_LOW_LEVLE_WIDTH     0x140 //about 10ms

#define HW_IR_RX_DEBUG
#define REMOTER_CALLBACK_ENABLE

#define IR_LEVEL_MASK    (0x80808080)
#define IR_TIME_MASK     (~IR_LEVEL_MASK)


void remote_ir_recive(U32 value)
{
#ifdef REMOTE_ENABLE
	int i;

	U32 ir_value = value;
	U32 ir_level;
	U32 ir_time;
	U8 *level = (U8 *)&ir_level;
	U8 *time = (U8 *)&ir_time;

	ir_level = value & IR_LEVEL_MASK;
	ir_time = value & IR_TIME_MASK;

	for (i=0; i<4; i++)
	{
		if ( (pulse_width_time != 0) && (*level != pre_pin_level) )
		{
			if ( (pulse_width_time > REMOTE_HIGH_LEVLE_WIDTH) && pre_pin_level )
			{
				//in the idle status
				asm("nop");
			}
			else
			{
				remoteHardISR(pre_pin_level, pulse_width_time);
			}
			
			pulse_width_time = 0;
		}

		pulse_width_time += *time++;
		
		pre_pin_level = *level++;
	}
#endif
}

void hw_ir_din_select(U8 sel)
{
	U32 val;
	val = REG_MISC_CPUCLK;
	MISC_CPUCLK_CONTROLs *pCpuClk = (MISC_CPUCLK_CONTROLs *) &val;

	if (sel == IR_IN_CH_0)
	{
		REMOTE_INT_CLEAR;
		hal_gpio_mux_func_select(IR_IN_CH_0_PIN, MUX_SEL_GPIO_INPUT);
		hal_gpio_interrupt_set(IR_IN_CH_0_PIN, TRUE, TRIGGER_EDGE, GPIO_INT_FALLING_EDGE);
		
		pCpuClk->ir_sel = 0;//use UART0 RX pin as IR pin
	}
	else if (sel == IR_IN_CH_1)
	{
		REMOTE_INT_CLEAR;
		hal_gpio_mux_func_select(IR_IN_CH_1_PIN, MUX_SEL_GPIO_INPUT);
		hal_gpio_interrupt_set(IR_IN_CH_1_PIN, TRUE, TRIGGER_EDGE, GPIO_INT_FALLING_EDGE);

		pCpuClk->ir_sel = 1;//use UART1 RX pin as IR pin
	}
	else if (sel == IR_IN_CH_2)
	{
		REMOTE_INT_CLEAR;
		hal_gpio_mux_func_select(IR_IN_CH_2_PIN, MUX_SEL_GPIO_INPUT);
		hal_gpio_interrupt_set(IR_IN_CH_2_PIN, TRUE, TRIGGER_EDGE, GPIO_INT_FALLING_EDGE);

		pCpuClk->ir_sel = 2;//use UART2 RX pin as IR pin
	}
	else
	{
		DBG_Assert(FALSE);
	}
	
	REG_MISC_CPUCLK = val;
}
#endif



