#include "os_config.h"

#ifdef COAXIAL_ENABLE
#include "hw_pll.h"

#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "fifo.h"
#include "tools.h"
#include "codec.h"

#include "audio_input_receive_task.h"
#include "audio_coaxial_input.h"

#ifdef SPDIF_ENABLE
#include "spdif_process.h"
#endif

#include "coaxial_dec.h"
#include "clock_slave_sync.h"

#define PEAKLEVEL_ENABLE

#ifdef PEAKLEVEL_ENABLE
#include "peak_level_meter.h"
#endif


#ifdef DEBUG_GPIO_PIN0_ENABLE
#undef DEBUG_GPIO_PIN0_ENABLE
#endif

#define COAXIAL_AUDIO_OUT_FRAME_FIXED

#define COAXIAL_DEC_FINISH_MSG_SEND

#define COAXIAL_UNDERFLOW_MSG_SEND

//#define RTOS_MALLOC_USED


#define MAX_NOISE_GAIN        128


typedef struct {
    U8 * read_buffer[2];
    U8 * rx_postProcessBuf;
    U16 discard_frame_cnt; //discard same frame when decoding begains
    U8 first_frame;        //for sync
    U8 index;
} COAXIAL_DEC_ENV_STRU;

COAXIAL_DEC_ENV_STRU coaxial_dec_env;


//for pcm decoding
void coaxial_dec_init(void)
{
#ifdef AUDIO_IN_STREAM_USED
    int len;
    int size;
    U32 sample_rate = audio_in_stream.sample_rate;

    DBG_Printf("%s\n\r", __func__);

    memset((void *)&coaxial_dec_env, 0, sizeof(coaxial_dec_env));

    DBG_Assert(coaxial_dec_env.read_buffer[0] == NULL);
    DBG_Assert(coaxial_dec_env.read_buffer[1] == NULL);

    codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

    size = (SPDIF_PCM_BLOCK_SIZE*4*4);
    coaxial_dec_env.read_buffer[0]  = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
    /*
	*coaxial_dec_env.read_buffer[1]  = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
	*/

    size = (SPDIF_PCM_BLOCK_SIZE*4*4);
    coaxial_dec_env.rx_postProcessBuf  = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

    coaxial_dec_env.discard_frame_cnt = 1;
    coaxial_dec_env.first_frame = 5;

    #if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE && defined IIS_IN_SYNC_W_MASTER)
    //only iis in slave role need source sync
    sync_w_src_start();
    #endif


    #ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
    clock_slave_sync_init(SPDIF_PCM_BLOCK_SIZE, sample_rate);
    #endif
#else
    memset((void *)&coaxial_dec_env, 0, sizeof(coaxial_dec_env));

    DBG_Assert(coaxial_dec_env.read_buffer[0] == NULL);
    DBG_Assert(coaxial_dec_env.read_buffer[1] == NULL);//memory leak?

    coaxial_dec_env.read_buffer[0] = codec_malloc_w_memtype(SPDIF_PCM_BLOCK_SIZE);
    coaxial_dec_env.read_buffer[1] = codec_malloc_w_memtype(SPDIF_PCM_BLOCK_SIZE);

    EVENT_AUDIO_INPUT_RX_CLR;

    audio_coaxial_input_rx_init();

    coaxial_dec_env.discard_frame_cnt = 1;
    coaxial_dec_env.first_frame = 5;

    #if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE && defined IIS_IN_SYNC_W_MASTER)
    //only iis in slave role need source sync
    sync_w_src_start();
    #endif

#endif //AUDIO_IN_STREAM_USED
}

void coaxial_dec_fini(int reason)
{
    DBG_Printf("%s\n\r", __func__);
    
#ifdef AUDIO_IN_STREAM_USED
    audio_coaxial_input_rx_stop_req();

    coaxial_dec_env.read_buffer[0] = NULL;
    coaxial_dec_env.read_buffer[1] = NULL;
#else
    audio_coaxial_input_rx_stop_req();

    coaxial_dec_env.read_buffer[0] = NULL;
    coaxial_dec_env.read_buffer[1] = NULL;
#endif

    #ifdef COAXIAL_DEC_FINISH_MSG_SEND
    system_msgSend(UI_DECODE_FINISH);
    #else
    audio_in_stream.status = STREAM_DECOCDE_FINISH;
    #endif

}

#ifdef IIS_IN_SYNC_W_MASTER
U16 reference_point;
U16 current_point;
#endif

int coaxial_dec_info_get(DECODE_INFO_STRU *info)
{
	#ifdef IIS_IN_SYNC_W_MASTER
	I16 dritf_samples;

	dritf_samples = current_point - reference_point;

	DBG_Printf("drift samples:%d,%d,%d\n",dritf_samples,current_point,reference_point);

    if (dritf_samples > 10)
	{
        dritf_samples = 10;
	}

    if (dritf_samples < -10)
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

int coaxial_dec_decode_frame(U8 **pout, U16 *plen)
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

    #ifdef COAXIAL_AUDIO_OUT_FRAME_FIXED
    if (in_sample_speed == SAMPLE_SPEED_4X)
    {
	    len = (SPDIF_PCM_BLOCK_SIZE*4)*(in_channel/2);
    }
    else if (in_sample_speed == SAMPLE_SPEED_2X)
    {
        len = (SPDIF_PCM_BLOCK_SIZE*2)*(in_channel/2);
    }
    else
    { 
        len = (SPDIF_PCM_BLOCK_SIZE)*(in_channel/2);
    }
    #else
    len = SPDIF_PCM_BLOCK_SIZE;
    #endif

    #ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
    clock_slave_sync_polling_for_spdif(sample_rate, len);
    #endif

    pSrc = coaxial_dec_env.read_buffer[0];
    pDest = coaxial_dec_env.rx_postProcessBuf;

    if (dlen >= SPDIF_PCM_BLOCK_SIZE)
    {
        AUDIO_FIFO_STREAM_GET_DATA(pSrc, len);
    }
    else
    {
	    #ifdef COAXIAL_UNDERFLOW_MSG_SEND
        system_msgSend(UI_SPDIF_STREAM_UNDERFLOW);
		#else
        audio_in_stream.status = STREAM_DECOCDE_FINISH;		
		#endif

        DBG_Printf ("Spdif Rx Underflow 2\n\r");

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
	
#else

    U8 *p;

    if (coaxial_dec_env.first_frame)
    {
        if (--coaxial_dec_env.first_frame == 0)
        {
            coaxial_dec_env.index = 0;
            audio_coaxial_input_rx_start(coaxial_dec_env.read_buffer[0], AD_SPDIF_BLOCK_SIZE/8 + 24*2);//rx dummy
        }
        else
        {
            p = coaxial_dec_env.read_buffer[0];
            memset(p, 0, AD_SPDIF_BLOCK_SIZE);          
        }
    }
    
    if (coaxial_dec_env.first_frame == 0)
    {
        coaxial_dec_env.index = !coaxial_dec_env.index;
        audio_coaxial_input_rx_put_next_frame_dataPtr(coaxial_dec_env.read_buffer[coaxial_dec_env.index], AD_SPDIF_BLOCK_SIZE/4);
        
        if ( (EVENT_AUDIO_INPUT_RX_GET(global_vari.eventGroup, pdMS_TO_TICKS(100)) & AUDIO_RECEIVE_EVENT) == 0)
        {
            DBG_Assert(FALSE);//must not get here
        }
        
        p = coaxial_dec_env.read_buffer[!coaxial_dec_env.index];

        if (coaxial_dec_env.discard_frame_cnt)
        {
            if (--coaxial_dec_env.discard_frame_cnt == 0)
            {

            }
            memset(p, 0, AD_SPDIF_BLOCK_SIZE);
        }
    }


    *pout = p;
    *plen = AD_SPDIF_BLOCK_SIZE;

    return DECODE_SUCCESS;

#endif //AUDIO_IN_STREAM_USED
}

static const DECODE_DEVICE_STRU coaxial_decode = 
{
    DECODE_COAXIAL,
	"coaxial pcm decode",
	coaxial_dec_init,
	coaxial_dec_fini,
	coaxial_dec_decode_frame,
    coaxial_dec_info_get
};

void coaxial_decocde_set(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_set_msgSend(&coaxial_decode);
}

void coaxial_decocde_start(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_start_msgSend();
}

void coaxial_decocde_start_w_init(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_start_w_init_msgSend();
}

#endif //COAXIAL_ENABLE
