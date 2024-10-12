#include "os_config.h"

#ifdef ARC_ENABLE

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hal_gpio.h"

#include "interrupt.h"
#include "fifo.h"
#include "app_timer.h"

#include "arc_detect.h"
#include "arc_process.h"

#include "LCDdevice.h"
#include "app_string_oem.h"

void arc_dectect_polling(void)	__INTERNAL_RAM_TEXT; 

volatile U8 arc_detect_status;

static U8 pre_detect = 0xFF;
static U8 cur_detect = 0xFF;


void arc_dectect_pin_init(void)
{
	hal_gpio_mux_func_select(ARC_DETECT_PIN, MUX_SEL_GPIO_INPUT);
}

void arc_dectect_status_init(void)
{
	arc_detect_status = enARC_UNPLUG;
	
	#if 1
	pre_detect = ARC_DETECT_READ;
	cur_detect = ARC_DETECT_READ;
	#else
	pre_detect = enARC_UNPLUG;
	cur_detect = enARC_UNPLUG;
	#endif
}

int arc_dectect_status_get(void)
{
	if (pre_detect == enARC_PLUG)
	{
		return enARC_PLUG;
	}
	else
	{
		return enARC_UNPLUG;
	}
}

void arc_dectect_polling(void)
{
	U8 detect_tmp;
	//static U8 pre_detect = 0xFF;
	//static U8 cur_detect = 0xFF;

	detect_tmp = ARC_DETECT_READ;
	
	if (cur_detect != detect_tmp)
	{
		cur_detect = detect_tmp;
		return;
	}
	
	if (pre_detect == detect_tmp)
	{
		return;
	}
	
	pre_detect = detect_tmp;
	
	if (pre_detect == enARC_PLUG)
	{
        //detect pin: low level
		DBG_Printf("arc Plug\n\r");

		#ifdef ARC_PLUG_TIMER_ENABLE
		if (app_timer_arc_cec_unplug_timer_get() == 0)
		{
			app_timer_arc_cec_plug_timer_set(25);
		}
		app_timer_arc_cec_unplug_timer_clr();
		#else
		func_wo_param_msgSend(arc_detect_plug);
		#endif
	}
	else
	{
        //detect pin: high level
		DBG_Printf("arc unPlug\n\r");

		#ifdef ARC_UNPLUG_TIMER_ENABLE
		//use 1000ms delay for "Sony TV KD-43X85K"
		app_timer_arc_cec_plug_timer_clr();
		app_timer_arc_cec_unplug_timer_set(1000);
		app_timer_arc_cec_init_start_timer_clr();
		#else
		func_wo_param_msgSend(arc_detect_unplug);
		#endif
	}
}

void arc_detect_plug(int param)
{
	TX_INTERRUPT_SAVE_AREA;

	DBG_Printf("%s\n\r", __func__);

	#if 1
	//hw_cec_init();
	//optek_cec_init();
	//optek_cec_open();
	#endif

	arc_cec_process_status = enARC_CEC_PRO_CONNCETED;

	arc_cec_i2c_read(0);

	#if !((defined LCD_MODULE_ENABLE) && (((defined ARC_PLUG_AT_STABNDBY_ENABLE) && (defined STANDBY_SOURCE_ENABLE)) || (defined ARC_PLUG_AT_OTHER_SRC_ENABLE)))
	if (global_vari.audio_source == SOURCE_ARC)
	#endif
	{	
		app_window_time_set(0);
		if (global_vari.audio_source == SOURCE_ARC)
		{
			app_lcd_device.display_char(STRING_SOURCE_MODE_ARC, 0, 3);
		}		
		app_lcd_device.display_char(STRING_ARC_MODE_CEC_CONNECTED, 1, 2);
		app_window_time_start();
	}

	#if ((defined ARC_PLUG_AT_STABNDBY_ENABLE) && (defined STANDBY_SOURCE_ENABLE))
	if (app_source_get_standby_mode_status())
	{
		#if	0//ndef ARC_PLUG_AT_OTHER_SRC_ENABLE
		if (global_vari.audio_source != SOURCE_ARC)
		{
			return;
		}
		#endif

		TX_DISABLE;
		arc_detect_status = enARC_PLUG;
		TX_RESTORE;

		if (global_vari.audio_source != SOURCE_ARC)
		{
			global_vari.audio_source = SOURCE_ARC;
			//key_hold_counter = FALSE;
		}

		app_source_convert_to_standby_release();
	}
	else
	#endif //((defined ARC_PLUG_AT_STABNDBY_ENABLE) && (defined STANDBY_SOURCE_ENABLE))
	#ifdef ARC_PLUG_AT_OTHER_SRC_ENABLE
	{
		#ifndef ARC_PLUG_AT_OTHER_SRC_ENABLE
		if (global_vari.audio_source != SOURCE_ARC)
		{
			return;
		}
		#endif

		TX_DISABLE;
		arc_detect_status = enARC_PLUG;
		TX_RESTORE;
		
		if (global_vari.audio_source != SOURCE_ARC)
		{
			enter_fix_source(SOURCE_ARC);
		}
	}
	#endif

}

void arc_detect_unplug(int param)
{
	TX_INTERRUPT_SAVE_AREA;

	DBG_Printf("%s\n\r", __func__);

	#if 0
	hw_cec_interrupt_set(FALSE);
	#endif

	#if !((defined LCD_MODULE_ENABLE) && (((defined ARC_PLUG_AT_STABNDBY_ENABLE) && (defined STANDBY_SOURCE_ENABLE)) || (defined ARC_PLUG_AT_OTHER_SRC_ENABLE)))
	if (global_vari.audio_source == SOURCE_ARC)
	#endif
	{
		app_window_time_set(0);
		if (global_vari.audio_source == SOURCE_ARC)
		{
			app_lcd_device.display_char(STRING_SOURCE_MODE_ARC, 0, 3);
		}
		app_lcd_device.display_char(STRING_ARC_MODE_CEC_DISCONNECTED, 1, 1);
		app_window_time_start();
	}


	TX_DISABLE;
	arc_detect_status = enARC_UNPLUG;
	hw_cec_init();
	optek_cec_init();
	optek_cec_open();
	TX_RESTORE;

	arc_cec_process_status = enARC_CEC_PRO_DISCONNCETED;
}

#endif //ARC_ENABLE
