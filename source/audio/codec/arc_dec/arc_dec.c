#include "os_config.h"

#ifdef ARC_ENABLE
#include "hw_pll.h"

#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "fifo.h"
#include "tools.h"
#include "codec.h"

#include "audio_input_receive_task.h"
#include "audio_arc_input.h"

#ifdef SPDIF_ENABLE
#include "spdif_process.h"
#endif

#include "arc_dec.h"
#include "clock_slave_sync.h"

#define PEAKLEVEL_ENABLE

#ifdef PEAKLEVEL_ENABLE
#include "peak_level_meter.h"
#endif


#ifdef DEBUG_GPIO_PIN0_ENABLE
#undef DEBUG_GPIO_PIN0_ENABLE
#endif

#define ARC_AUDIO_OUT_FRAME_FIXED

#define ARC_DEC_FINISH_MSG_SEND

#define ARC_UNDERFLOW_MSG_SEND

//#define RTOS_MALLOC_USED


#define MAX_NOISE_GAIN        128


typedef struct {
    U8 * read_buffer[2];
    U8 * rx_postProcessBuf;
    U16 discard_frame_cnt; //discard same frame when decoding begains
    U8 first_frame;        //for sync
    U8 index;
} ARC_DEC_ENV_STRU;

ARC_DEC_ENV_STRU arc_dec_env;


//for pcm decoding
void arc_dec_init(void)
{
#ifdef AUDIO_IN_STREAM_USED
    int len;
    int size;
    U32 sample_rate = audio_in_stream.sample_rate;

    DBG_Printf("%s\n\r", __func__);

    memset((void *)&arc_dec_env, 0, sizeof(arc_dec_env));

    DBG_Assert(arc_dec_env.read_buffer[0] == NULL);
    DBG_Assert(arc_dec_env.read_buffer[1] == NULL);

    codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

	size = (ARC_PCM_BLOCK_SIZE*4*4);
    arc_dec_env.read_buffer[0]  = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
    /*
	*arc_dec_env.read_buffer[1]  = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
	*/

	size = (ARC_PCM_BLOCK_SIZE*4*4);
    arc_dec_env.rx_postProcessBuf  = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

    arc_dec_env.discard_frame_cnt = 1;
    arc_dec_env.first_frame = 5;

    #if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE )//&& defined IIS_IN_SYNC_W_MASTER)
    //only iis in slave role need source sync
    sync_w_src_start();
    #endif


    #ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	clock_slave_sync_init(ARC_PCM_BLOCK_SIZE, sample_rate);
    #endif
    #else
    memset((void *)&arc_dec_env, 0, sizeof(arc_dec_env));

    DBG_Assert(arc_dec_env.read_buffer[0] == NULL);
    DBG_Assert(arc_dec_env.read_buffer[1] == NULL);//memory leak?

    arc_dec_env.read_buffer[0] = codec_malloc_w_memtype(ARC_PCM_BLOCK_SIZE);
    arc_dec_env.read_buffer[1] = codec_malloc_w_memtype(ARC_PCM_BLOCK_SIZE);

    EVENT_AUDIO_INPUT_RX_CLR;

    audio_arc_input_rx_init();

    arc_dec_env.discard_frame_cnt = 1;
    arc_dec_env.first_frame = 5;

    #if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE )//&& defined IIS_IN_SYNC_W_MASTER)
    //only iis in slave role need source sync
    sync_w_src_start();
    #endif

#endif //AUDIO_IN_STREAM_USED
}

void arc_dec_fini(int reason)
{
    DBG_Printf("%s\n\r", __func__);
    
#ifdef AUDIO_IN_STREAM_USED
    audio_arc_input_rx_stop_req();

    arc_dec_env.read_buffer[0] = NULL;
    arc_dec_env.read_buffer[1] = NULL;
#else
    audio_arc_input_rx_stop_req();

    arc_dec_env.read_buffer[0] = NULL;
    arc_dec_env.read_buffer[1] = NULL;
#endif

    #ifdef ARC_DEC_FINISH_MSG_SEND
    system_msgSend(UI_DECODE_FINISH);
    #else
    audio_in_stream.status = STREAM_DECOCDE_FINISH;
    #endif
}

#ifdef IIS_IN_SYNC_W_MASTER
U16 reference_point;
U16 current_point;
#endif

int arc_dec_info_get(DECODE_INFO_STRU *info)
{
	#ifdef LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE
	I16 dritf_samples;

	//dritf_samples = current_point - reference_point;

    dritf_samples = ((steam_data_len - DEST_DATA_LEN)/8);
    #ifdef AUDIO_CLOCK_SYNC_PRINTF
	DBG_Printf("drift samples:%d\n",dritf_samples);
    #endif

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

int arc_dec_decode_frame(U8 **pout, U16 *plen)
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

    static U16 cnt = (0x8000-1);

    if (cnt++ == 0x8000)
    {
        cnt = 0;
        // printf_info("dlen:%d\n\r", dlen);
    }
    steam_data_len = dlen;
    
    #ifdef ARC_AUDIO_OUT_FRAME_FIXED
    if (in_sample_speed == SAMPLE_SPEED_4X)
    {
	    len = (ARC_PCM_BLOCK_SIZE*4)*(in_channel/2);
    }
    else if (in_sample_speed == SAMPLE_SPEED_2X)
    {
        len = (ARC_PCM_BLOCK_SIZE*2)*(in_channel/2);
    }
    else
    { 
        len = (ARC_PCM_BLOCK_SIZE)*(in_channel/2);
    }
    #else
    len = ARC_PCM_BLOCK_SIZE;
    #endif

    #ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
    clock_slave_sync_polling_for_spdif(sample_rate, len);
    #endif

    pSrc = arc_dec_env.read_buffer[0];
    pDest = arc_dec_env.rx_postProcessBuf;

    if (dlen >= ARC_PCM_BLOCK_SIZE)
    {
        AUDIO_FIFO_STREAM_GET_DATA(pSrc, len);
    }
    else
    {
	    #ifdef ARC_UNDERFLOW_MSG_SEND
        system_msgSend(UI_SPDIF_STREAM_UNDERFLOW);
		#else
        audio_in_stream.status = STREAM_DECOCDE_FINISH;		
		#endif

        DBG_Printf ("ARC Rx Underflow2\n\r");

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

    if (arc_dec_env.first_frame)
    {
        if (--arc_dec_env.first_frame == 0)
        {
            arc_dec_env.index = 0;
            audio_arc_input_rx_start(arc_dec_env.read_buffer[0], AD_SPDIF_BLOCK_SIZE/8 + 24*2);//rx dummy
        }
        else
        {
            p = arc_dec_env.read_buffer[0];
            memset(p, 0, AD_SPDIF_BLOCK_SIZE);          
        }
    }
    
    if (arc_dec_env.first_frame == 0)
    {
        arc_dec_env.index = !arc_dec_env.index;
        audio_arc_input_rx_put_next_frame_dataPtr(arc_dec_env.read_buffer[arc_dec_env.index], AD_SPDIF_BLOCK_SIZE/4);
        
        if ( (EVENT_AUDIO_INPUT_RX_GET(global_vari.eventGroup, pdMS_TO_TICKS(100)) & AUDIO_RECEIVE_EVENT) == 0)
        {
            DBG_Assert(FALSE);//must not get here
        }
        
        p = arc_dec_env.read_buffer[!arc_dec_env.index];

        if (arc_dec_env.discard_frame_cnt)
        {
            if (--arc_dec_env.discard_frame_cnt == 0)
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

static const DECODE_DEVICE_STRU arc_decode = 
{
    DECODE_ARC,
	"arc pcm decode",	
	arc_dec_init,
	arc_dec_fini,
	arc_dec_decode_frame,
    arc_dec_info_get
};

void arc_decocde_set(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_set_msgSend(&arc_decode);
}

void arc_decocde_start(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_start_msgSend();
}

void arc_decocde_start_w_init(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_start_w_init_msgSend();
}

#endif //ARC_ENABLE
