#include "os_config.h"

#if ((defined DOLBY_AC3_DEC_ENABLE) || (defined DOLBY_DDPLUS71_DEC_ENABLE) || (defined DTS_DMP_DEC_EANBLE))
#include "mem_reloc.h"

#include "hw_timer.h"

#include "sdk_com.h"
#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "optek_src.h"
#include "flash_boot.h"

#include "audio_dec_dolby.h"
#include "audio_dec_dolby_ddplus71.h"
#include "audio_dec_dtsdmp.h"


#define AUDIO_OUT_DATA_CHANNEL_SWITCH

//#define AUDIO_DATA_CHANNEL_CHECK_OUT

/*
*#if defined MI2S_OUTPUT_2CHANEL
*U32 optek_src_buf[(AUDIO_OUT_BUF_SIZE/4)*2*2]; //small out buf, MP3_DEC_FRAME_SIZE = 1152
*#elif defined MI2S_OUTPUT_4CHANEL
*U32 optek_src_buf[(AUDIO_OUT_BUF_SIZE/4)*4*2]; //small out buf, MP3_DEC_FRAME_SIZE = 1152
*#elif defined MI2S_OUTPUT_6CHANEL
*U32 optek_src_buf[(AUDIO_OUT_BUF_SIZE/4)*6*2]; //small out buf, MP3_DEC_FRAME_SIZE = 1152
*#elif defined MI2S_OUTPUT_8CHANEL
*U32 optek_src_buf[(AUDIO_OUT_BUF_SIZE/4)*8*2]; //small out buf, MP3_DEC_FRAME_SIZE = 1152
*#else
*U32 optek_src_buf[AUDIO_OUT_BUF_SIZE/4)*2*2]; //small out buf, MP3_DEC_FRAME_SIZE = 1152
*#endif
*/

/*
*FRAME_OUT_SIZE SUPPORT
*(AUDIO_OUT_BUF_SIZE)
*(AUDIO_OUT_BUF_SIZE/2)
*/
#define FRAME_OUT_SIZE    (AUDIO_OUT_BUF_SIZE)


extern BOOL uiDacMute;


void optek_src_init (void)
{

}


#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE

#ifdef DOLBY_AC3_DEC_ENABLE

#define AC3_DEC_FRAME_BYTE_PER_2_CH    (1024)

#define DMA_TX_INT_INTERVAL_US_TIME(tx_byte_each_time, sample_rate)   (((tx_byte_each_time/4)*1000000)/sample_rate) //us

#undef DMA_TX_INT_INTERVAL_MS_TIME

//(1024/4)/48kHz = 5.333ms
#define DMA_TX_INT_INTERVAL_MS_TIME    5333 //5.333ms/loop

#else

#define AC3_DEC_FRAME_BYTE_PER_2_CH    (1024)

#define DMA_TX_INT_INTERVAL_US_TIME(tx_byte_each_time, sample_rate)   (((tx_byte_each_time/4)*1000000)/sample_rate) //us

#undef DMA_TX_INT_INTERVAL_MS_TIME

//(1024/4)/48kHz = 5.333ms
#define DMA_TX_INT_INTERVAL_MS_TIME    5333 //5.333ms/loop

#endif //DOLBY_AC3_DEC_ENABLE


void clock_slave_sync_polling_for_codec_ddplus71(U32 sample_rate)
{
#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	if (clock_sync_cnt)
	{
		--clock_sync_cnt;
		if (clock_sync_cnt < ((20000*1000)/DMA_TX_INT_INTERVAL_US_TIME(AC3_DEC_FRAME_BYTE_PER_2_CH, sample_rate)))
		{
			if (clock_sync_cnt % ((2500*1000)/DMA_TX_INT_INTERVAL_US_TIME(AC3_DEC_FRAME_BYTE_PER_2_CH, sample_rate)) == 0)
			{
				#if 0
				spdif_clk_adaption_process_for_dolby_ac3();
				clock_sync_init();
				#else
				func_wo_param_msgSend(spdif_clk_adaption_process_for_dolby_ac3);
				func_wo_param_msgSend(clock_sync_init);
				#endif
				
				if (clock_sync_cnt == 0)
				{
					clock_sync_cnt = ((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(AC3_DEC_FRAME_BYTE_PER_2_CH, sample_rate))+
									((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(AC3_DEC_FRAME_BYTE_PER_2_CH, sample_rate));
				}
			}
		}
		else if(clock_sync_cnt == ((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(AC3_DEC_FRAME_BYTE_PER_2_CH, sample_rate)))
		{
			clock_sync_cnt = ((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(AC3_DEC_FRAME_BYTE_PER_2_CH, sample_rate))+
							((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(AC3_DEC_FRAME_BYTE_PER_2_CH, sample_rate));

			func_wo_param_msgSend(spdif_clk_adaption_process_for_dolby_ac3);
			func_wo_param_msgSend(clock_sync_init);
		}
	}
#endif //CLOCK_SYNC_WITH_MASTER_ENABLE
}

#endif //CLOCK_SYNC_WITH_MASTER_ENABLE



void dec_direct_out_for_xa_codec( short *pBuf, U16 len ) __INTERNAL_RAM_TEXT;
void dec_direct_out_for_xa_codec( short *pBuf, U16 len )
{
    U16 out_len = len;

	#ifndef AUDIO_OUT_DATA_CHANNEL_SWITCH

	CFasm_memcpy(audio_task_env.ouput_buffer[audio_task_env.index], pBuf, out_len);
	
	audio_output_put_next_frame_dataPtr(audio_task_env.ouput_buffer[audio_task_env.index], out_len/4);

    #ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
    clock_slave_sync_polling_for_spdif();
    #endif

	EVENT_AUDIO_DECODE_GET(global_vari.eventGroup, pdMS_TO_TICKS(portMAX_DELAY));
	audio_task_env.index = !audio_task_env.index;

	#else
	
    U32 *pSrc;
    U32 *pDest;
    U16 in_sample_speed;
    U16 in_channel;
	
	in_sample_speed = audio_out_stream.sample_speed;
	in_channel = audio_out_stream.decode_out_channel;
	pSrc = pBuf;

	int decode_type = decode_type_get();

	if (in_channel < AUDIO_OUT_STREAM_CHANNEL)
	{
		#ifdef DOLBY_AC3_DEC_ENABLE
		if (decode_type == DECODE_DD_AC3)
		{
		    pDest = dd_ac3_dec_env.rx_postProcessBuf;
		}
		else
		#endif
		#ifdef DTS_DMP_DEC_EANBLE
		if (decode_type == DECODE_DTS_DMP)
		{
		    pDest = dtstmp_dec_env.rx_postProcessBuf;
		}
		else
		#endif
		#ifdef DOLBY_DDPLUS71_DEC_ENABLE
		if (decode_type == DECODE_DDPLUS71)
		{
		    pDest = ddplus71_dec_env.rx_postProcessBuf;
		}
		else
		#endif
		{
			DBG_Assert(FALSE);
		}

		audio_out_data_channel_switch(pSrc, pDest, in_sample_speed, in_channel, len, &out_len);
		CFasm_memcpy(audio_task_env.ouput_buffer[audio_task_env.index], pDest, out_len);
	}
	else
	{
		CFasm_memcpy(audio_task_env.ouput_buffer[audio_task_env.index], pSrc, out_len);
	}

	audio_output_put_next_frame_dataPtr(audio_task_env.ouput_buffer[audio_task_env.index], out_len/4);

    #ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
    clock_slave_sync_polling_for_spdif();
    #endif

	EVENT_AUDIO_DECODE_GET(global_vari.eventGroup, pdMS_TO_TICKS(portMAX_DELAY));	
	audio_task_env.index = !audio_task_env.index;

	#endif
}

void dec_direct_out_32bit( int *pBuf, U16 len ) __INTERNAL_RAM_TEXT;
void dec_direct_out_32bit( int *pBuf, U16 len )
{
	short *pO = (short *)pBuf;
	int *pI = pBuf;
	int i;

	for (i = 0; i < len /4; i++)
	{
		*pO++ = (*pI++)>>16;
	}

	dec_direct_out_for_xa_codec((short *)pBuf, len/2);
}


U8 optek_src_output(U16 *buf, U32 byte_len, U8 out_channel) __INTERNAL_RAM_TEXT;
U8 optek_src_output(U16 *buf, U32 byte_len, U8 out_channel)
{
	int size = ((byte_len + AUDIO_OUT_BUF_SIZE - 1)/AUDIO_OUT_BUF_SIZE)*AUDIO_OUT_BUF_SIZE/out_channel;

	#if 1//def DD_AC3_DEC_16BIT_OUT
	if (byte_len > AUDIO_OUTPUT_BUF_SIZE )
	{
		//overflow
		DBG_Assert(FALSE);
	}

	#if 0
	static U16 dbg_cnt = 0;
	dbg_cnt++;
	if (dbg_cnt >= 100)
	{
		dbg_cnt = 0;
		DBG_Printf("O 0x%x\n\r", byte_len);
	}
	#endif

	if (size > (AUDIO_OUT_BUF_SIZE) )
	{
		U16 slice, slice_len, src, i;
		U16 o_len;
		U32 *pU32Buf = buf;

		src = 0x01;

		#if 0
		slice = size/AUDIO_OUT_BUF_SIZE;
		slice_len = AUDIO_OUT_BUF_SIZE;
		#endif

		if (size >= 1024*4)
		{
			slice = 4;
			slice_len = 4;
		}
		else
		{
			slice = 1;
			slice_len = byte_len;
		}


		for (i=0; i<slice; i++)
		{
			dec_direct_out_for_xa_codec(buf, slice_len);

			if (audio_task_env.decoding_flag == TRUE)
			{
				return FALSE;
			}
		}
	}
	else
	{		
		dec_direct_out_for_xa_codec(buf, byte_len);
	}
	#endif //DD_AC3_DEC_16BIT_OUT

	return TRUE;

}

void audio_data_channel_check_out(void)
{
#ifdef AUDIO_DATA_CHANNEL_CHECK_OUT
	//for testing channals 
	short * p = audio_task_env.ouput_buffer[audio_task_env.index];
	
	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = 0x1000;
		*p++ = 0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = -0x1000;
		*p++ = -0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = 0x1000;
		*p++ = 0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = -0x1000;
		*p++ = -0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}
	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = 0x1000;
		*p++ = 0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = -0x1000;
		*p++ = -0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}
	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = 0x1000;
		*p++ = 0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = -0x1000;
		*p++ = -0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}
#endif //AUDIO_DATA_CHANNEL_CHECK_OUT
}
#else
U8 optek_src_output(U16 *buf, U32 byte_len, U8 out_channel)
{

}
#endif //DOLBY_AC3_DEC_ENABLE
