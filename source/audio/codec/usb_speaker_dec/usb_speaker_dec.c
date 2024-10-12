#include "os_config.h"

#ifdef USB_SPEAKER
#include "hw_audio_i2s.h"

#include "fifo.h"

#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "clock_slave_sync.h"
#include "audio_input_receive_task.h"
#include "codec_malloc.h"
#include "usb_speaker_dec.h"
#include "audio_usb_speaker_input.h"

#ifdef AUDIO_CLOCK_SYNC_ENABLE
#include "audio_clock_sync.h"
#endif

typedef struct {
    U8 *read_buffer[2];
    U8 *rx_postProcessBuf;
    U16 discard_frame_cnt;//discard same frame when decoding begains
    U8 first_frame;//for sync
    U8 index;
} USBDEV_DEC_ENV_STRU;

USBDEV_DEC_ENV_STRU usbdev_dec_env;

#ifdef USB_DEV_UPDATA_MIC_ENABLE
U8 *usbdev_temp_buf_8bit_48k;
#endif
U16 decode_delay_exit_count;
void usbdev_dec_init(void)
{
    int size;
    U8 *pStreamBuf;
    U32 len;

	DBG_Printf("%s\n\r", __func__);

    memset((void *)&usbdev_dec_env, 0, sizeof(USBDEV_DEC_ENV_STRU));


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
	#endif
	#endif //AUDIO_OUT_CLK_SET_FIXED

	decode_delay_exit_count = 0;

	audio_in_stream.sample_rate = 48000;
	audio_in_stream.sample_speed = SAMPLE_SPEED_1X;

	//audio_in_stream.channel = AUDIO_IN_STREAM_CHANNEL;
	audio_in_stream.sample_rate = AUDIO_IN_STREAM_SAMPLE_RATE;
	audio_in_stream.sample_speed = audio_stream_speed_get(audio_in_stream.sample_rate);


	audio_clock_freq_set(audio_in_stream.sample_rate);

	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

    DBG_Assert(usbdev_dec_env.read_buffer[0] == NULL);
    DBG_Assert(usbdev_dec_env.read_buffer[1] == NULL);

	size = (USB_PCM_BLOCK_SIZE*AUDIO_OUTPUT_CHANNEL*2);
    usbdev_dec_env.read_buffer[0] = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

	size = (USB_PCM_BLOCK_SIZE*AUDIO_OUTPUT_CHANNEL*2);
    usbdev_dec_env.rx_postProcessBuf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

	#ifdef USB_DEV_UPDATA_MIC_ENABLE
    size = 192*2*2*4;
    usbdev_mic_in_fifo_buf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_4);
    usbdev_up_fifo_buf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_4);
    memset(usbdev_mic_in_fifo_buf, 0, size);
    memset(usbdev_up_fifo_buf, 0, size);

	fifo_init (&MicInFifo, (U8 *)usbdev_mic_in_fifo_buf, size, TRUE);	
	fifo_init (&UsbDevUpFifo, (U8 *)usbdev_up_fifo_buf, size, TRUE);
	
	size = (USB_PCM_BLOCK_SIZE*2*2);
	usbdev_temp_buf_8bit_48k = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
	#endif

    usbdev_dec_env.discard_frame_cnt = 1;
    usbdev_dec_env.first_frame = 5;

	#if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE)// && defined IIS_IN_SYNC_W_MASTER)
	//only iis in slave role need source sync
	sync_w_src_start();

	audio_clock_sync_vari.need_sync_flag = 40;//TRUE;
	#endif



    #ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	clock_slave_sync_init(USB_PCM_BLOCK_SIZE, audio_in_stream.sample_rate);
    #endif
}

void usbdev_dec_fini(int reason)
{
    DBG_Printf("%s\n\r", __func__);

	audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_WAITING;
    audio_usb_speaker_input_receive_stop_req();

    usbdev_dec_env.read_buffer[0] = NULL;
    usbdev_dec_env.read_buffer[1] = NULL;
}

#ifdef IIS_IN_SYNC_W_MASTER
U16 reference_point;
U16 current_point;
#endif

int usbdev_dec_info_get(DECODE_INFO_STRU *info)
{
	#ifdef IIS_IN_SYNC_W_MASTER
	I32 dritf_samples;

    DBG_Printf("%s\n\r", __func__);
 
	dritf_samples = current_point - reference_point;

	DBG_Printf("drift samples:%d,%d,%d\n", dritf_samples, current_point, reference_point);

	if (dritf_samples > 10)
		dritf_samples = 10;

	if (dritf_samples < -10)
    	dritf_samples = -10;

		info->src_speed = dritf_samples;

	#elif defined AUDIO_CLOCK_SYNC_ENABLE
	U32 dlen;
	I32 drift;

	if (audio_clock_sync_vari.need_sync_flag == FALSE)
	{
		dlen = AUDIO_FIFO_STREAM_DATA_LEN();

		drift = dlen-EX_AUDIO_STREAM_THRESHOLD_FOR_USB_DEV;
	}
	else
	{
		drift = 0;
	}


	DBG_Printf("drift bytes:%d\n",drift);

	info->src_speed = (I8)(drift/192);

	return 1;


	#else

	info->src_speed = 0;

	#endif

	return 1;
}

/*********************************************************************************************
 *
 * |__rx dummy__|_________rx N+2_________|_________rx N+3_________|...
 *
 *  get N,       process N+1,get N+1,     process N+2,get N+2,     process N+3,...
 * |__________tx 1__________|__________tx 2__________|__________tx 3__________|...
 *
 * process:decode->mix1->dsp->mix2
 *
 **********************************************************************************************/

int usbdev_dec_decode_frame(U8 **pout, U16 *plen)
{
    U8 *pSrc;
    U8 *pDest;
    U32 dlen;
    U32 len, out_len;
    U32 sample_rate = audio_in_stream.sample_rate;
    int in_sample_speed;
    int in_channel;
	int ret;



    in_sample_speed = audio_in_stream.sample_speed;
    in_channel = audio_in_stream.channel;
	if (in_channel <= 0)
	{
		in_channel = 2;
	}

    dlen = AUDIO_FIFO_STREAM_DATA_LEN();

	#if 0//printf
	static U16 printf_dlen_count = 0;
	if (printf_dlen_count++ > 100)
	{
		printf_dlen_count = 0;
		DBG_Printf ("dlen=%d\n\r", dlen);
	}
	#endif


    if (0)
       len = USB_PCM_BLOCK_SIZE;
    else
       len = (USB_PCM_BLOCK_SIZE/2)*audio_in_stream.channel;


    #ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
    clock_slave_sync_polling_for_spdif(sample_rate, USB_PCM_BLOCK_SIZE);
    #endif

	#if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE)// && defined IIS_IN_SYNC_W_MASTER)
	if (audio_clock_sync_vari.need_sync_flag)
	{
		if (audio_clock_sync_vari.need_sync_flag == 1)
		{
			if (dlen > (EX_AUDIO_STREAM_THRESHOLD_FOR_USB_DEV - USB_AUDIO_DEC_FRAME_SIZE)*4)
			{
				DBG_printf("synced\r\n");
				audio_clock_sync_vari.need_sync_flag = FALSE;

				sync_w_src_start();

				AUDIO_FIFO_STREAM_DISCARD_DATA(dlen - (EX_AUDIO_STREAM_THRESHOLD_FOR_USB_DEV - USB_AUDIO_DEC_FRAME_SIZE));
			}
		}
		else
		{
			audio_clock_sync_vari.need_sync_flag--;
		}

		memset(usbdev_dec_env.rx_postProcessBuf, 0, USB_AUDIO_DEC_FRAME_SIZE);
		*pout = usbdev_dec_env.rx_postProcessBuf;
		*plen = USB_AUDIO_DEC_FRAME_SIZE;

		return DECODE_SUCCESS;
	}
	#endif

    pSrc = usbdev_dec_env.read_buffer[0];
    pDest = usbdev_dec_env.rx_postProcessBuf;


	if (dlen >= len)
	{
		AUDIO_FIFO_STREAM_GET_DATA(pSrc, len);
		decode_delay_exit_count = 0;
	}
	else
	{
		//If using Media Player, dlen = (USB_PCM_BLOCK_SIZE/2)
		//If using Edge, dlen = (USB_PCM_BLOCK_SIZE*3/4) or (USB_PCM_BLOCK_SIZE/4)
		DBG_Printf ("usb speaker Rx Underflow,dlen=%d\n\r", dlen);

		#if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE)// && defined IIS_IN_SYNC_W_MASTER)
		audio_clock_sync_vari.need_sync_flag = TRUE;
		#endif

		system_msgSend(UI_SPDIF_STREAM_UNDERFLOW);
		memset(pSrc, 0, len);
		if (++decode_delay_exit_count > 10)
		{
			decode_delay_exit_count = 0;
			return DECODE_END;
		}
		
	}
	
    #ifdef USB_DEV_UPDATA_MIC_ENABLE
	if (usbdev_mic_in_fifo_buf == NULL)
	{
		goto USB_DEV_UPDATA_MIC_EXIT;
	}
	int data_Bytelen = len/(audio_in_stream.channel/2);
	int readlen = fifo_get_data_len(&MicInFifo);
	int getlen = 0;

	if (readlen > data_Bytelen*2)
	{
		getlen = fifo_get_data(&MicInFifo, usbdev_temp_buf_8bit_48k, data_Bytelen);
	}
	
	if (getlen != data_Bytelen)
	{

		int free_space_len = fifo_get_free_space(&UsbDevUpFifo);
		if (free_space_len < data_Bytelen)
		{
			//DBG_iPrintf("overflow\n\r");
		}
		memset(usbdev_temp_buf_8bit_48k, 0, data_Bytelen);

		int putlen = fifo_put_data(&UsbDevUpFifo, usbdev_temp_buf_8bit_48k, data_Bytelen);
	}
	else
	{

		int free_space_len = fifo_get_free_space(&UsbDevUpFifo);
		if (free_space_len < data_Bytelen)
		{
			//DBG_iPrintf("overflow\n\r");
		}

		int putlen = fifo_put_data(&UsbDevUpFifo, usbdev_temp_buf_8bit_48k, data_Bytelen);

		if (putlen != data_Bytelen)
		{
			//DBG_iPrintf("%d,%d\n\r",putlen,data_Bytelen);
		}
	}
	USB_DEV_UPDATA_MIC_EXIT:
	#endif	

	#ifdef AUDIO_OUT_CHANNEL_SET_FIXED
	audio_out_data_channel_switch(pSrc, pDest, in_sample_speed, in_channel, len, &out_len);
    *pout = pDest;
    *plen = out_len;
    #else
    *pout = pSrc;
    *plen = len;    
	#endif //AUDIO_OUT_CHANNEL_SET_FIXED

    return DECODE_SUCCESS;

}

static const DECODE_DEVICE_STRU usbdev_decode =
{
    DECODE_PCM,
	"usbdev decode",
	usbdev_dec_init,
	usbdev_dec_fini,
	usbdev_dec_decode_frame,
	usbdev_dec_info_get
};


void usbdev_decocde_set(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_set_msgSend(&usbdev_decode);
}

void usbdev_decocde_start_w_init(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_start_w_init_msgSend();
}

void usbdev_decocde_set_from_isr(void)
{
    DBG_iPrintf("%s\r\n", __func__);
    decode_set_msgSend_from_isr(&usbdev_decode);
}

void usbdev_decocde_start_w_init_from_isr(void)
{
    DBG_iPrintf("%s\r\n", __func__);
    decode_start_w_init_msgSend_from_isr();
}

#endif //USB_ENABLE
