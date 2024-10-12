
#include "os_config.h"

#ifdef HDMI_ENABLE
#include "regmap.h"

#include "hw_timer.h"
#include "hw_audio_i2s.h"
#include "hal_gpio.h"

#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"

#include "system_task.h"
#include "system_timer.h"
#include "interrupt.h"

#include "hdmi_process.h"
#include "hdmi_dec.h"

#include "ITEMT_Sys.h"


HDMI_INFOR_STRU hdmi_infor;


void hdmi_process_init(void)
{
   DBG_Printf("%s\n\r", __func__);

	memset(&hdmi_infor, 0, sizeof(hdmi_infor));
	hdmi_infor.init_enable = FALSE;
	hdmi_infor.init_status = HDMI_INIT_PROCESS_START;
	hdmi_infor.int_process_status = HDMI_INT_PROCESS_UNKNOWN;

	hdmi_hw_isr_init();
}

void hdmi_start_set(int con_flag)
{
	printf_info("%s\n\r", __func__);

	if (con_flag)
	{
		hdmi_infor.init_enable = TRUE;
	}
	else
	{	
		hdmi_infor.init_enable = FALSE;
	}
}

void hdmi_channel_set(int channel_index)
{
	SB_HostControl(channel_index, 0);
	
	printf_info("hdmi channel set:%d\n\r", channel_index);
}

void hdmi_hw_isr_init(void)
{
    printf_info("%s\n\r", __func__);
    
	hal_gpio_mux_func_select( HDMI_INIT_PIN, MUX_SEL_GPIO_INPUT);

	HDMI_INT_CLEAR;

	hal_gpio_interrupt_set(HDMI_INIT_PIN, TRUE, TRIGGER_EDGE, GPIO_INT_FALLING_EDGE);

	HDMI_INT_DISABLE;
}


void hdmi_hw_isr(void)
{
	DBG_iPrintf("%s\n\r", __func__);

	if (hdmi_infor.int_process_status == HDMI_INT_PROCESS_WAIT_START)
	{
		hdmi_infor.int_process_status = HDMI_INT_PROCESS_START;
		hdmi_infor.int_count = 0;
		
		if (global_vari.audio_source == SOURCE_HDMI)
		{
			/*
			*audio_hdmi_input_rx_disable_from_isr();
			*/
			/*
			*hw_audio_mi2s_rx_stop_from_isr();
			*/
			decode_exit_req_set_from_isr();
		}
	}

	hdmi_infor.int_count++;

	func_wo_param_msgSend_formIsr(SB_Audio_Interrupt_Rx);
}

void hdmi_int_process_finish_set(HDMI_INFOR_STRU *parm)
{
	DBG_Printf("%s\n\r", __func__);

	if ( !(global_vari.audio_source == SOURCE_HDMI) )
	{
		return;
	}

	#ifdef AUDIO_INPUT_FROM_IIS0
	if (audio_in_stream.channel != parm->channel)
	{
		audio_in_stream.channel = parm->channel;
	}
	
	hw_audio_mi2s_rx_stop();
	delayms(1);

	if (parm->channel == 0)
	{
		parm->channel = 2;
	}

	if (parm->sample_rate == 0)
	{
		parm->sample_rate = 48000;
	}

	audio_in_stream.stream_type = hdmi_infor.stream_type;
	audio_in_stream.channel = hdmi_infor.channel;
	audio_in_stream.sample_rate = hdmi_infor.sample_rate;
	audio_in_stream.sample_speed = audio_stream_speed_get(hdmi_infor.sample_rate);
	
	hw_audio_mi2s_in_channel_set(parm->channel);
	audio_clock_freq_set(parm->sample_rate);

	/*
	* (hdmi_infor.stream_type == AudType_LPCM)
	*/

	delayms(1);
	hw_audio_mi2s_rx_start();
	
	hdmi_infor.int_process_status = HDMI_INT_PROCESS_FINISH;

	/*
	*audio_hdmi_input_rx_enable();
	*/

	#endif //AUDIO_INPUT_FROM_IIS0
}

#endif //HDMI_ENABLE
