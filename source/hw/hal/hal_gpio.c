#include "os_config.h"

#include "regmap.h"

#include "hw_gpio.h"
#include "hal_gpio.h"

#include "flash_boot.h"


/**
 ****************************************************************************************
 *
 * @file app_gpio.c
 * @defgroup GPIO_profile_commons
 * @ingroup GPIO_profile
 * @brief GPIO profile commons implement.
 *
 ****************************************************************************************
 */


void hal_gpio_mux_func_select(enumGPIO_INDEX gpio_index, enumGPIOMUX_SEL mux_sel)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		//valid GPIO port
		U8 gpio_group = gpio_index>>5;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				gpio0_MUX_select(1<<gpio_index, mux_sel);
				break;
			default:
				DBG_Assert(FALSE);
				break;
		}
	}
}

void hal_gpio_output_level_set(enumGPIO_INDEX gpio_index, enumGPIO_LEVEL level) __attribute__ ((section (".internal_ram_1_text")));
void hal_gpio_output_level_set(enumGPIO_INDEX gpio_index, enumGPIO_LEVEL level)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				gpio0_SetOutput_level(1<<gpio_index, level);
				break;
			default:
				DBG_Assert(FALSE);
				break;
		}
	}
}

/* the function is running at sflash or psram*/
void hal_gpio_output_level_set_for_slow(enumGPIO_INDEX gpio_index, enumGPIO_LEVEL level)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				gpio0_SetOutput_level(1<<gpio_index, level);
				break;
			default:
				DBG_Assert(FALSE);
				break;
		}
	}
}

/********gpio_index->GPIO_INDEX   return GPIO_LEVEL (TRUE(HIGH) FALSE(LOW)) ************/
U32 hal_gpio_input_level_read(enumGPIO_INDEX gpio_index)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;
		U32 temp;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				temp = gpio0_input_level_read(1<<gpio_index);
				break;
			default:
				DBG_Assert(FALSE);
				temp = 0;
				break;
		}

		return temp;
	}
	
	return -1;
}

U32 hal_gpio_group_input_level_read(U8 gpio_group)
{
	if (gpio_group >= 0 && gpio_group < GPIO_GROUP_MAX)
	{
		U32 temp;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				temp = gpio0_group_input_level_read();
				break;
				
			#ifdef POWER_KEY_ENABLE
			case GPIO_GROUP_SREG:
				temp = gpio_sreg_group_input_level_read();
				break;
			#endif
			
			default:
				DBG_Assert(FALSE);
				temp = 0;
				break;
		}

		return temp;
	}
	return -1;
}

/********gpio_index->GPIO_INDEX   enable->int onpen close
Trigger_type->GPIO_INT_TRIGGER_TYPE  Trigger_level->GPIO_INT_TRIGGER_LEVEL************/
void hal_gpio_interrupt_set(enumGPIO_INDEX gpio_index, U8 enable, enumGPIO_INT_TRIGGER_TYPE Trigger_type, enumGPIO_INT_TRIGGER_LEVEL Trigger_level)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				gpio0_interrupt_set(1<<gpio_index, enable, Trigger_type, Trigger_level);
				break;
			default:
				DBG_Assert(FALSE);
				break;
		}
	}
}

/********gpio_index->GPIO_INDEX return gpio_index int status and clear************/
U32 hal_gpio_interrupt_st_read_use_index(enumGPIO_INDEX gpio_index)
{
	if ((gpio_index >= 0) && gpio_index < GPIO_MAX_INDEX)
	{
		U8 gpio_group = gpio_index>>5;
		U32 temp;

		switch (gpio_group)
		{
			case GPIO_GROUP0:
				temp = gpio0_interrupt_st_mask_read(1<<gpio_index);
				break;
			default:
				temp = 0;
				DBG_Assert(FALSE);
				break;
		}

		return temp;
	}
	return -1;
}

/********gpio_group->GPIO_GROUP   return gpio_group all int status and clear ************/
U32 hal_gpio_interrupt_st_read_use_group(enumGPIO_GROUP gpio_group)
{
	U32 temp;

	switch (gpio_group)
	{
		case GPIO_GROUP0:
			temp = gpio0_interrupt_st_group_read();
			break;
		default:
			temp = 0;
			DBG_Assert(FALSE);
			break;
	}

	return temp;
}

