#include "os_config.h"

#ifdef HDMI_ENABLE
#include "hw_pll.h"

#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "fifo.h"
#include "tools.h"
#include "codec.h"
#include "audio_input_receive_task.h"
#include "audio_hdmi_input.h"
#include "hdmi_dec.h"

#ifdef SPDIF_ENABLE
#include "spdif_process.h"
#endif

#include "clock_slave_sync.h"


#define HDMI_AUDIO_OUT_FRAME_FIXED

//#define HDMI_DEC_FINISH_MSG_SEND

//#define HDMI_UNDERFLOW_MSG_SEND

#define PEAKLEVEL_ENABLE

#ifdef PEAKLEVEL_ENABLE
#include "peak_level_meter.h"
#endif


//#define RTOS_MALLOC_USED

#define MAX_NOISE_GAIN        128


typedef struct {
    U8 * read_buffer[2];
    U8 * rx_postProcessBuf;
    U16 discard_frame_cnt; //discard same frame when decoding begains
    U8 first_frame;        //for sync
    U8 index;
} HDMI_DEC_ENV_STRU;

HDMI_DEC_ENV_STRU hdmi_dec_env;


//for pcm decoding
void hdmi_dec_init(void)
{
#ifdef AUDIO_IN_STREAM_USED
    int size;
    U8 *pStreamBuf;
    U32 len;
	U32 sample_rate = audio_in_stream.sample_rate;

	DBG_Printf("%s\n\r", __func__);

    memset((void *)&hdmi_dec_env, 0, sizeof(hdmi_dec_env));

    DBG_Assert(hdmi_dec_env.read_buffer[0] == NULL);
    DBG_Assert(hdmi_dec_env.read_buffer[1] == NULL);

	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

	size = (HDMI_PCM_BLOCK_SIZE*4*4);
    hdmi_dec_env.read_buffer[0] = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
    /*
	*hdmi_dec_env.read_buffer[1] = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
	*/

	size = (HDMI_PCM_BLOCK_SIZE*4*4);

    hdmi_dec_env.rx_postProcessBuf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

    hdmi_dec_env.discard_frame_cnt = 1;
    hdmi_dec_env.first_frame = 5;

	#if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE && defined IIS_IN_SYNC_W_MASTER)
	//only iis in slave role need source sync
	sync_w_src_start();
	#endif


 	#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	clock_slave_sync_init(HDMI_PCM_BLOCK_SIZE, sample_rate);
    #endif
#else
    memset((void *)&spdif_dec_env, 0, sizeof(spdif_dec_env));

    DBG_Assert(spdif_dec_env.read_buffer[0] == NULL);
    DBG_Assert(spdif_dec_env.read_buffer[1] == NULL);//memory leak?

    hdmi_dec_env.read_buffer[0] = codec_malloc_w_memtype(HDMI_PCM_BLOCK_SIZE);
    hdmi_dec_env.read_buffer[1] = codec_malloc_w_memtype(HDMI_PCM_BLOCK_SIZE);

 	AUDIO_INPUT_RX_EVENT_CLR;

    audio_spdif_input_rx_init();

    spdif_dec_env.discard_frame_cnt = 1;
    spdif_dec_env.first_frame = 5;

	#if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE && defined IIS_IN_SYNC_W_MASTER)
	//only iis in slave role need source sync
	sync_w_src_start();
	#endif

#endif //AUDIO_IN_STREAM_USED
}

void hdmi_dec_fini(int reason)
{
    DBG_Printf("%s\n\r", __func__);
    
 #ifdef AUDIO_IN_STREAM_USED
    hdmi_dec_env.read_buffer[0] = NULL;
    hdmi_dec_env.read_buffer[1] = NULL;
#else
    audio_hdmi_input_rx_stop_req();

    hdmi_dec_env.read_buffer[0] = NULL;
    hdmi_dec_env.read_buffer[1] = NULL;
#endif

    #ifdef HDMI_DEC_FINISH_MSG_SEND
    system_msgSend(UI_DECODE_FINISH);
    #else
    audio_in_stream.status = STREAM_DECOCDE_FINISH;
    #endif

	/*
	*decode_exit_req_set();
	*/
}

#ifdef IIS_IN_SYNC_W_MASTER
U16 reference_point;
U16 current_point;
#endif

int hdmi_dec_info_get(DECODE_INFO_STRU *info)
{
#ifdef IIS_IN_SYNC_W_MASTER
    I32 dritf_samples;

    dritf_samples = current_point - reference_point;

    DBG_Printf("drift samples:%d,%d,%d\n",dritf_samples,current_point,reference_point);

    if (dritf_samples > 10)
	{
        dritf_samples = 10;
	}
    else if (dritf_samples < -10)
	{
        dritf_samples = -10;
	}

    info->src_speed = dritf_samples;

#else
	
	info->src_speed = 0;
	
#endif

	return 1;
}

/*******************************************************************************************************************
 * 
 * |__rx dummy__|_________rx N+2_________|_________rx N+3_________|...
 * 
 *  get N,       process N+1,get N+1,     process N+2,get N+2,     process N+3,...
 * |__________tx 1__________|__________tx 2__________|__________tx 3__________|...
 * 
 * process:decode->mix1->dsp->mix2
 * 
 *******************************************************************************************************************/

int hdmi_dec_decode_frame(U8 **pout, U16 *plen)
{
#ifdef AUDIO_IN_STREAM_USED
    U8 *pSrc;
    U8 *pDest;
    U32 dlen;
    U16 len, out_len;
    U32 sample_rate = audio_in_stream.sample_rate;
    int in_sample_speed;
    int in_channel;
	int ret;

	#ifdef DEBUG_GPIO_PIN0_ENABLE
    static int debug_index = 0;
	debug_index++;
	if (debug_index & 0x01)
	{
		DBG_PIN_HIGH(DEBEG_PIN);
	}
	else
	{
		DBG_PIN_LOW(DEBEG_PIN);
	}
	#endif

    in_sample_speed = audio_in_stream.sample_speed;
	in_channel = audio_in_stream.channel;
	if (in_channel <= 0)
	{
		in_channel = 2;
	}

    dlen = AUDIO_FIFO_STREAM_DATA_LEN();
  
    #ifdef HDMI_AUDIO_OUT_FRAME_FIXED
    if (in_sample_speed == SAMPLE_SPEED_4X)
    {
        len = (HDMI_PCM_BLOCK_SIZE*4)*(in_channel/2);
	}
    else if (in_sample_speed == SAMPLE_SPEED_2X)
    {
        len = (HDMI_PCM_BLOCK_SIZE*2)*(in_channel/2);
    }
    else
    { 
        len = (HDMI_PCM_BLOCK_SIZE)*(in_channel/2);
	}
	#else
    len = HDMI_PCM_BLOCK_SIZE;
	#endif

	#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	clock_slave_sync_polling_for_spdif(sample_rate, len);
	#endif

	pSrc = hdmi_dec_env.read_buffer[0];
    pDest = hdmi_dec_env.rx_postProcessBuf;

    if (dlen >= HDMI_PCM_BLOCK_SIZE)
    {
        AUDIO_FIFO_STREAM_GET_DATA(pSrc, len);
    }
    else
    {
        DBG_Printf ("Hdmi Rx Underflow1\n\r");

	    #ifdef HDMI_UNDERFLOW_MSG_SEND
        system_msgSend(UI_SPDIF_STREAM_UNDERFLOW);
        #else
        decode_exit_msgSend();
        #endif
		
        return DECODE_END;
    }

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

#endif //AUDIO_IN_STREAM_USED

static const DECODE_DEVICE_STRU hdmi_decode =
{
    DECODE_PCM,
    "hdmi pcm decode",
    hdmi_dec_init,
    hdmi_dec_fini,
    hdmi_dec_decode_frame,
    hdmi_dec_info_get
};

void hdmi_decocde_set(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_set_msgSend(&hdmi_decode);
}

void hdmi_decocde_start_w_init(void)
{
	DBG_Printf("hdmi dec start1\n\r");
    decode_start_w_init_msgSend();
}

void hdmi_decocde_start(void)
{
	DBG_Printf("hdmi dec start2\n\r");
    decode_start_msgSend();
}

#endif //HDMI_ENABLE
