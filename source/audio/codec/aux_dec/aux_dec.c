#include "os_config.h"

#ifdef AUX_ENABLE
#include "hw_audio_i2s.h"
#include "hw_audio_adc_dac.h"

#include "fifo.h"
#include "codec.h"
#include "audio_aux_input.h"
#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "aux_dec.h"
#include "audio_input_receive_task.h"
#include "spdif_data_parse.h"
#include "spdif_process.h"

#include "tools.h"
#include "string.h"

#include "audio_device.h"
#include "optek_vol_table.h"
#ifdef DEBUG_GPIO_PIN0_ENABLE
#undef DEBUG_GPIO_PIN0_ENABLE
#endif

#define PEAKLEVEL_ENABLE

#ifdef PEAKLEVEL_ENABLE
#include "peak_level_meter.h"
#endif

#define VOL_TOLERANCE  22

extern U8 AUX_STREAM_START[];
extern U8 AUX_STREAM_END[];

#define MAX_NOISE_GAIN        128


typedef struct {
    U8 *read_buffer[2];
    U8 *rx_postProcessBuf;
    U16 discard_frame_cnt;//discard same frame when decoding begains
    U16 frame_samples;
    U8 first_frame;//for sync
    U8 index;
} AUX_DEC_ENV_STRU;


AUX_DEC_ENV_STRU aux_dec_env;

void pcm_dec_next_rx_buffer_put(void)
{
    aux_dec_env.index = !aux_dec_env.index;
    audio_aux_input_put_next_frame_dataPtr((U32 *)aux_dec_env.read_buffer[aux_dec_env.index], aux_dec_env.frame_samples);    
}

void pcm_dec_init(void)
{
    int size;

	DBG_Printf("%s\n\r", __func__);

    memset((void *)&aux_dec_env, 0, sizeof(AUX_DEC_ENV_STRU));

	#ifdef MCLK_IN_AS_AUDIO_OUT_CLK

	#ifdef I2S_TDM_DATA_WIDTH_24_BIT 
    hw_audio_mi2s_osr_div_set(1);
	#else
	if (audio_in_stream.sample_speed == SAMPLE_SPEED_4X)
	{
		hw_audio_mi2s_osr_div_set(2);
	}
	else
	{
        // hw_audio_mi2s_osr_div_set(2);
		// hw_audio_mi2s_osr_div_set(0);
        hw_audio_mi2s_osr_div_set(1);
	}
  	#endif
	
    //MClk:external clock
	hw_audio_mi2s_mclk_switch(TRUE);
	#else
    #ifdef ADC_DAC_DATA_WIDTH_24_BIT
    // hw_audio_mi2s_osr_div_set(2);//96k
    hw_audio_mi2s_osr_div_set(4);
    #else
	hw_audio_mi2s_osr_div_set(4);
    #endif
		
	//MClk:internal clock
	hw_audio_mi2s_mclk_switch(FALSE);
	#endif

    #ifdef IIS0_MASTER_ENABLE
	hw_audio_mi2s_master_slave_switch(IIS_RX_MASTER);
	#else
	hw_audio_mi2s_master_slave_switch(IIS_RX_SLAVE);
    #endif

    #ifdef AUX_INPUT_FROM_ADC

    #if ( (defined AUX_MODE_INPUT_FROM_ADC_A_ANALOG) || (defined AUX_MODE_INPUT_FROM_ADC_A_DMIC) ||\
         (defined AUX_MODE_INPUT_FROM_ADC_B_ANALOG) || (defined AUX_MODE_INPUT_FROM_ADC_B_DMIC) ||\
         (defined AUX_MODE_INPUT_FROM_ADC_C_DMIC) || (defined AUX_MODE_INPUT_FROM_ADC_D_DMIC) )
    U8 input_ch_count = 0;
    #if ( (defined AUX_MODE_INPUT_FROM_ADC_A_ANALOG) || (defined AUX_MODE_INPUT_FROM_ADC_A_DMIC) )
    input_ch_count +=2;
    #endif
    #if ( (defined AUX_MODE_INPUT_FROM_ADC_B_ANALOG) || (defined AUX_MODE_INPUT_FROM_ADC_B_DMIC ))
    input_ch_count +=2;
    #endif 
    #if (defined AUX_MODE_INPUT_FROM_ADC_C_DMIC)
    input_ch_count +=2;
    #endif
    #if (defined AUX_MODE_INPUT_FROM_ADC_D_DMIC)
    input_ch_count +=2;
    #endif

    if (AUX_MODE_INPUT_ADC_CH != input_ch_count)
    {
        printf_error("set AUX_MODE_INPUT_ADC_CH!\n\r");
    }

    audio_in_stream.channel = input_ch_count;
    #else
    audio_in_stream.channel = AUDIO_IN_STREAM_CHANNEL;
    #endif
    #elif defined AUX_INPUT_FROM_ADC
    audio_in_stream.channel = AUDIO_IN_STREAM_CHANNEL;
    #else
	audio_in_stream.channel = AUDIO_IN_STREAM_CHANNEL;
    #endif
	
	audio_in_stream.sample_rate = AUDIO_IN_STREAM_SAMPLE_RATE;
	audio_in_stream.sample_speed = audio_stream_speed_get(audio_in_stream.sample_rate);

	hw_audio_mi2s_in_channel_set(audio_in_stream.channel);
	audio_clock_freq_set(audio_in_stream.sample_rate);


	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

    DBG_Assert(aux_dec_env.read_buffer[0] == NULL);
    DBG_Assert(aux_dec_env.read_buffer[1] == NULL);

    aux_dec_env.frame_samples = decode_frame_samples_get();

    if (0 == aux_dec_env.frame_samples) 
        aux_dec_env.frame_samples = AUX_PCM_BLOCK_SIZE/4;//todo, now, just regard it as two channel and 16bit

	size = (aux_dec_env.frame_samples*4*4);
    aux_dec_env.read_buffer[0] = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
	#ifndef AUX_IN_STREAM_USED
	aux_dec_env.read_buffer[1] = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
	#endif

	size = (aux_dec_env.frame_samples*4*4);
    aux_dec_env.rx_postProcessBuf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

    EVENT_AUDIO_INPUT_RX_CLR(global_vari.eventGroup);

    aux_dec_env.discard_frame_cnt = 1;
    aux_dec_env.first_frame = 0;
 
    aux_dec_env.index = 0;

    memset(aux_dec_env.read_buffer[0], 0, sizeof(aux_dec_env.read_buffer[0]));

    size = (((aux_dec_env.frame_samples - aux_dec_env.frame_samples/5)+3)>>2)<<2;//reserve 1/5 time for encode
    audio_aux_input_rx_start((U32 *)aux_dec_env.read_buffer[0]+(aux_dec_env.frame_samples-size), size); //rx dummy

    aux_dec_env.index = !aux_dec_env.index;
    audio_aux_input_put_next_frame_dataPtr((U32 *)aux_dec_env.read_buffer[aux_dec_env.index], aux_dec_env.frame_samples);

	#if (defined LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE && defined IIS_IN_SYNC_W_MASTER)
	//only iis in slave role need source sync
	sync_w_src_start();
	#endif
}

void pcm_dec_fini(int reason)
{
    DBG_Printf("%s\n\r", __func__);
    
#ifdef AUX_IN_STREAM_USED
    aux_dec_env.read_buffer[0] = NULL;
    aux_dec_env.read_buffer[1] = NULL;

    audio_in_stream.status = STREAM_DECOCDE_FINISH;
#else
    audio_aux_input_rx_stop_req();

    aux_dec_env.read_buffer[0] = NULL;
    aux_dec_env.read_buffer[1] = NULL;
#endif
}

#ifdef IIS_IN_SYNC_W_MASTER
U16 reference_point;
U16 current_point;
#endif

int pcm_dec_info_get(DECODE_INFO_STRU *info)
{
    #ifdef AUDIO_CLOCK_SYNC_ENABLE
    #if defined AUX_INPUT_FROM_IIS0 || defined AUX_INPUT_FROM_IIS1
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
 
enumDECODE_FRAME_RETs pcm_dec_decode_frame(U8 **pout, U16 *plen)
{
#ifdef AUX_IN_STREAM_USED
    U32 *pSrc;
    U32 *pDest;
    U32 dlen;
    U32 len, out_len;
	#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
    U32 sample_rate = audio_in_stream.sample_rate;
	#endif
    int in_sample_speed;
    int in_channel;

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
 
    len = AUX_PCM_BLOCK_SIZE;
 
    #ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
    clock_slave_sync_polling(sample_rate, AUX_PCM_BLOCK_SIZE);
    #endif

    pSrc = (U32 *)aux_dec_env.read_buffer[0];
    pDest = (U32 *)aux_dec_env.rx_postProcessBuf;


	if (dlen >= len)
	{
		AUDIO_FIFO_STREAM_GET_DATA((U8 *)pSrc, len);
	}
	else
	{
		DBG_Printf ("Aux Rx Underflow 1\n\r");

		decode_exit_msgSend();
		return DECODE_END;
	}


	#ifdef AUDIO_OUT_CHANNEL_SET_FIXED
	audio_out_data_channel_switch(pSrc, pDest, in_sample_speed, in_channel, len, &out_len);
    *pout = (U8 *)pDest;
    *plen = out_len;
    #else
    *pout = (U8 *)pSrc;
    *plen = len;    
	#endif //AUDIO_OUT_CHANNEL_SET_FIXED

    return DECODE_SUCCESS;

#else

    U32 *pSrc;
    U32 *pDest;
    U32 len, out_len;
    int in_sample_speed;
    int in_channel;

    len = aux_dec_env.frame_samples*4;
    in_sample_speed = audio_in_stream.sample_speed;
    in_channel = audio_in_stream.channel;
	if (in_channel <= 0)
	{
		in_channel = 2;
	}

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
/*
    if (aux_dec_env.first_frame)
    {
        if (--aux_dec_env.first_frame == 0)
        {
            aux_dec_env.index = 0;
            audio_aux_input_rx_start((U32 *)aux_dec_env.read_buffer[0], len/8 + 24*2); //rx dummy
        }
        else
        {
            pSrc = (U32 *)aux_dec_env.read_buffer[0];
            memset((U8 *)pSrc, 0, len);
        }
    }
*/    

    if (aux_dec_env.first_frame == 0)
    {
        btdm_debug_pin(26, 1);
        if ( (EVENT_AUDIO_INPUT_RX_GET(global_vari.eventGroup, pdMS_TO_TICKS(100)) & AUDIO_RECEIVE_EVENT) == 0)
        {
            DBG_Assert(FALSE);//must not get here
        }
        btdm_debug_pin(26, 0);

        aux_dec_env.index = !aux_dec_env.index;
        audio_aux_input_put_next_frame_dataPtr((U32 *)aux_dec_env.read_buffer[aux_dec_env.index], len/4);

        pSrc = (U32 *)aux_dec_env.read_buffer[aux_dec_env.index];

        if (aux_dec_env.discard_frame_cnt)
        {
            if (--aux_dec_env.discard_frame_cnt == 0)
            {

            }
            memset(pSrc, 0, len);
        }
    }

    pSrc = (U32 *)aux_dec_env.read_buffer[aux_dec_env.index];
    pDest = (U32 *)aux_dec_env.rx_postProcessBuf;

    // optek_vol_24b_process (pSrc, pSrc 0x0ff64c17, len/4); //+6db
    // optek_vol_24b_process (pSrc, pSrc, optek_vol_vol_table[240 - 12*10 + (VOLUME_MAX - audio_vari.volume)*5], len/4); //+3db

    // optek_vol_24b_process (pSrc, pSrc, optek_vol_vol_table[240 - 12*10 + (VOLUME_MAX - VOL_TOLERANCE)*5], len/4); //7.5db
    
    #if 0//def AUDIO_OUT_CHANNEL_SET_FIXED
	audio_out_data_channel_switch(pSrc, pDest, in_sample_speed, in_channel, len, &out_len);
    *pout = (U8 *)pDest;
    *plen = out_len;
    #else
    *pout = (U8 *)pSrc;
    *plen = len;
	#endif //AUDIO_OUT_CHANNEL_SET_FIXED

    return DECODE_SUCCESS;

#endif //AUX_IN_STREAM_USED
}

static const DECODE_DEVICE_STRU aux_decode = 
{
    DECODE_PCM,
	"aux decode",	
	pcm_dec_init,
	pcm_dec_fini,
	pcm_dec_decode_frame,
    pcm_dec_info_get
};

void aux_decocde_set(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_set_msgSend((DECODE_DEVICE_STRU *)&aux_decode);
}

void aux_decocde_start_w_init(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_start_w_init_msgSend();
}

#endif //AUX_ENABLE
