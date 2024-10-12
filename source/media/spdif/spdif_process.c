/*
 * spdif_proces.c
 *
 *  Created on: 20230616
 *      Author: Windowns
 */

#include "os_config.h"

#ifdef SPDIF_ENABLE
#include "regmap.h"

#include "mem_reloc.h"

#include "hw_pll.h"

#include "hal_gpio.h"
#include "hal_dma.h"

#include "sdk_com.h"

#ifdef SPDIF_ENABLE
#include "spdif_data_parse.h"
#include "spdif_process.h"
#include "audio_optical_input.h"
#include "audio_coaxial_input.h"
#endif

#include "audio_input_receive_task.h"

#ifdef HDMI_ENABLE
#include "hdmi_process.h"
#endif


U8 spdif_dect_cn;
U8 spdif_waiting_cnt;
U8 spdif_rx_down_sample;

SPDIF_IN_STREAM_STRUC spdif_in_stream;


void spdif_dect_cn_init(void)
{
	spdif_dect_cn = 0;
}

void spdif_rcv_waiting_timeoout_init(void)
{
	spdif_waiting_cnt = 0;
}

void spdif_dma_rx_enable(void)
{
}

void spdif_dma_rx_disable(void)
{
	#ifdef OPTICAL_ENABLE
	spdifRX_Dma_Opened = FALSE;
	#endif
	#ifdef COAXIAL_ENABLE
	coaxialRX_Dma_Opened = FALSE;
	#endif
	hal_dma_close(SPDIF_IN_RX_DMA_ID);
}

void Spdif_rcv_enable(void)
{
	DBG_Printf("%s\n\r", __func__);

	#ifdef SPDIF_ENABLE
	hw_spdif_dec_disable(); //reset fifo	
	asm("nop");
	hw_spdif_dec_enable();
	#endif

    if (global_vari.audio_source == SOURCE_OPTICAL)
    {
		#ifdef OPTICAL_ENABLE
		audio_optical_input_rx_init();
		#endif
	}
	else if (global_vari.audio_source == SOURCE_COAXIAL)
    {
		#ifdef COAXIAL_ENABLE
		audio_coaxial_input_rx_init();
		#endif
	}
    else if (global_vari.audio_source == SOURCE_ARC)
	{
		#ifdef ARC_ENABLE
		audio_arc_input_rx_init();
		#endif
	}
    else if (global_vari.audio_source == SOURCE_HDMI)
	{
    	asm("nop");
		/*
		* hdmi output select spdif out
		* #ifdef HDMI_ENABLE 
		* audio_hdmi_input_rx_init();
		* #endif
		*
		* */
	
	}
	else
	{
		DBG_Assert(FALSE);
		DBG_Printf("%s error\n\r", __func__);
	}
}

void Spdif_rcv_disable(void)
{
	DBG_Printf("%s\n\r", __func__);

	#ifdef SPDIF_ENABLE
    if (global_vari.audio_source == SOURCE_OPTICAL)
	{
		#ifdef OPTICAL_ENABLE
		audio_optical_input_rx_disable();
		#endif
	}
	else if (global_vari.audio_source == SOURCE_COAXIAL)
    {
		#ifdef COAXIAL_ENABLE
		audio_coaxial_input_rx_disable();
		#endif
	}
    else if (global_vari.audio_source == SOURCE_ARC)
	{
		#ifdef ARC_ENABLE
		audio_arc_input_rx_disable();
		#endif
	}
	/*
	*	hdmi output select spdif out
	* 	else if (global_vari.audio_source == SOURCE_HDMI)
	*	{
	*		#ifdef HDMI_ENABLE
	*		audio_hdmi_input_rx_disable();
	*		#endif
	*	}
	*
	**/

	spdif_dma_rx_disable();
	#endif
}

void spdif_in_stream_data_type_init(void)
{
	spdif_in_stream.data_type = SPDIF_STREAM_UNKNOWN;
}

void spdif_in_detect_process_init(void)
{
	spdif_in_stream.detect_status = STREAM_SPDIF_SR_DETECT;
}

int spdif_in_stream_data_type_get(void)
{
	int temp = 	spdif_in_stream.data_type;
	return temp;
}

void spdif_detect_process (U32 spdif_parm)
{
	static U8 count;
	U32 tmp;
	U8 detected;
	U8 need_to_check_32k;
	U32 sample_rate;
	U8 stream_detect_status = spdif_in_stream.detect_status;

	need_to_check_32k = 0;
	detected = 0;

	switch (spdif_parm)
	{
	case UI_FIT_TIMER:
		if (stream_detect_status == STREAM_SPDIF_SR_DETECT_32k)
		{
		    spdif_dect_cn++;
			if (spdif_dect_cn < 4)
			{
				break;
			}

			tmp = SPDIF_DEC_SAMPLE_RATE_DETECT;
			tmp =((tmp & 0xff) + ((tmp>>8) & 0xff) );
			if (tmp != 0)
			{
				//printf_info ("spdif 0x%x\n\r", tmp);
			}

			if ( (tmp >= 0x30) && (tmp <= 0x32) )
			{
				sample_rate = 32000*1;
				detected = 1;
				audio_in_stream.sample_rate = 32000;
				printf_info ("SPDIF Sample Rate 32k\n\r");
			}
			else
			{
				detected = 0;
				need_to_check_32k = 0;
			}

			goto detect_enter;
		}
		else if (stream_detect_status == STREAM_SPDIF_SR_DETECT)
		{
			spdif_dect_cn++;

			if (spdif_dect_cn < 4)
			{
				break;
			}

			tmp = SPDIF_DEC_SAMPLE_RATE_DETECT;

			tmp =((tmp & 0xff) + ((tmp>>8) & 0xff) );

			if ( (tmp == 0) || (tmp == 0xFF) )
			{
				asm("nop");
			}
			else
			{
				//printf_info ("spdif 0x%x\n\r", tmp);
			}

			detected = 0;
			
			if ( (tmp >= (0x64-2)) && (tmp <= (0x64+2)) )
			{
				sample_rate = 48000*1;
				detected = 1;
				DBG_Printf ("SPDIF Sample Rate 48k\n\r");
			}
			else if ( (tmp >= (0x32-1)) && (tmp <= (0x32+1)) )
			{
				sample_rate = 48000*2;
				detected = 1;
				DBG_Printf ("SPDIF Sample Rate 96k\n\r");
			}
			else if ( (tmp >= 0x18) && (tmp <= 0x1B) )
			{
				sample_rate = 48000*4;
				detected = 1;
				DBG_Printf ("SPDIF Sample Rate 192k\n\r");
			}
			else if ( (tmp >= (0x6C-2)) && (tmp <= (0x6C+2)) )
			{
				sample_rate = 44100*1;
				detected = 1;
				DBG_Printf ("SPDIF Sample Rate 44.1k\n\r");
			}
			else if ( (tmp >= (0x36-1)) && (tmp <= (0x36+1)) )
			{
				sample_rate = 44100*2;
				detected = 1;
				DBG_Printf ("SPDIF Sample Rate 88.2k\n\r");
			}
			else if ( (tmp >= 0x1C) && (tmp <= 0x1D) )
			{
				sample_rate = 44100*4;
				detected = 1;
				DBG_Printf ("SPDIF Sample Rate 176.4k\n\r");
			}
			else if ( (tmp >= 0x90*2) && (tmp <= 0x96*2) )
			{
				//please note max detect value is 31
				need_to_check_32k = 1;
				DBG_Printf ("Recheck SPDIF Sample Rate 32k?\n\r");
			}
			else if ( (tmp >= 0x50) && (tmp <= 0x52) )
			{
				sample_rate = 32000*2;
				detected = 1;
				DBG_Printf ("SPDIF Sample Rate 64k\n\r");
			}
			if ( (tmp >= 0x24) && (tmp <= 0x27) )
			{
				sample_rate = 32000*4;
				detected = 1;
				DBG_Printf ("SPDIF Sample Rate 128k\n\r");
			}
			else
			{

			}

detect_enter:

			#ifdef HDMI_ENABLE
			if (detected == 1)
			{
				int decode_type = decode_type_get();
				if (decode_type == DECODE_DD_AC3)
				{
					audio_in_stream.channel = 2;
				}
				else if (decode_type == DECODE_DTS_DMP)
				{
					audio_in_stream.channel = 2;
				}
				else if (decode_type == DECODE_DDPLUS71)
				{
					if (audio_in_stream.channel > 2)
					{
						detected = 0;
						DBG_Printf ("(hdmi_infor.channe=%d) > 2, no support!\n\r", audio_in_stream.channel);
					}
				}
				else
				{
					audio_in_stream.channel = 2;
				}
			}
			#endif

			if (detected == 1)
			{
				DBG_Printf("SPDIF detected\n\r");

				#ifdef SPDIF_ENABLE
				DBG_Printf ("SPDIF Sample Rate=%d\n\r", sample_rate);
				
				audio_in_stream.sample_rate = sample_rate;
				audio_in_stream.sample_speed = audio_stream_speed_get(sample_rate); 
				
				audio_clock_freq_set(sample_rate);
				hw_audio_mi2s_in_channel_set(audio_in_stream.channel);

				stream_detect_status = STREAM_SPDIF_SR_DETECT_FINISH;
				audio_in_stream.status = STREAM_WAITING_DATA;
				spdif_waiting_cnt = 0;

				hw_spdif_dec_disable ();
				hw_spdif_sr_detect_disable ();

				Spdif_rcv_enable();
				hw_spdif_dec_enable();				
				#endif //SPDIF_ENABLE
			}
			else
			{
				#ifdef SPDIF_ENABLE
				hw_spdif_sr_detect_disable ();
				#endif

				if (need_to_check_32k == 0)
				{
					stream_detect_status = STREAM_SPDIF_SR_REDETECT;

					#ifdef SPDIF_ENABLE
					hw_spdif_detect_level(1); //restart detect high
					#endif
				}
				else
				{
					stream_detect_status = STREAM_SPDIF_SR_REDETECT_32k;
					
					#ifdef SPDIF_ENABLE
					hw_spdif_detect_level(0); //restart detect low
					#endif
				}
			}
		}
		else if (stream_detect_status == STREAM_SPDIF_SR_REDETECT)
		{
			#ifdef SPDIF_ENABLE
			hw_spdif_sr_detect_enable();
			#endif

			spdif_dect_cn_init();

			stream_detect_status = STREAM_SPDIF_SR_DETECT;
		}
		else if(stream_detect_status == STREAM_SPDIF_SR_REDETECT_32k)
		{
			spdif_dect_cn_init();

			#ifdef SPDIF_ENABLE
			hw_spdif_sr_detect_enable();
			#endif

			stream_detect_status = STREAM_SPDIF_SR_DETECT_32k;
		}
		

		if (audio_in_stream.status == STREAM_WAITING_DATA)
		{
			spdif_waiting_cnt++;

			/*
			 * 20  -> 1s
			 * 40  -> 2s
			 * 60  -> 3s
			 * 80  -> 4s
			 * 160 -> 8s
			 */
			if (spdif_waiting_cnt > 20)
			{
				DBG_Printf("SPDIF Underflow 2\n");

				//no data input for about x sec
				spdif_waiting_cnt = 0;
	            system_msgSend(UI_SPDIF_STREAM_UNDERFLOW);
			}
		}
		else if (audio_in_stream.status == STREAM_MEDIA)
		{

		}
		break;

	case UI_DECODE_FINISH:
		printf_info("ui dec finish\n\r");

	case UI_SPDIF_STREAM_UNDERFLOW:
		printf_info("under flow\n\r");

	case UI_SPDIF_STREAM_OVERFLOW:
		printf_info("SPDIF over/under flow\n\r");	

		decode_exit_req_set();

		Spdif_rcv_disable ();

		hw_pll_audio_clock_set(SF_BASE_DET, SPDIF_SF_4x);

		spdif_dect_cn_init();

		stream_detect_status = STREAM_SPDIF_SR_REDETECT;

		printf_info ("SPDIF stream check\n\r");
		break;

	default:
		break;
	}

	spdif_in_stream.detect_status = stream_detect_status;
}

void spdif_process_sync_lost(int lost_counter)
{
	DBG_Printf("%s\n\r", __func__);

	app_nav_spdif_stream_reinit();
}

#endif //SPDIF_ENABLE
