/*
 * rotary_used_gpio.c
 *
 *  Created on: 2024/4/13
 *      Author: Windows
 */

#include "os_config.h"


#ifdef ROTARY_ENABLE

#ifdef ROTARY_DET_USED_GPIO


#include "regmap.h"

#include "hw_gpio.h"

#include "rotary_used_gpio.h"

#define ROTARY_FUNC_ENABLE
#define DBG_Printf


#define ROTARY_A1_PIN          GPIO0_19_INDEX
#define ROTARY_B1_PIN          GPIO0_18_INDEX

#define ROTARY_A1_DET          hal_gpio_input_level_read(ROTARY_A1_PIN)
#define ROTARY_B1_DET          hal_gpio_input_level_read(ROTARY_B1_PIN)



#ifdef ROTARY_NUM_2
#define ROTARY_A2_PIN          GPIO0_03_INDEX
#define ROTARY_B2_PIN          GPIO0_17_INDEX

#define ROTARY_A2_DET          hal_gpio_input_level_read(ROTARY_A2_PIN)
#define ROTARY_B2_DET          hal_gpio_input_level_read(ROTARY_B2_PIN)
#endif


U8 prevRotary_A1_State;
U8 prevRotary_B1_State;


#ifdef ROTARY_NUM_2
U8 prevRotary_A2_State;
U8 prevRotary_B2_State;
#endif


#define ROTARY_1_COUNT    10

#ifdef ROTARY_NUM_2
#define ROTARY_2_COUNT    10
#endif


void rotary_used_gpio_init(void)
{
	hal_gpio_mux_func_select(ROTARY_A1_PIN,MUX_SEL_GPIO_INPUT);
	hal_gpio_mux_func_select(ROTARY_B1_PIN,MUX_SEL_GPIO_INPUT);

	prevRotary_A1_State = 0;
	prevRotary_B1_State = 0;

#ifdef ROTARY_NUM_2
	app_gpio_MUXfunction_select(ROTARY_A2_PIN, MUX_SEL_GPIO_INPUT);
	app_gpio_MUXfunction_select(ROTARY_B2_PIN, MUX_SEL_GPIO_INPUT);

	prevRotary_A2_State = 0;
	prevRotary_B2_State	= 0;
#endif
}


static const BYTE Rotary_Table[] = {
/* 	0 0 0 0         0 0 0 1         0 0 1 0         0 0 1 1         0 1 0 0         0 1 0 1         0 1 1 0         0 1 1 1 */
	ROTARY_UNMOVED, ROTARY_REVERSE, ROTARY_FORWARD, ROTARY_UNMOVED, ROTARY_FORWARD, ROTARY_UNMOVED, ROTARY_UNMOVED, ROTARY_REVERSE,
/*	1 0 0 0         1 0 0 1         1 0 1 0         1 0 1 1         1 1 0 0         1 1 0 1         1 1 1 0         1 1 1 1 */
   	ROTARY_REVERSE, ROTARY_UNMOVED, ROTARY_UNMOVED, ROTARY_FORWARD, ROTARY_UNMOVED, ROTARY_FORWARD, ROTARY_REVERSE, ROTARY_UNMOVED
};


static U8 rotary_detection(U8 type)//every 2ms
{
    DBG_Printf("%s:%d\n\r", __func__, type);
    #ifdef ROTARY_FUNC_ENABLE

	volatile U8 KeyTabData;
	volatile U8 KeyTabBassData;
	volatile U8 KeyTabTreData;

	KeyTabData = 0;

	if ( prevRotary_A1_State )
	{
		KeyTabData |= 0x08;
	}

	if ( prevRotary_B1_State )
	{
		KeyTabData |= 0x04;
	}

	#ifdef ROTARY_NUM_2
	if ( prevRotary_A2_State )
	{
		KeyTabBassData |= 0x08;
	}

	if ( prevRotary_B2_State )
	{
		KeyTabBassData |= 0x04;
	}
	#endif

	switch(type)
	{
	case ROTARY_1:
		if ( ROTARY_A1_DET )
		{
			prevRotary_A1_State = 1;
			KeyTabData |= 0x02;
		}
		else
		{
			prevRotary_A1_State = 0;
		}

		if ( ROTARY_B1_DET )
		{
			prevRotary_B1_State = 1;
			KeyTabData |= 0x01;
		}
		else
		{
			prevRotary_B1_State = 0;
		}

		KeyTabData = Rotary_Table[KeyTabData];
		break;

	#ifdef ROTARY_NUM_2
	case ROTARY_2:
		if ( ROTARY_A2_DET )
		{
			prevRotary_A2_State = 1;
			KeyTabData |= 0x02;
		}
		else
		{
			prevRotary_A2_State = 0;
		}

		if ( ROTARY_B2_DET )
		{
			prevRotary_B2_State = 1;
			KeyTabData |= 0x01;
		}
		else
		{
			prevRotary_B2_State = 0;
		}

		KeyTabData = Rotary_Table[KeyTabData];
		break;
	#endif

	default:
		break;
	}

	return KeyTabData;
    #else
    return 0;
    #endif
}

I8 rotary_type;

void rotary_used_gpio_process(U32 ticks)
{
    DBG_Printf("%s:%d\n\r", __func__, ticks);
    #ifdef ROTARY_FUNC_ENABLE
	U8 rotary_dir = 0;
	static U8 Rotary_1_DetNum = 0;
	#ifdef ROTARY_NUM_2
	static U8 Rotary_2_DetNum = 0;
	#endif

	rotary_dir = rotary_detection(ROTARY_1);

	if ( rotary_dir != ROTARY_UNMOVED )
	{
		Rotary_1_DetNum++;
		if(Rotary_1_DetNum > ROTARY_1_COUNT)
		{
			Rotary_1_DetNum = 0;
			rotary_type = rotary_dir + ROTART_1_STEP;
		}
	}
	else
	{
		#ifdef ROTARY_NUM_2
		rotary_dir	= rotary_detection(ROTARY_2);
		if ( rotary_dir != ROTARY_UNMOVED )
		{
			Rotary_2_DetNum++;
			if(Rotary_2_DetNum > ROTARY_2_COUNT)
			{
				Rotary_2_DetNum = 0;
				rotary_type = rotary_dir + ROTART_2_STEP;
			}
		}
		#endif
	}

	if ( (ticks % 2) == 0)
	{
		#ifdef ROTARY_DET_USED_GPIO
		{
			U8 tmp;
			U8 rotary_dir;
			
			tmp = rotary_type;
			rotary_type = 0;
			rotary_dir = (tmp & 0x0F);

			if (rotary_dir != ROTARY_UNMOVED)
			{
				DBG_Printf("rotary_dir = %d\n\r",rotary_dir);
				#if 1
				func_w_one_param_msgSend( (U32) app_nav_rotary_key_handler, tmp);
				#else
				timer_rotary_gpio_timer_set (rotary_dir);
				#endif
			}
		}
		#endif
	}
    #endif
}

void app_nav_rotary_key_handler(U32 data)
{
	DBG_Printf("%s:%d\n\r", __func__, data);
    #ifdef ROTARY_FUNC_ENABLE   
	if ( data == (ROTARY_FORWARD + ROTART_1_STEP) )
	{
		printf_info("Rotary1 + 0x%x\n\r",data);
	}
	else if ( data == (ROTARY_REVERSE + ROTART_1_STEP) )
	{
		printf_info("Rotary1 - 0x%x\n\r",data);
	}

    #ifdef ROTARY_TEST
	if ( data == (ROTARY_FORWARD + ROTART_2_STEP) )
	{
		DBG_Puts("Rotary2 +\n\r");
	}
	else if ( data == (ROTARY_REVERSE + ROTART_2_STEP) )
	{
		DBG_Puts("Rotary2 -\n\r");
	}
    #endif
    #endif
}
#endif //ROTARY DETECT GPIO

#ifdef ROTARY_0_ENABLE
U8 rotary0_KeyScan_act(U16 dir) __attribute__ ((section (".interal_ram_1_text")));

U8 rotary0_KeyScan_act(U16 dir)
{
	DBG_Printf("%s:%d\n\r", __func__, dir);
    #ifdef ROTARY_FUNC_ENABLE

	watchdog_time_reset();

	if (dir == ROTARY_LEFT)
	{
		ukMsgSend(app_nav_mp3_minus);
		return TRUE;
	}
	else if (dir == ROTARY_RIGHT)
	{
		ukMsgSend(app_nav_mp3_plus);
		return TRUE;
	}
    #endif
	return FALSE;
}
#endif //ROTARY_0_ENABLE

#ifdef ROTARY_1_ENABLE
U8 rotary1_KeyScan_act(U16 dir, I16 size) __attribute__ ((section (".interal_ram_1_text")));

U8 rotary1_KeyScan_act(U16 dir, I16 size)
{
	DBG_Printf("%s:%d %d\n\r", __func__, dir, size);
    #ifdef ROTARY_FUNC_ENABLE

	watchdog_time_reset();

	if (dir == ROTARY_LEFT)
	{

		return TRUE;
	}
	else if (dir == ROTARY_RIGHT)
	{

		return TRUE;
	}
    #endif
	return FALSE;

}
#endif //ROTARY_1_ENABLE
#endif //ROTARY_ENABLE

