#include "os_config.h"

#include "hw_saradc.h"

#include "hal_key_adc.h"
#include "hal_gpio.h"

#include "sdk_com.h"
#include "app_entry.h"
#include "app_window.h"
#include "app_task.h"
#include "app_key.h"
#include "key_scan.h"

#ifdef HDMI_ENABLE
#include "hdmi_process.h"
#endif

#include "ITEMT_Sys.h"

#ifdef ROTARY_ENABLE
#include "rotary_used_gpio.h"
#endif

#define DBG_KEYPrintf printf_info

/*
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */

#define KEY_FILTER_FOCTOR      3// pressed KEY_FILTER_FOCTOR*5ms

#define MIX_KEY_REALESE_FILTER_FOCTOR    60 //MIX_KEY_REALESE_FILTER_FOCTOR*5 ms

/*
 * STRUCT DEFINITION
 ****************************************************************************************
 */
struct {
	APP_KEY gpiokey[KEY_GPIO_NUMBER_MAX];
	APP_KEY adkey[KEY_ADC_NUMBER_MAX];

	struct {
		APP_KEY key_id;
		U32 filter_cnt;
	} key_filter, mix_key_release_filter;

	APP_KEY key_pressed;
	U16 num_of_key_pressed;
} keyScan_env;

//#define ADC_KEY_DEBUG

void KeyScan_init(void)
{
	memset(keyScan_env.gpiokey, APP_KEY_INVALID, sizeof(keyScan_env.gpiokey));
	memset(keyScan_env.adkey, APP_KEY_INVALID, sizeof(keyScan_env.adkey));
	keyScan_env.key_filter.key_id = APP_KEY_INVALID;
	keyScan_env.key_filter.filter_cnt = 0;
	keyScan_env.key_pressed = APP_KEY_INVALID;

	keyScan_env.mix_key_release_filter.key_id = APP_KEY_INVALID;
	keyScan_env.mix_key_release_filter.filter_cnt = 0;
	
	#ifdef KEY_GPIO_ENABLE
	app_gpio_key_init();
	#endif

	#ifdef KEY_ADC_ENABLE
	hal_key_adc_init();
	#endif

}

void KeyScan_open (void)
{
	/* incompelet function */
}

void KeyScan_close (void)
{
	/* incompelet function */
}

void KeyScan_fini (void)
{
	/* incompelet function */
}



U8 KeyScan_MainBorad(U16 *kdata)	
{
    static APP_KEY last_debounce = APP_KEY_INVALID;
	static APP_KEY last_valid = APP_KEY_INVALID;
    APP_KEY key = APP_KEY_INVALID;

    return 0;
}

void KeyScan(U32 ticks)
{
	U8 num_of_key_pressing = 0;
	APP_KEY gpio_key[MAX_SIMULT_KEY] = {0};
	APP_KEY key_id = APP_KEY_INVALID;

	#ifdef KEY_GPIO_ENABLE	
	U32 gpios_status = hal_gpio_group_input_level_read(GPIO_GROUP0);
	if (MAX_SIMULT_KEY - num_of_key_pressing)
	{
		num_of_key_pressing = vGpioMapMainKey(GPIO_GROUP0, gpios_status, gpio_key, MAX_SIMULT_KEY);
	}
	#endif

	#ifdef POWER_KEY_ENABLE	
	gpios_status = hal_gpio_group_input_level_read(GPIO_GROUP_SREG);
	if (MAX_SIMULT_KEY - num_of_key_pressing)
	{
		num_of_key_pressing += vGpioMapMainSregKey(GPIO_GROUP_SREG, gpios_status, &gpio_key[num_of_key_pressing], MAX_SIMULT_KEY);
	}
	#endif	

	#ifdef KEY_ADC_ENABLE
	if (MAX_SIMULT_KEY - num_of_key_pressing)
	{
		num_of_key_pressing += hal_key_adc_detect(&gpio_key[num_of_key_pressing], MAX_SIMULT_KEY - num_of_key_pressing);
	}
	#endif

	/* others key detect in here */
	if (num_of_key_pressing == 0)
	{
		/* do nothing */
	}
	else if (num_of_key_pressing == 1)
	{
		key_id = gpio_key[0];
	}
	else if (num_of_key_pressing <= MAX_SIMULT_KEY)
	{
		/* key mixing */
		key_id = vMainKeyMix(gpio_key, num_of_key_pressing);

		if (key_id != APP_KEY_INVALID)
		{
			/* mixing key detected */
		}
		else
		{
			/* this mixing key is not defined */
		}
	}
	else
	{
		/* too many key being pressed at once */
	}
	
	/* key filter */
	if (keyScan_env.key_filter.key_id != key_id)
	{
		if (keyScan_env.key_filter.filter_cnt < KEY_FILTER_FOCTOR)
		{
			DBG_KEYPrintf("filter a key %d\r\n", keyScan_env.key_filter.key_id);
		}

		keyScan_env.key_filter.key_id = key_id;
		keyScan_env.key_filter.filter_cnt = 0;
	}
	
	if (keyScan_env.key_filter.filter_cnt < KEY_FILTER_FOCTOR)
	{
		if (++keyScan_env.key_filter.filter_cnt == KEY_FILTER_FOCTOR)
		{
			//key status change
			DBG_KEYPrintf("2  filter a key %d\r\n", keyScan_env.key_filter.key_id);
		}
	}
	
	if (keyScan_env.key_filter.filter_cnt == KEY_FILTER_FOCTOR)
	{
		if (key_id == APP_KEY_INVALID)
		{
			if (keyScan_env.key_pressed != APP_KEY_INVALID)
			{
				key_msgSend(MAIN_BOARD_KEY, KEY_UP, keyScan_env.key_pressed);

				if (keyScan_env.mix_key_release_filter.key_id != keyScan_env.key_pressed)
				{
					DBG_KEYPrintf("- filter a mix key %d to release\r\n", keyScan_env.key_pressed);
				}

				DBG_KEYPrintf ("key %d up\n\r", keyScan_env.key_pressed);
				keyScan_env.key_pressed = key_id;
			}
		}
		else
		{	
			if (keyScan_env.key_pressed == APP_KEY_INVALID)
			{
				///a key down
				keyScan_env.key_pressed = key_id;
				keyScan_env.mix_key_release_filter.key_id = key_id;
				keyScan_env.num_of_key_pressed = num_of_key_pressing;
				key_msgSend(MAIN_BOARD_KEY, KEY_DOWN, keyScan_env.key_pressed);
				DBG_KEYPrintf ("key %d down\n\r", keyScan_env.key_pressed);	
			}
			else if (keyScan_env.key_pressed != key_id)
			{
				U8 be_filtered;
				if (keyScan_env.num_of_key_pressed < num_of_key_pressing)
				{
					be_filtered = 0; //no need filter when key pressed number increase
				}
				else
				{
					//need filter when key pressed number decrease(release)
					be_filtered = 1;
					if (keyScan_env.mix_key_release_filter.key_id != key_id)
					{
						if (keyScan_env.mix_key_release_filter.filter_cnt < MIX_KEY_REALESE_FILTER_FOCTOR)
						{
							DBG_KEYPrintf("filter a mix key %d to release\r\n", keyScan_env.mix_key_release_filter.key_id);
						}

						keyScan_env.mix_key_release_filter.key_id = key_id;
						keyScan_env.mix_key_release_filter.filter_cnt = 0;
					}
					
					if (keyScan_env.mix_key_release_filter.filter_cnt < MIX_KEY_REALESE_FILTER_FOCTOR)
					{
						if (++keyScan_env.mix_key_release_filter.filter_cnt == MIX_KEY_REALESE_FILTER_FOCTOR)
						{
							be_filtered = 0;
						}
					}
				}

				if (!be_filtered)
				{
					/// key change
					DBG_KEYPrintf ("key %d changes to %d\n\r", keyScan_env.key_pressed, key_id);

					//old key up
					key_msgSend(MAIN_BOARD_KEY, KEY_UP, keyScan_env.key_pressed);

					//new key down
					keyScan_env.key_pressed = key_id;
					keyScan_env.mix_key_release_filter.key_id = key_id;
					keyScan_env.num_of_key_pressed = num_of_key_pressing;
					key_msgSend(MAIN_BOARD_KEY, KEY_DOWN, keyScan_env.key_pressed);
				}
			}				
		}
	}

}
