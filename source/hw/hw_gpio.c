#include "os_config.h"

#include "regmap.h"

#include "hw_gpio.h"
#include "hal_gpio.h"

#include "flash_boot.h"


#undef TX_DISABLE
#undef TX_RESTORE
#define TX_DISABLE
#define TX_RESTORE


#undef __inline__
#define __inline__

__inline__ void gpio0_MUX_select(U32 mask, U8 select)
{
	TX_INTERRUPT_SAVE_AREA;
	
	TX_DISABLE;

	switch(select)
	{
		case MUX_SEL_GPIO_INPUT:
			REG_GPIO0_FUNC &= ~mask;
	    	REG_GPIO3_FUNC &= ~mask;
			REG_GPIO4_FUNC &= ~mask;
						
			REG_GPIO0_DOE &= ~mask;
			REG_GPIO0_IE  |=  mask;
			break;
			
		case MUX_SEL_GPIO_OUTPUT:
			REG_GPIO0_FUNC &= ~mask;
	    	REG_GPIO3_FUNC &= ~mask;
			REG_GPIO4_FUNC &= ~mask;

		    REG_GPIO0_IE  &= ~mask;
			REG_GPIO0_DOE |=  mask;
			break;
			
		case MUX_SEL_FUNCTION1:
			REG_GPIO3_FUNC &= ~mask;
			REG_GPIO4_FUNC &= ~mask;
			REG_GPIO0_FUNC |= mask;
			break;
			
		case MUX_SEL_FUNCTION2:
		case MUX_SEL_FUNCTION3:
			REG_GPIO0_FUNC &= ~mask;
			REG_GPIO4_FUNC &= ~mask;
			REG_GPIO3_FUNC |= mask;
			break;
		case MUX_SEL_FUNCTION4:
			REG_GPIO0_FUNC &= ~mask;
			REG_GPIO3_FUNC &= ~mask;
			REG_GPIO4_FUNC |= mask;
			break;						
			
		default:
			DBG_Assert(FALSE);
			break;
	}

	TX_RESTORE;
}

__inline__ void gpio0_SetOutput_level(U32 mask,U8 level)
{
	TX_INTERRUPT_SAVE_AREA;
	
	TX_DISABLE;
	
	if (level == GPIO_LOW)
	{
		REG_GPIO0_DOUT &= ~mask;
	}
	else
	{
		REG_GPIO0_DOUT |= mask;
	}
	
	TX_RESTORE;
}

__inline__ U32 gpio0_input_level_read(U32 mask)
{
	U32 tmp;
	tmp = (!(!(REG_GPIO0_DIN & mask)));
	return tmp;
}

__inline__ U32 gpio0_group_input_level_read(void)
{
	return REG_GPIO0_DIN;
}

__inline__ U32 gpio_sreg_group_input_level_read(void)
{
	return REG_GPIO_SREG & 0x01;
}

void gpio0_interrupt_set(U32 mask, U8 enable, U8 Trigger_type, U8 Trigger_level)
{
	TX_INTERRUPT_SAVE_AREA;
	
	TX_DISABLE;
	
	if (enable)
	{
		if (Trigger_type == TRIGGER_LEVEL)
		{
			//trigger by levle
			REG_GPIO0_INTTP0 |= mask;
		}
		else
		{
			//trigger by edge
			REG_GPIO0_INTTP0 &= ~mask;
		}
		
		if (Trigger_level == GPIO_INT_RISING_EDGE)
		{
			//rising or high
			REG_GPIO0_INTTP1 |= mask;
		}
		else
		{
			//falling or low
			REG_GPIO0_INTTP1 &= ~mask;
		}
	}
	else
	{
		REG_GPIO0_INTEN &= ~mask;
	}
	
	TX_RESTORE;
}

__inline__ U32 gpio0_interrupt_st_group_read(void)
{
	U32 st;
	TX_INTERRUPT_SAVE_AREA;
	
	TX_DISABLE;
	
	st = REG_GPIO0_INTST;
	REG_GPIO0_INTST = st;        //clear int

	TX_RESTORE;
	
	return st;
}

__inline__ U32 gpio0_interrupt_st_mask_read(U32 mask)
{
	U32 st;
	TX_INTERRUPT_SAVE_AREA;
	
	TX_DISABLE;

	st = (REG_GPIO0_INTST & mask);
	REG_GPIO0_INTST = st;        //clear int
	st = !(!st);

	TX_RESTORE;
	
	return st;
}
