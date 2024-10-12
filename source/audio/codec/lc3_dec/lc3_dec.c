#include "os_config.h"

#include "hw_pll.h"
#include "hw_timer.h"

#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "audio_input_receive_task.h"
#include "audio_aux_input.h"

#include "codec.h"
#include "xa_type_def.h"
#include "xa_error_standards.h"


#ifdef LC3_DECODE_ENABLE
XA_ERRORCODE xa_lc3_dec_handle_init(void **decode_handle,U32 bitrate,U16 samplerate,U8 channels,float frame_Time_ms);
XA_ERRORCODE xa_lc3_dec_frame (void *decode_handle,U8 *pIn,U8 inLen,U8 **pout_buf, U16 *out_byte_len);

#define MAX_LC3_DECODER 2
#define MAX_CONCEALMENT_CNT 5

#define DECODE_FRAME_SZIE (240*4)//LC3 5MS

typedef struct {
	void *decode_hanle[MAX_LC3_DECODER];
    U8 *rx_postProcessBuf;
    U16 discard_frame_cnt; //discard some frames when decoding start
    U8 first_frame;        //for sync
    U8 index;
	U8 loss_cnt[MAX_LC3_DECODER];
} LC3_DEC_ENV_STRU;

LC3_DEC_ENV_STRU lc3_dec_env = {0};
 
void lc3_dec_init(void)
{
   	int size;

	//The audio in information is determined by the optek link protocol

	hw_audio_mi2s_in_channel_set(audio_in_stream.channel);

	audio_out_stream.channel = AUDIO_OUT_STREAM_CHANNEL;
	hw_audio_mi2s_out_channel_set(audio_out_stream.channel);

	audio_clock_freq_set(audio_in_stream.sample_rate);

	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

	if (audio_in_stream.sample_rate == 44100)
	{
		if (xa_lc3_dec_handle_init(&lc3_dec_env.decode_hanle[0], 158760, audio_in_stream.sample_rate, 2, 5.0) != XA_NO_ERROR)
		{
			DBG_Printf("rx1 lc3 dec init error\n\r");
		}
	}
	else
	{
		if (xa_lc3_dec_handle_init(&lc3_dec_env.decode_hanle[0], 96*8*200/*160000*/, audio_in_stream.sample_rate, 2, 5.0) != XA_NO_ERROR)
		{
			DBG_Printf("rx1 lc3 dec init error\n\r");
		}
	}

	lc3_dec_env.loss_cnt[0] = MAX_CONCEALMENT_CNT;

	size = (DECODE_FRAME_SZIE);
	lc3_dec_env.rx_postProcessBuf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
}

void lc3_dec_fini(int reason)
{
	DBG_Printf("%s", __func__);

}

int lc3_dec_info_get(DECODE_INFO_STRU *info)
{
	info->src_speed = 0;

	return 1;
}

/*******************************************************************************************************************
 * 
 * 
 *******************************************************************************************************************/


int lc3_dec_decode_frame(U8 **pout, U16 *plen)
{	

	U8 inLen;
	U8 *p_rx;
	U8 *pDecOut;
	U16 outlen;

	inLen = get_lc3_decode_frame_bytes(lc3_dec_env.decode_hanle[0]);
	inLen = dec_get_optek_link_rx_data(&p_rx,1,inLen);

	if (inLen == 0)
	{
		if (lc3_dec_env.loss_cnt[0] < MAX_CONCEALMENT_CNT)
			lc3_dec_env.loss_cnt[0]++;
	}
	else if (lc3_dec_env.loss_cnt[0] != 0)
	{
		lc3_dec_env.loss_cnt[0] = 0;
	}
	
	if (lc3_dec_env.loss_cnt[0] < MAX_CONCEALMENT_CNT)
		xa_lc3_dec_frame(lc3_dec_env.decode_hanle[0], p_rx, inLen, &pDecOut, &outlen);
	else
	{
		outlen = DECODE_FRAME_SZIE;
		pDecOut = lc3_dec_env.rx_postProcessBuf;
		memset(pDecOut,0,outlen);
	}		

#if 0

	if (len != dec_frame_size)
	{
		DBG_Printf("len err:%d\r\n",len);
		//len = dec_frame_size;
		//pOut = pDacbuf;
		//memset(pOut,0,len);					
	}

	if (spk_out_sync_w_tx_status != SPK_AND_TX_SYNCED && spk_out_sync_w_tx_status != SPK_AND_TX_NOTSYNC)
	{
		memset(pOut,0,len);
	}
	else
	{
		/********  put upstream audio to usb uplink****************/
		/*
		if (fifo_put_data(&AdcInFifo, pOut,len) != len)
		{
			//DBG_Printf("usbdev upstream overflow\r\n");
		}
		*/
	}
#endif

	*pout = pDecOut;
	*plen = outlen;

	return DECODE_SUCCESS;
}

static const DECODE_DEVICE_STRU lc3_decode = 
{
    DECODE_LC3,
	"lc3 decode",	
	lc3_dec_init,
	lc3_dec_fini,
	lc3_dec_decode_frame,
    lc3_dec_info_get
};

void lc3_decocde_set(void)
{
    //DBG_Printf("%s\r\n", __func__);
    decode_set_msgSend(&lc3_decode);
}

#endif