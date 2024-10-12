#include <xtensa/tie/xt_hifi2.h>

#include "os_config.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hal_gpio.h"

#include "sdk_com.h"
#include "audio_com.h"
#include "audio_output.h"
#include "audio_aux_input.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "audio_input_receive_task.h"

#ifdef SPDIF_ENABLE
#include "spdif_data_parse.h"
#include "spdif_process.h"
#endif

#ifdef HDMI_ENABLE
#include "hdmi_process.h"
#include "audio_hdmi_input.h"
#endif

#ifdef OPTICAL_ENABLE
#include "audio_optical_input.h"
#endif

#ifdef COAXIAL_ENABLE
#include "audio_coaxial_input.h"
#endif

#include "interrupt.h"
#include "fifo.h"


#define PEAKLEVEL_ENABLE

#ifdef PEAKLEVEL_ENABLE
#include "peak_level_meter.h"
static peak_level_det peak_det_handle;
U32 max_peak;
#endif


//#define DBG_Printf

#define HDMI_QUICK_EXIT

//#define OPTICAL_QUICK_EXIT


/*
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */
#define QUEUE_DEEP_LENGTH            16

#define TASK_AUDIO_RCV_STACK_SIZE    1024

U32 audio_input_rx_buf[2][AUDIO_INPUT_BUF_SIZE/4] __attribute__(( aligned (64) ));

AUDIO_INPUT_RX_TAST_MSG audio_input_rx_task_msg __attribute__(( aligned (16) ));

#define STREAM_FIFO_SIZE    (100000)


void audio_input_rx_task_init(void);
int audio_input_rx_wstream_callback (U32 size) __INTERNAL_RAM_TEXT;
void audio_input_stream_rx(U8 * buf, I16 len, int input_source) __INTERNAL_RAM_TEXT;
void audio_input_stream_rx_int(void);


static void audio_input_rx_task (void *param)
{
    AUDIO_INPUT_RX_TAST_MSG qMsgData;

  	DBG_Printf("audio input rx task start!\n\r");

    XT_INTS_ON(LEVEL3_INT9_MASK);

    audio_input_rx_task_msg.queue = xQueueCreate( QUEUE_DEEP_LENGTH, sizeof(AUDIO_INPUT_RX_TAST_MSG) );

    memset(audio_input_rx_buf, 0, sizeof(audio_input_rx_buf));
 
	#ifdef SPDIF_ENABLE
	spdif_in_stream.data_type = SPDIF_STREAM_UNKNOWN;
	#endif

    while(1)
    {
        if (QUEUE_AUDIO_IN_RX_GET(audio_input_rx_task_msg.queue, (void *)&qMsgData) == pdPASS)
        {
			if (qMsgData.source == AUDIO_INPUT_RX_SOURCE_HDMI)
            {
                 audio_input_stream_rx(qMsgData.buf, qMsgData.byte_length, AUDIO_INPUT_RX_SOURCE_HDMI);
            }
            else if (qMsgData.source == AUDIO_INPUT_RX_SOURCE_OPTICAL)
			{
                 audio_input_stream_rx(qMsgData.buf, qMsgData.byte_length, AUDIO_INPUT_RX_SOURCE_OPTICAL);
			}
            else if (qMsgData.source == AUDIO_INPUT_RX_SOURCE_COAXIAL)
			{
                 audio_input_stream_rx(qMsgData.buf, qMsgData.byte_length, AUDIO_INPUT_RX_SOURCE_COAXIAL);
			}
            else if (qMsgData.source == AUDIO_INPUT_RX_SOURCE_ARC)
			{
                 audio_input_stream_rx(qMsgData.buf, qMsgData.byte_length, AUDIO_INPUT_RX_SOURCE_ARC);
			}
            else if (qMsgData.source == AUDIO_INPUT_RX_SOURCE_AUX)
			{
                 audio_input_stream_rx(qMsgData.buf, qMsgData.byte_length, AUDIO_INPUT_RX_SOURCE_AUX);
			}
			else
			{
				DBG_Assert(FALSE);
			}
        }
    }
}


extern U8 SPDIF_STREAM_START[];
extern U8 SPDIF_STREAM_END[];



//Normal AC3
//payload bytes:0xA00(2560), repeat size of bytes:0x1800, 1024*24/(0x400*3) = 9.6 frames

//data rate:32k av's AC3, once channel
//payload bytes:0x400/8=0x80, repeat size of bytes:0x1800, 1024*5/0x80 = 40 frames;

//data rate:48k av's AC3, once channel
//payload bytes:0xC0, repeat szie:0x1800

//data rate:128k av's AC3, once channel
//payload bytes:0x200, repeat szie:0x1800

//data rate:128k av's AC3, 2 channels
//payload bytes:0x200, repeat szie:0x1800

//data rate:64k av's AC3, 2 channels
//payload bytes:0x100, repeat szie:0x1800

//data rate:96k av's AC3, 2 channels
//payload bytes:0x180, repeat szie:0x1800

//data rate:192k av's AC3, 2 channels
//payload bytes:0x300, repeat szie:0x1800

//data rate:256k av's AC3, 2 channels
//payload bytes:0x400, repeat szie:0x1800

//data rate:256k av's AC3, 2 channels
//payload bytes:0x400, repeat szie:0x1800

//data rate:384k av's AC3, 2 channels
//payload bytes:0x600, repeat szie:0x1800

//data rate:448k av's AC3, 2 channels
//payload bytes:0x700, repeat szie:0x1800

//data rate:640k av's AC3, 2 channels
//payload bytes:0x7E0, repeat szie:0x1800

//data rate:448k av's AC3, 3 channels
//payload bytes:0x700, repeat szie:0x1800

//data rate:448k av's AC3, 4 channels
//payload bytes:0x700, repeat szie:0x1800

//data rate:448k av's AC3, 4 channels
//payload bytes:0x700, repeat szie:0x1800

//data rate:448k av's AC3, 4 channels
//payload bytes:0x700, repeat szie:0x1800

//data rate:64k -> 640k av's AC3, 4 channels
//payload bytes:0x100, repeat szie:0x6000
//payload changed!:0x180
//payload changed!:0x200
//payload changed!:0x300
//payload changed!:0x400
//payload changed!:0x700
//payload changed!:0x800
//payload changed!:0xA00

//data rate:448k av's AC3, 5 channels. sample rate:44.1k
//payload bytes:0x79E, repeat szie:0x1800


//Normal EC3
//payload bytes:0x1400(5120), repeat size of bytes:0x6000, 1024*32/5120 = 6.4 frames

//data rate:32k av's AC3, once channel
//payload bytes:0x80, repeat szie of bytes:0x6000

//data rate:64k av's EC3, 2 channels
//payload bytes:0x100, repeat szie:0x6000

//data rate:96k av's EC3, 2 channels
//payload bytes:0x180, repeat szie:0x6000

//data rate:128k av's EC3, 2 channels
//payload bytes:0x200, repeat szie:0x6000

//data rate:192k av's EC3, 2 channels
//payload bytes:0x300, repeat szie:0x6000

//data rate:256k av's EC3, 2 channels
//payload bytes:0x400, repeat szie:0x6000


//data rate:448k av's EC3, 5 channels
//payload bytes:0x800, repeat szie:0x6000

//data rate:512k av's EC3, 5 channels
//payload bytes:0x800, repeat szie:0x6000

//data rate:640k av's EC3, 5 channels
//payload bytes:0xA00, repeat szie:0x6000

//data rate:800k av's EC3, 5 channels
//payload bytes:0xC80, repeat szie:0x6000

//data rate:1080k av's EC3, 5 channels
//payload bytes:0xFC0, repeat szie:0x6000

//data rate:1110k av's EC3, 5 channels
//payload bytes:0x112E, repeat szie:0x6000

//data rate:1200k av's EC3, 5 channels
///payload bytes:0x12C0, repeat szie:0x6000

//data rate:1299k av's EC3, 5 channels
//payload bytes:0x144C, repeat szie:0x6000

//data rate:1512k av's EC3, 5 channels
//payload bytes:0x17A0, repeat szie:0x6000

//data rate:1599k av's EC3, 5 channels
//payload bytes:0x18FC, repeat szie:0x6000

//data rate:3024k av's EC3, 5 channels
//payload bytes:0x2F40, repeat szie:0x6000

//data rate:3999k av's EC3, 5 channels
//payload bytes:0x3E7C, repeat szie:0x6000

//data rate:4999k av's EC3, 5 channels
//payload bytes:0x4E18, repeat szie:0x6000

//data rate:6138k av's EC3, 5 channels
//payload bytes:0x4E18, repeat szie:0x6000

//data rate:1008k <- 512k, av's EC3, 5 channels
//payload bytes:0x112E, repeat szie:0x6000
//payload changed!:0x12C0
//payload changed!:0x144C
//payload changed!:0x17A0

//data rate:1100k <- 512k, av's EC3, 5 channels
//payload bytes:0x112E, repeat szie:0x6000
//payload changed!:0x12C0
//payload changed!:0x144C
//payload changed!:0x17A0

//data rate:1600k -> 6140k, av's EC3, 5 channels
//payload bytes:0x18FC, repeat szie:0x6000
//payload changed!:0x2F40
//payload changed!:0x3E7C
//payload changed!:0x4E18
//payload changed!:0x5FE8


int audio_input_aux_rx_wstream_callback (U32 size)
{
#ifdef AUX_ENABLE
	int len;
	int data_type;

	if (audio_in_stream.prepare_status == AUDIO_IN_STREAM_PREPARE_WAITING)
	{
		int payload_byte_size;

		size = AUDIO_FIFO_STREAM_DATA_LEN();
		len = AD_PCM_BLOCK_SIZE*2;

		if (size >= len)
		{
 			DBG_Printf("start to send decoding msg\n\r");

		    aux_decocde_set();
		    aux_decocde_start_w_init();

			audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_READY;
		}
	}
#endif //SPDIF_ENABLE

	return 0x0;
}

int audio_input_rx_wstream_callback (U32 size)
{
#ifdef SPDIF_ENABLE
	int len;
	int data_type;

	data_type = spdif_in_stream.data_type;
	spdif_rcv_waiting_timeoout_init();

	if (audio_in_stream.prepare_status == AUDIO_IN_STREAM_PREPARE_WAITING)
	{
		int payload_byte_size;
		payload_byte_size = spdif_stream_frame_payload_byte_size();

		#if (defined DOLBY_AC3_DEC_ENABLE || defined DOLBY_DDPLUS71_DEC_ENABLE)
		if ( (data_type == SPDIF_STREAM_AC3) )
		{
			if (payload_byte_size == 0x80)
			{
				//32k, one channle
				len = payload_byte_size * 40; //av's ac3, ok
			}
			else if (payload_byte_size == 0xC0)
			{
				//48k, one channel
				len = payload_byte_size * 30; //av's ac3, ok
			}
			else if (payload_byte_size == 0x100)
			{
				//64k, 2 channels
				len = payload_byte_size * 20; //av's ac3, ok
			}
			else if (payload_byte_size == 0x180)
			{
				//96k,2 channels
				len = payload_byte_size * 20; //av's ac3, ok
			}
			else if (payload_byte_size == 0x200)
			{
				//128k, 1 channel
				len = payload_byte_size * 20; //av's ac3, ok
			}
			else if (payload_byte_size == 0x300)
			{
				//192k,2 channels
				len = payload_byte_size * 20; //av's ac3, ok
			}
			else if (payload_byte_size == 0x400)
			{
				//256k,2 channels
				len = payload_byte_size * 10; //av's ac3, ok
			}
			else if (payload_byte_size == 0x600)
			{
				//384k,2 channels
				len = payload_byte_size * 10; //av's ac3, ok
			}
			else if (payload_byte_size == 0x800)
			{
				//512k,2 channels
				len = payload_byte_size * 10; //av's ac3, ok
			}
			else
			{
				len = payload_byte_size * 9; //normal ac3
			}
		}
		else if ( (data_type == SPDIF_STREAM_EC3) )
		{
			if (payload_byte_size > 0x1400)
			{
				len = 1024*16;
			}
			else
			{
				if (payload_byte_size == 0x80)
				{
					//32k, one channel
					len = payload_byte_size * 40; //av's ac3, ok
				}
				else if (payload_byte_size == 0xC0)
				{
					//48k, one channel
					len = payload_byte_size * 30; //av's ac3, ok
				}
				else if (payload_byte_size == 0x100)
				{
					//64k, 2 channels
					len = payload_byte_size * 20; //av's ac3, ok
				}
				else if (payload_byte_size == 0x180)
				{
					//96k,2 channels
					len = payload_byte_size * 20; //av's ac3, ok
				}
				else if (payload_byte_size == 0x200)
				{
					//128k, 1 channel
					len = payload_byte_size * 20; //av's ac3, ok
				}
				else if (payload_byte_size == 0x300)
				{
					//192k,2 channels
					len = payload_byte_size * 20; //av's ac3, ok
				}
				else if (payload_byte_size == 0x400)
				{
					//256k,2 channels
					len = payload_byte_size * 10; //av's ac3, ok
				}
				else if (payload_byte_size == 0x600)
				{
					//384k,2 channels
					len = payload_byte_size * 10; //av's ac3, ok
				}
				else if (payload_byte_size == 0x800)
				{
					//512k,2 channels
					len = payload_byte_size * 10; //av's ac3, ok
				}
				else
				{
					/*
					*Normal Bit Rate ec3
 					/*payload_byte_size * 9;
					*/
					len = payload_byte_size * 6; //normal ec3
				}
			}
		}
		else if ( (data_type == SPDIF_STREAM_DTS) )
		{
			//len = 1024*32;
			len = 1024*64;
		}
		else
		#endif //(defined DOLBY_AC3_DEC_ENABLE || defined DOLBY_DDPLUS71_DEC_ENABLE)
		{
			#ifdef ATMOS_CODEC_ENABLE
			len = 1024*16;
			#else
			//len = 64*4*2;//192*4*2/2/2/2/2;
			len = 192*4*4;
			#endif
		}

		if (size >= len)
		{
 			DBG_Printf("start to send decoding msg\n\r");

			#ifdef AUX_ENABLE
 			if (global_vari.audio_source == SOURCE_AUX)
			{
			    aux_decocde_set();
			    aux_decocde_start_w_init();
			}
			else
			{
				decode_start_w_init_msgSend();
			}
			#else
 			decode_start_w_init_msgSend();
			#endif //AUX_ENABLE

			audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_READY;
		}
	}
#endif //SPDIF_ENABLE

	return 0x0;
}

void audio_input_stream_rx_int(void)
{
	#ifdef SPDIF_ENABLE
	spdif_in_stream.data_type = SPDIF_STREAM_UNKNOWN;
	spdif_in_stream.detect_status = STREAM_SPDIF_SR_DETECT;
	#endif
}

void audio_input_rx_stream_config( void )
{
	U8 *pStreamBuf;
	U32 size;

	TX_INTERRUPT_SAVE_AREA;

	DBG_Printf( "%s\n\r", __func__);

	TX_DISABLE;

	AUDIO_FIFO_STREAM_FLUSH();
	
	pStreamBuf = SPDIF_STREAM_START;
	size = SPDIF_STREAM_END - SPDIF_STREAM_START;
	AUDIO_FIFO_STREAM_FLUSH();
	AUDIO_FIFO_STREAM_OPEN(pStreamBuf, size);
	AUDIO_FIFO_STREAM_CREATE(0,STREAM_LENTH_UNKNOWN,0,0);

	TX_RESTORE;
} 


#define AUDIO_OUT_CLK_SET_FIXED

void audio_input_stream_rx(U8 * buf, I16 len, int input_source)
{
	TX_INTERRUPT_SAVE_AREA;

	U32 size;
	U8 *pInBuf = buf;
	#ifdef SPDIF_ENABLE
  	enSPDIF_STREAM_TYPE stream_type;
	#endif
	U16 insize = len;

	#ifdef HDMI_ENABLE
    if (global_vari.audio_source == SOURCE_HDMI)
	{
		if ( !(hdmi_infor.init_status == HDMI_INIT_PROCESS_END) )
		{
			return;
		}
		
		if (hdmi_infor.int_process_status == HDMI_INT_PROCESS_UNKNOWN)
		{
			return;
		}	
		else if ( ( hdmi_infor.int_process_status == HDMI_INT_PROCESS_START) )
		{
			return;
		}
		else if ( hdmi_infor.int_process_status == HDMI_INT_PROCESS_FINISH)
		{
			hdmi_infor.int_process_status = HDMI_INT_PROCESS_WAIT_START;
			audio_in_stream.status = STREAM_DECOCDE_FINISH;

			DBG_Puts ("\n\r");
			DBG_Printf ("audio in,channel=%d,sample rate=%d\n\r", audio_in_stream.channel, audio_in_stream.sample_rate);
			DBG_Puts ("\n\r");
		}
		else
		{
			asm("nop");
		}
	}
	// else
	// {
	// 	if (global_vari.audio_source == SOURCE_AUX)
	// 	{
	// 		if ( hdmi_infor.int_process_status == HDMI_INT_PROCESS_FINISH)
	// 		{
	// 			hdmi_infor.int_process_status = HDMI_INT_PROCESS_WAIT_START;
	// 			audio_in_stream.status = STREAM_DECOCDE_FINISH;
	// 		}
	// 	}
	// }
	#endif //HDMI_ENABLE


	#ifdef AUX_ENABLE
	if (global_vari.audio_source == SOURCE_AUX)
	{
		if (audio_in_stream.status == STREAM_DECOCDE_FINISH)
		{
			#ifdef HDMI_ENABLE
			hdmi_infor.int_process_status =  HDMI_INT_PROCESS_WAIT_START;
			#endif

			audio_in_stream.status = STREAM_WAITING_DATA;
			audio_input_rx_stream_config();
			audio_aux_input_rx_init();
			return;
		}

		size = (U32) AUDIO_FIFO_STREAM_PUT_DATA(pInBuf, insize);

		if ( size != insize )
		{
			DBG_Printf("overflow 1\r\n");
		}
		else
		{
			size = AUDIO_FIFO_STREAM_DATA_LEN();
			/*
			* zero_action = audio_input_rx_wstream_callback (size);
			* zero_action = audio_input_aux_rx_wstream_callback (size);
			*/
			audio_input_aux_rx_wstream_callback (size);
		}
		return;
	}
	#endif //AUX_ENABLE


 	#ifdef SPDIF_ENABLE
	U32 *pout;
    U16 consumed;
	U16 zero_action;
	U16 outsize;
	//U16 insize = len;

	#ifdef HDMI_ENABLE
    if (global_vari.audio_source == SOURCE_HDMI)
	{
		if (audio_in_stream.status == STREAM_DECOCDE_FINISH)
		{
			#ifdef HDMI_ENABLE
			hdmi_infor.int_process_status =  HDMI_INT_PROCESS_WAIT_START;

			audio_in_stream.status = STREAM_WAITING_DATA;
			audio_input_rx_stream_config();
			audio_hdmi_input_rx_init();
			#endif

			return;
		}
	}
    else
	#endif
	#ifdef OPTICAL_ENABLE
	if (global_vari.audio_source == SOURCE_OPTICAL)
	{
		if (audio_in_stream.status == STREAM_DECOCDE_FINISH)
		{
			#ifdef HDMI_ENABLE
			hdmi_infor.int_process_status =  HDMI_INT_PROCESS_WAIT_START;
			#endif

			audio_in_stream.status = STREAM_WAITING_DATA;
			audio_input_rx_stream_config();
			audio_optical_input_rx_init();
			return;
		}
	}
	else
	#endif
	#ifdef COAXIAL_ENABLE
	if (global_vari.audio_source == SOURCE_COAXIAL)
	{
		if (audio_in_stream.status == STREAM_DECOCDE_FINISH)
		{
			#ifdef HDMI_ENABLE
			hdmi_infor.int_process_status =  HDMI_INT_PROCESS_WAIT_START;
			#endif

			audio_in_stream.status = STREAM_WAITING_DATA;
			audio_input_rx_stream_config();
			audio_coaxial_input_rx_init();
			return;
		}
	}
	else
	#endif	
	#ifdef ARC_ENABLE
    if (global_vari.audio_source == SOURCE_ARC)
	{
		if (audio_in_stream.status == STREAM_DECOCDE_FINISH)
		{
			#ifdef HDMI_ENABLE
			hdmi_infor.int_process_status =  HDMI_INT_PROCESS_WAIT_START;
			#endif

			audio_in_stream.status = STREAM_WAITING_DATA;
			audio_input_rx_stream_config();
			audio_arc_input_rx_init();
			return;
		}
	}
	#else
	{
		
	}
	#endif

	#if ( (defined HDMI_ENABLE) || (defined OPTICAL_ENABLE) || (defined COAXIAL_ENABLE) || (defined ARC_ENABLE) )
	do
	{
		stream_type = spdif_stream_preProcess(pInBuf, insize, &pout, &outsize, &consumed);

		#ifdef PCM_DEC_USE_ATMOS
		if ( (stream_type == SPDIF_STREAM_PCM) &&
			(audio_in_stream.type != AudType_LPCM) )
		{
			stream_type = SPDIF_STREAM_UNKNOWN;
			outsize = 0;
		}
		#endif

		if (spdif_in_stream.data_type != stream_type)
		{
            DBG_Printf("audio stream diff:%d\n\r", stream_type);
			
 		    if (global_vari.audio_source == SOURCE_HDMI)
            {
	            #ifdef HDMI_QUICK_EXIT
	            if (spdif_in_stream.data_type != SPDIF_STREAM_UNKNOWN)
    	        {
        	        //when sync lost, immediately exit DD Plus decoding
            	    decode_exit_req_set();
                    //return;
                }
				#else
               	if (spdif_in_stream.data_type != SPDIF_STREAM_UNKNOWN)
                {
                    audio_in_stream.status = STREAM_DECOCDE_FINISH;
                    return;
                }
				#endif
            }
		    else if ( (global_vari.audio_source == SOURCE_OPTICAL)  ||
				(global_vari.audio_source == SOURCE_COAXIAL) ||
				(global_vari.audio_source == SOURCE_ARC) )
			{
	            #ifdef OPTICAL_QUICK_EXIT
	            if (spdif_in_stream.data_type != SPDIF_STREAM_UNKNOWN)
	            {
	                decode_exit_req_set();
	                return;
	            }
	            #else
				if (spdif_in_stream.data_type != SPDIF_STREAM_UNKNOWN) 
                {
				    audio_in_stream.status = STREAM_DECOCDE_FINISH;
				    return;
                }
				#endif
            }
            else
            {
                DBG_Assert(FALSE);
            }
			
			spdif_in_stream.data_type = stream_type;

			if (spdif_in_stream.data_type == SPDIF_STREAM_UNKNOWN)
			{
			   /*
				* return;
				*/
			}

			audio_input_rx_stream_config();

			switch (spdif_in_stream.data_type)
			{
			#if (defined DOLBY_AC3_DEC_ENABLE || defined DOLBY_DDPLUS71_DEC_ENABLE)
			case SPDIF_STREAM_AC3:
				#if ((defined DOLBY_AC3_DEC_ONLY)||(!(defined DOLBY_DDPLUS71_DEC_ENABLE)))
				dd_ac3_decocde_set();
				#else
				ddplus71_decocde_set();
				#endif
				
				if (input_source == AUDIO_INPUT_RX_SOURCE_HDMI)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED
					
					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(0);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MCLK:interanl clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif //MCLK_IN_AS_AUDIO_OUT_CLK
					
					#endif //AUDIO_OUT_CLK_SET_FIXED
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_OPTICAL)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED
					
					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(0);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MCLK:interanl clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif //MCLK_IN_AS_AUDIO_OUT_CLK
					
					#endif //AUDIO_OUT_CLK_SET_FIXED
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_COAXIAL)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED
					
					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(0);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MCLK:interanl clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif //MCLK_IN_AS_AUDIO_OUT_CLK
					
					#endif //AUDIO_OUT_CLK_SET_FIXED
				}
				else
				{
					DBG_Assert(FALSE);
					DBG_Printf ("input source rx msg error1!\n\r");
				}

				DBG_Printf ("SPDIF Dolby AC3\n\r");
				break;
			#else
			case SPDIF_STREAM_AC3:
				DBG_Printf ("SPDIF AC3 No Soupport\n\r");
				break;
			#endif //(defined DOLBY_AC3_DEC || defined DOLBY_DDPLUS71_DEC_ENABLE)

			#ifdef AAC_MCH_DEC
			case SPDIF_STREAM_AAC:
				DBG_Printf ("SPDIF AAC\n\r");
				break;
			#endif //AAC_MCH_DEC

			#if (defined DOLBY_DDPLUS71_DEC_ENABLE || defined ATMOS_CODEC_ENABLE)
			case SPDIF_STREAM_EC3:
				#ifdef ATMOS_CODEC_ENABLE
				dd_atmos_decocde_set();
				#elif defined DOLBY_DDPLUS71_DEC_ENABLE
				ddplus71_decocde_set();
				#else
				DBG_Assert(FALSE);
				#endif

				if (input_source == AUDIO_INPUT_RX_SOURCE_HDMI)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED
				
					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(2);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MClk:internal clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif

					#ifdef HDMI_ENABLE
					audio_in_stream.sample_speed = audio_stream_speed_get(hdmi_infor.sample_rate);
					#endif

					#endif
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_OPTICAL)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED

					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(2);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MClk:internal clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif

					#endif
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_COAXIAL)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED

					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(2);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MClk:internal clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif

					#endif
				}
				else
				{
					DBG_Assert(FALSE);
					DBG_Printf ("input source rx msg error12\n\r");
				}

				DBG_Printf ("SPDIF EC3\n\r");
				break;
			#else
			case SPDIF_STREAM_EC3:
				DBG_Printf ("SPDIF EC3 No Support\n\r");
				break;
			#endif //(defined DOLBY_DDPLUS71_DEC_ENABLE || defined ATMOS_CODEC_ENABLE)

			#ifdef ATMOS_CODEC_ENABLE
			case SPDIF_STREAM_MAT:
				dd_atmos_decocde_set();

				if (input_source == AUDIO_INPUT_RX_SOURCE_HDMI)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED

					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(2);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MClk:internal clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif

					audio_in_stream.sample_speed = audio_stream_speed_get(hdmi_infor.sample_rate);
					#endif
				}
				else
				{
					DBG_Assert(FALSE);
					DBG_Printf ("input source rx msg error12\n\r");
				}

				DBG_Printf ("SPDIF MAT\n\r");
				break;
			#else
			case SPDIF_STREAM_MAT:
				DBG_Printf ("SPDIF MAT No Soupport\n\r");
				break;
			#endif //ATMOS_CODEC_ENABLE

			#ifdef ATMOS_CODEC_ENABLE
			case SPDIF_STREAM_THD:
				dd_atmos_decocde_set();

				if (input_source == AUDIO_INPUT_RX_SOURCE_HDMI)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED
					hw_audio_mi2s_osr_div_set(2);
					hw_audio_mi2s_mclk_switch(TRUE);
					#endif
				}
				else
				{
					DBG_Assert(FALSE);
					DBG_Printf ("input source rx msg error12\n\r");
				}

				DBG_Printf ("SPDIF THD\n\r");
				break;
			#else
			case SPDIF_STREAM_THD:
				DBG_Printf ("SPDIF THD No Soupport\n\r");
				break;
			#endif //ATMOS_CODEC_ENABLE
			
			#ifdef DTS_DMP_DEC_EANBLE
			case SPDIF_STREAM_DTS:
				#ifdef DTS_DMP_DEC_ONLY
				dtsdmp_dec_set();
				#endif
				
				if (input_source == AUDIO_INPUT_RX_SOURCE_HDMI)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED
					
					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(0);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MCLK:interanl clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif //MCLK_IN_AS_AUDIO_OUT_CLK
					
					#endif //AUDIO_OUT_CLK_SET_FIXED
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_OPTICAL)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED
					
					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(0);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MCLK:interanl clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif //MCLK_IN_AS_AUDIO_OUT_CLK
					
					#endif //AUDIO_OUT_CLK_SET_FIXED
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_COAXIAL)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED
					
					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					hw_audio_mi2s_osr_div_set(0);
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MCLK:interanl clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif //MCLK_IN_AS_AUDIO_OUT_CLK
					
					#endif //AUDIO_OUT_CLK_SET_FIXED
				}
				else
				{
					DBG_Assert(FALSE);
					DBG_Printf ("input source rx msg error1!\n\r");
				}

				DBG_Printf ("SPDIF DTS\n\r");
				break;

			case SPDIF_STREAM_DTS_TYPE2:
				outsize = 0;
				DBG_Printf ("SPDIF DTS Type 2 No Soupport\n\r");
				break;

			case SPDIF_STREAM_DTS_TYPE3:
				outsize = 0;
				DBG_Printf ("SPDIF DTS Type 3 No Soupport\n\r");
				break;

			case SPDIF_STREAM_DTS_TYPE4:
				outsize = 0;
				DBG_Printf ("SPDIF DTS Type 4 No Soupport\n\r");
				break;
			#else
			case SPDIF_STREAM_DTS:
				outsize = 0;
				DBG_Printf ("SPDIF DTS Type 1 No Soupport\n\r");
				break;

			case SPDIF_STREAM_DTS_TYPE2:
				outsize = 0;
				DBG_Printf ("SPDIF DTS Type 2 No Soupport\n\r");
				break;

			case SPDIF_STREAM_DTS_TYPE3:
				outsize = 0;
				DBG_Printf ("SPDIF DTS Type 3 No Soupport\n\r");
				break;

			case SPDIF_STREAM_DTS_TYPE4:
				outsize = 0;
				DBG_Printf ("SPDIF DTS Type 4 No Soupport\n\r");
				break;
			#endif //DTS_DMP_DEC_EANBLE

			case SPDIF_STREAM_PCM:
				if (input_source == AUDIO_INPUT_RX_SOURCE_HDMI)
				{
					#ifdef HDMI_ENABLE
					
					#ifdef AUDIO_OUT_CLK_SET_FIXED
					audio_in_stream.sample_speed = audio_stream_speed_get(hdmi_infor.sample_rate);
					
					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					if (audio_in_stream.sample_speed == SAMPLE_SPEED_4X)
					{
						hw_audio_mi2s_osr_div_set(2);
					}
					else
					{
						hw_audio_mi2s_osr_div_set(0);
					}
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MClk:internal clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif

					#endif //AUDIO_OUT_CLK_SET_FIXED

		            hdmi_decocde_set();

					DBG_Printf ("hdmi in PCM!\n\r");
					#endif //HDMI_ENABLE
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_OPTICAL)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED

					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					if (audio_in_stream.sample_speed == SAMPLE_SPEED_4X)
					{
						hw_audio_mi2s_osr_div_set(2);
					}
					else
					{
						hw_audio_mi2s_osr_div_set(0);
					}
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MClk:internal clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif //MCLK_IN_AS_AUDIO_OUT_CLK

					#endif //AUDIO_OUT_CLK_SET_FIXED

					#ifdef OPTICAL_ENABLE 
		            optical_decocde_set();
					#endif //OPTICAL_ENABLE

					DBG_Printf ("optical spdif in PCM!\n\r");
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_COAXIAL)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED

					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					if (audio_in_stream.sample_speed == SAMPLE_SPEED_4X)
					{
						hw_audio_mi2s_osr_div_set(2);
					}
					else
					{
						hw_audio_mi2s_osr_div_set(0);
					}
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MClk:internal clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif //MCLK_IN_AS_AUDIO_OUT_CLK

					#endif //AUDIO_OUT_CLK_SET_FIXED

					#ifdef COAXIAL_ENABLE 
					coaxial_decocde_set();
					#endif //COAXIAL_ENABLE

					DBG_Printf ("coaxial spdif in PCM!\n\r");
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_ARC)
				{
					#ifdef AUDIO_OUT_CLK_SET_FIXED
					
					#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
					if (audio_in_stream.sample_speed == SAMPLE_SPEED_4X)
					{
						hw_audio_mi2s_osr_div_set(2);
					}
					else
					{
						hw_audio_mi2s_osr_div_set(0);
					}
					//MClk:external clock
					hw_audio_mi2s_mclk_switch(TRUE);
					#else
					hw_audio_mi2s_osr_div_set(4);
					//MClk:internal clock
					hw_audio_mi2s_mclk_switch(FALSE);
					#endif //MCLK_IN_AS_AUDIO_OUT_CLK
					
					#endif //AUDIO_OUT_CLK_SET_FIXED

					#ifdef ARC_ENABLE 
		            arc_decocde_set();
					#endif //ARC_ENABLE

					DBG_Printf ("arc in PCM!\n\r");
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_AUX)
				{
					DBG_Printf ("aux in PCM!\n\r");
				}
				else
				{
					DBG_Assert(FALSE);
					DBG_Printf ("input source rx msg error3!\n\r");
				}
				break;

			default:
				#ifdef SPDIF_ENABLE
				spdif_in_stream.data_type = SPDIF_STREAM_UNKNOWN;
				/*
				*app_timer_mute_on_time_set(25);
				*/
				DBG_Printf ("SPDIF Stream Unknown 1\n\r");
				if (input_source == AUDIO_INPUT_RX_SOURCE_OPTICAL)
				{
					/*
					* When the hdmi output is spdif, this option on for spdif detection
					* system_msgSend(UI_SPDIF_STREAM_UNDERFLOW);
					*
					**/
					asm("nop");
				}
				else if (input_source == AUDIO_INPUT_RX_SOURCE_COAXIAL)
				{
					/*
					* When the hdmi output is spdif, this option on for spdif detection
					* system_msgSend(UI_SPDIF_STREAM_UNDERFLOW);
					*
					**/
					asm("nop");
				}
				#endif
				
				return;
			}
			
			audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_WAITING;
		}

		if (outsize)
		{
			#ifdef SYNC_WORD_SWAP
			if (!spdif_stream_is_bigEndian())
			{
				int i;
				U32 *p = pout;

				/*
				note:frame size must be divisible by 2, and the start word of a frame ust be 0 (invalid data), 
				so even though it's possible go a half-word beyond the boundary, it doesn't matter.
				*/

				for (i=0; i<(outsize+3)/4; i++)
				{
					*p = AE_SHA32(*p);
					p++;
				}
			}
			#endif //SYNC_WORD_SWAP

			if(AUDIO_FIFO_STREAM_FREE_LEN() >= outsize)
			{
				size = (U32) AUDIO_FIFO_STREAM_PUT_DATA(pout, outsize);
				size = AUDIO_FIFO_STREAM_DATA_LEN();
				zero_action = audio_input_rx_wstream_callback (size);
			}
			else
			{
				DBG_Printf("overflow\r\n");
			}
		}

		debug_assert(insize >= consumed);
		insize -= consumed;
		
		if (insize == 0)
		{
			break;
		}
		else
		{
			pInBuf += consumed;
		}
		
	} while (1);
	#endif //(defined HDMI_ENABLE) || (defined OPTICAL_ENABLE) || (defined COAXIAL_ENABLE) || (defined ARC_ENABLE)
	
	#endif //SPDIF_ENABLE

	return;
}


void audio_input_rx_task_init(void)
{
#if (defined HDMI_ENABLE || defined OPTICAL_ENABLE || defined COAXIAL_ENABLE || defined ARC_ENABLE || defined AUX_ENABLE)
    int ret;
	
    ret = xTaskCreate(audio_input_rx_task, 
                    "audio input rx task", 
                    TASK_AUDIO_RCV_STACK_SIZE, 
                    NULL, 
                    TASK_AUDIO_RCV_PRIORITY,
                    NULL);

    if (ret != TRUE)
    {
        DBG_Assert(FALSE);
    }
#endif
}

