#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hw_gpio.h"
#include "hal_gpio.h"

#include "flash_boot.h"
#include "debug.h"


//#pragma section debug_code ".text_debug"
//#pragma use_section debug_code debug_assert


void debug_assert (BOOL condition) __INTERNAL_RAM_TEXT;

void debug_assert (BOOL condition)
{
	if (condition == TRUE)
	{
		return;
	}
	else
	{
		//system fatal error
		#ifdef JTAG_DEBUG_MODE
		
		#ifdef DEBUG_MODE
		asm("break 1,1");
		#endif
		
		#else //JTAG_DEBUG_MODE
		
		#ifdef DEBUG_MODE
		DBG_iPuts("debug assert !!!\r\n");
		#endif
		
		#endif //JTAG_DEBUG_MODE
	}
}

void debug_assert_info (BOOL condition, int param1, int param2)
{
	//U8 continous = TRUE;

	if (condition != FALSE)
	{
		//asm("nop");
		return;
	}
	else
	{
		//system fatal error
		#ifdef JTAG_DEBUG_MODE
		
		#ifdef DEBUG_MODE
		asm("break 1,1");
		#endif
		
		#else //JTAG_DEBUG_MODE
		
		#ifdef DEBUG_MODE
		DBG_iPuts("debug assert !!!\r\n");
		#endif
		
		#endif //JTAG_DEBUG_MODE
	}
}

#ifdef DEBUG_GPIO_ENABLE

void debug_pin_init(void) __FLASH_BOOT_TEXT;
void debug_pin_init(void)
{
	#ifdef DEBUG_GPIO_PIN0_ENABLE
	hal_gpio_output_level_set_for_slow(DEBEG_PIN, GPIO_LOW);
	hal_gpio_mux_func_select(DEBEG_PIN, MUX_SEL_GPIO_OUTPUT);
	#endif
	
	#ifdef DEBUG_GPIO_PIN1_ENABLE
	hal_gpio_output_level_set_for_slow(DEBEG_PIN1, GPIO_LOW);
	hal_gpio_mux_func_select(DEBEG_PIN1, MUX_SEL_GPIO_OUTPUT);
	#endif

	#ifdef DEBUG_GPIO_PIN2_ENABLE
	hal_gpio_output_level_set_for_slow(DEBEG_PIN2, GPIO_LOW);
	hal_gpio_mux_func_select(DEBEG_PIN2, MUX_SEL_GPIO_OUTPUT);
	#endif
}

void debug_pin_test(void)
{
	static U8 temp;
	while (1)
	{
		if (temp)
		{
			temp = 0;
			DBG_PIN_HIGH(DEBEG_PIN);
			delayms(1);
		}
		else
		{
			temp = 1;
			DBG_PIN_LOW(DEBEG_PIN);
			delayms(1);
		}
	}
}
#endif


#ifdef DEBUG_GPIO_PIN0_ENABLE
void debug_pin_loop(void) __FLASH_BOOT_TEXT;
void debug_pin_loop(void)
{
	while (1)
	{
		DBG_PIN_HIGH(DEBEG_PIN);
		delay_loop(1); //about 1.36us, run at 48MHz
		DBG_PIN_LOW(DEBEG_PIN);
		delay_loop(1); //about 1.36us, run at 48MHz
	}
}
#endif

#ifdef DEBUG_GPIO_PIN1_ENABLE
void debug_pin1_loop(void) __FLASH_BOOT_TEXT;
void debug_pin1_loop(void)
{
	while (1)
	{
		DBG_PIN1_HIGH(DEBEG_PIN);
		delay_loop(1); //about 1.36us, run at 48MHz
		DBG_PIN1_LOW(DEBEG_PIN);
		delay_loop(1); //about 1.36us, run at 48MHz
	}
}
#endif

#ifdef DEBUG_GPIO_PIN2_ENABLE
void debug_pin2_loop(void) __FLASH_BOOT_TEXT;
void debug_pin2_loop(void)
{
	while (1)
	{
		DBG_PIN2_HIGH(DEBEG_PIN);
		delay_loop(1); //about 1.36us, run at 48MHz
		DBG_PIN2_LOW(DEBEG_PIN);
		delay_loop(1); //about 1.36us, run at 48MHz
	}
}
#endif

void delay_loop(U32 count) __FLASH_BOOT_TEXT;
void delay_loop(U32 count)
{
	while (count--)
	{
		asm("nop");
	}
}
