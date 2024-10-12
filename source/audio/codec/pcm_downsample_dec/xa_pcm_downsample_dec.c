#include "os_config.h"

#include "hw_pll.h"
#include "hw_timer.h"

#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "audio_input_receive_task.h"
#include "audio_aux_input.h"

#include "codec.h"

#if (defined OPL_MODE_SWF && defined OPL_SLAVE_ENBALE)

#define DOWNSAMPLE 10 
#define READ_FRAME_SIZE 24

typedef struct {
    U8 *read_buffer;
    U8 *rx_postProcessBuf;
    U16 discard_frame_cnt; //discard some frames when decoding start
    U8 first_frame;        //for sync
	void *downsampleFilterHandle;
} _DEC_ENV_STRU;

_DEC_ENV_STRU _dec_env = {0};

void xa_pcm_downsample_dec_init(void)
{
   int size;
 
	//DBG_Printf("%s", __func__);

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

	audio_in_stream.channel = 2;
	//audio_in_stream.sample_rate = AUDIO_IN_STREAM_SAMPLE_RATE;
	audio_in_stream.sample_speed = audio_stream_speed_get(audio_in_stream.sample_rate);

	hw_audio_mi2s_in_channel_set(audio_in_stream.channel);


	audio_clock_freq_set(audio_in_stream.sample_rate);

	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

	size = (READ_FRAME_SIZE);
	_dec_env.read_buffer = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
	_dec_env.rx_postProcessBuf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size * DOWNSAMPLE * 2, CODEC_MEM_ALIGN_32);

	#define SUBWOOFER_MAX_BIQUAD	2
	static const int subwoofer_dsp_coef[SUBWOOFER_MAX_BIQUAD*6] = {
		0x40000000,0x001c3611,0x00386c22,0x001c3611,0x87ba88d1,0x38b64f74,
		0x40000000,0x001c3611,0x00386c22,0x001c3611,0x87ba88d1,0x38b64f74,
	};

	_dec_env.downsampleFilterHandle = optek_bqriir_open(codec_malloc(optek_bqriir_alloc(SUBWOOFER_MAX_BIQUAD)),SUBWOOFER_MAX_BIQUAD,subwoofer_dsp_coef);
	if (_dec_env.downsampleFilterHandle == NULL) {
		DBG_Assert(FALSE);
	}

}

void xa_pcm_downsample_dec_fini(int reason)
{
	DBG_Printf("%s", __func__);

}

int xa_pcm_downsample_dec_info_get(DECODE_INFO_STRU *info)
{
	info->src_speed = 0;

	return 1;
}

/*******************************************************************************************************************
 * 
 * 
 *******************************************************************************************************************/

int xa_pcm_downsample_dec_decode_frame(U8 **pout, U16 *plen)
{	
	U16 inLen, outLen;
	U8 *p_read;
	U16 samples;

	inLen = dec_get_optek_link_rx_data(&p_read,1,24);

	if (inLen)
	{
		int i,k;
		short *pIn, *pOut;
	
		pIn = (short *) p_read;
		pOut = (short *) _dec_env.rx_postProcessBuf;

		//upsample X10
		for (i=0;i<inLen/2;i++) 
		{
			for (k=0;k<DOWNSAMPLE;k++)
			{
				pOut[DOWNSAMPLE*i+k] = pIn[i];
			} 
		}

		outLen = inLen*DOWNSAMPLE;
		samples = outLen/2;
		optek_bqdiir_mono_16b_process(_dec_env.downsampleFilterHandle, pOut, pOut, samples);

		pIn = (short *) pOut;
		pOut = (short *) pOut;

		//mono to stereo
		for (i=1; i<=samples; i++) 
		{
			pOut[2*samples - 2*i] = pIn[samples - i];
			pOut[2*samples - 2*i+1] = pIn[samples - i];
		}

		outLen *= 2;	
	}
	else
	{
		outLen = READ_FRAME_SIZE * DOWNSAMPLE * 2;
		memset(_dec_env.rx_postProcessBuf,0,outLen);
	}

	*pout = _dec_env.rx_postProcessBuf;
	*plen = outLen;	

	return DECODE_SUCCESS;
}

static const DECODE_DEVICE_STRU xa_pcm_downsample_decode = 
{
    DECODE_PCM_DOWNSAMPLE,
	"xa_pcm_downsample decode",	
	xa_pcm_downsample_dec_init,
	xa_pcm_downsample_dec_fini,
	xa_pcm_downsample_dec_decode_frame,
    xa_pcm_downsample_dec_info_get
};

void xa_pcm_downsample_decocde_set(void)
{
    decode_set_msgSend(&xa_pcm_downsample_decode);
}

void xa_pcm_downsample_decocde_start_w_init(void)
{
    decode_start_w_init_msgSend();
}
#endif
