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
#include "msbc_dec_test_01_msbc.h"
#include "fifo.h"

#if defined mSBC_DEC && defined mSBC_ENC
int xa_msbc_dec_init (void);
XA_ERRORCODE xa_msbc_enc_init (void);
int xa_msbc_dec_frame(U8 *pInBuf, U16 inLen, U8 **pOutBuf, U32 *pOutDataLen);
XA_ERRORCODE xa_msbc_enc_main_process(U16 *pInBuf, U32 samples, U8 **pOutBuf, U32 *pOutDataLen);

#define MAX_CONCEALMENT_CNT 5

#define HFP_MSBC_FRAME_SIZE 60
#define HFP_PCM_FRAME_SIZE (360*4)//MSBC 7.5MS

typedef struct {
    U8 *rx_inputBuf;
    U8 *rx_postProcessBuf;
    U16 discard_frame_cnt; //discard some frames when decoding start
    U8 first_frame;        //for sync
    U8 index;
	U8 loss_cnt;
	void *hfp_upStreamDSPHandle;
	void *hfp_downStreamDSPHandle;
	FIFO *upstream_fifo;
} MSBC_DEC_ENV_STRU;

MSBC_DEC_ENV_STRU msbc_dec_env = {0};


U8 *test_in;

#define MSBC_MAX_BIQUAD 2
const int msbc_dsp_coef[MSBC_MAX_BIQUAD*6] = {
	0x40000000,0x063f9666,0x0c7f2ccc,0x063f9666,0xc3a90445,0x15555555,
	0x40000000,0x063f9666,0x0c7f2ccc,0x063f9666,0xc3a90445,0x15555555,
};

extern FIFO MicInFifo;
extern U32 *usbdev_mic_in_fifo_buf;
void msbc_dec_init(void)
{
   	int size;
	U8 * dsp_buf;

	audio_out_stream.sample_rate = 48000;
	audio_in_stream.channel = 2;
	hw_audio_mi2s_in_channel_set(audio_in_stream.channel);

	audio_out_stream.channel = AUDIO_OUT_STREAM_CHANNEL;
	hw_audio_mi2s_out_channel_set(audio_out_stream.channel);

	audio_clock_freq_set(audio_in_stream.sample_rate);

	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

	msbc_dec_env.loss_cnt = MAX_CONCEALMENT_CNT;

	size = (HFP_PCM_FRAME_SIZE);
	msbc_dec_env.rx_postProcessBuf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

	msbc_dec_env.rx_inputBuf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, HFP_MSBC_FRAME_SIZE, CODEC_MEM_ALIGN_32);

	dsp_buf = codec_malloc(optek_bqriir_alloc(MSBC_MAX_BIQUAD));
	if (dsp_buf == NULL)
	{
		DBG_Assert(FALSE);
	}

	msbc_dec_env.hfp_upStreamDSPHandle = optek_bqriir_open(dsp_buf, MSBC_MAX_BIQUAD, msbc_dsp_coef);
	if (msbc_dec_env.hfp_upStreamDSPHandle == NULL)
	{
		DBG_Assert(FALSE);
	}

	dsp_buf = codec_malloc(optek_bqriir_alloc(MSBC_MAX_BIQUAD));
	if (dsp_buf == NULL)
	{
		DBG_Assert(FALSE);
	}

	msbc_dec_env.hfp_downStreamDSPHandle=optek_bqriir_open(dsp_buf, MSBC_MAX_BIQUAD, msbc_dsp_coef);
	if (msbc_dec_env.hfp_downStreamDSPHandle == NULL)
	{
		DBG_Assert(FALSE);
	}

	msbc_dec_env.upstream_fifo = codec_malloc((sizeof(FIFO)));

	fifo_init(msbc_dec_env.upstream_fifo, codec_malloc(HFP_MSBC_FRAME_SIZE*8), HFP_MSBC_FRAME_SIZE*8,TRUE);

	//need change hfp's mic input function

//	dsp_buf = codec_malloc(HFP_PCM_FRAME_SIZE*8);
//	fifo_init (&MicInFifo, (U8 *)dsp_buf, HFP_PCM_FRAME_SIZE*8, TRUE);
//
//	usbdev_mic_in_fifo_buf = dsp_buf;
//
//	audio_usb_speaker_input_rx_start(NULL, 0);

	test_in = msbc_dec_test_01_msb;

	xa_msbc_dec_init();
	xa_msbc_enc_init();

}

void msbc_dec_fini(int reason)
{
	DBG_Printf("%s", __func__);
	usbdev_mic_in_fifo_buf = NULL;
}

int msbc_dec_info_get(DECODE_INFO_STRU *info)
{
	info->src_speed = 0;

	return 1;
}

/*******************************************************************************************************************
 * 
 * 
 *******************************************************************************************************************/
int msbc_dec_decode_frame(U8 **pout, U16 *plen)
{	

	U8 inLen;
	U8 *p_rx;
	U8 *pDecOut;
	U16 outlen;
	U8 *pOut;
	u16 outputsize;
    U32 dlen;
    int i;

/************msbc encode****************************/
	dlen = fifo_get_data_len(&MicInFifo);		

	if (dlen >= HFP_PCM_FRAME_SIZE) 
	{
		if (dlen > HFP_PCM_FRAME_SIZE*4)
		{
			//DBG_Assert(FALSE);
			fifo_discard_data(&MicInFifo,(dlen - (HFP_PCM_FRAME_SIZE+480)));
			DBG_Printf("HFP mic in overflow,dlen:%d\n\r", dlen);		
		}
		
		fifo_get_data (&MicInFifo, msbc_dec_env.rx_postProcessBuf, HFP_PCM_FRAME_SIZE);
	}
	else
	{
		if (dlen > 480)
		{
			fifo_discard_data(&MicInFifo, (dlen - 480));
		}
		
		//DBG_Assert(FALSE);
		memset(msbc_dec_env.rx_postProcessBuf, 0, HFP_PCM_FRAME_SIZE);
		DBG_Printf("HFP mic in buf underflow 1\n\r");
	}

	optek_bqdiir_stereo_16b_process (msbc_dec_env.hfp_upStreamDSPHandle, msbc_dec_env.rx_postProcessBuf, msbc_dec_env.rx_postProcessBuf, HFP_PCM_FRAME_SIZE/2);

	/** convert 48k to 16k, stereo to mono **/
	U16 samples = HFP_PCM_FRAME_SIZE/12;
	short *pin = msbc_dec_env.rx_postProcessBuf;


	#ifdef MIC_INPUT_CHANGE_CH_LR
	for (i=0; i<samples; i++) 
	{ 
		if (adc_LRch_switch == ADC_INPUT_LCH)
		{
			pin[i] = pin[6*i+0]; //left channel
		}
		else if (adc_LRch_switch == ADC_INPUT_RCH)
		{
			pin[i] = pin[6*i+1]; //right channel
		}
		else//ADC_INPUT_UNKNOW
		{
			pin[i] = pin[6*i+0]; //unknow channel
		}
	}
	#else
	for (i=0; i<samples; i++) 
	{ 
		#ifdef MIC_INPUT_FIX_CHANNEL_L
		pin[i] = pin[6*i+1]; //left channel
		#else
		pin[i] = pin[6*i+0]; //right channel
		#endif
	}
	#endif


	int xa_msbc_enc_main_process(U16 *pInBuf, U32 samples, U8 **pOutBuf, U32 *pOutDataLen);
	U8 *pOutBuf = NULL;
	U8 size;

	xa_msbc_enc_main_process(msbc_dec_env.rx_postProcessBuf, samples, &pOutBuf, &size);

	fifo_put_data (msbc_dec_env.upstream_fifo, pOutBuf, size);




/************msbc decode****************************/
	dlen = AUDIO_FIFO_STREAM_DATA_LEN();

	if (dlen >= HFP_MSBC_FRAME_SIZE)
	{
		dlen = AUDIO_FIFO_STREAM_GET_DATA(msbc_dec_env.rx_inputBuf, HFP_MSBC_FRAME_SIZE);

		//printf_info("%x, %x, %x, %x\n", msbc_dec_env.rx_inputBuf[0], msbc_dec_env.rx_inputBuf[1], msbc_dec_env.rx_inputBuf[2], msbc_dec_env.rx_inputBuf[3]);
		xa_msbc_dec_frame( msbc_dec_env.rx_inputBuf+2, 57, &pOut, &outputsize);
		//test_in += 57;
	}
	else
	{
		DBG_Printf("hfp buf underflow 1\r\n");
		pOut = NULL;
	}

	if (pOut != NULL)
	{
		short *pO = msbc_dec_env.rx_postProcessBuf;
		short *pI = pOut;
		
		for (i=0; i<120; i++)
		{
			*pO++ = *pI;
			*pO++ = *pI;
			*pO++ = *pI;

			*pO++ = *pI;
			*pO++ = *pI;
			*pO++ = *pI++;
		}
		
		optek_bqdiir_stereo_16b_process (msbc_dec_env.hfp_downStreamDSPHandle, msbc_dec_env.rx_postProcessBuf, msbc_dec_env.rx_postProcessBuf, HFP_PCM_FRAME_SIZE/2);
	}
	else
	{
		DBG_Printf("hfp msbc dec err 1\r\n");
		memset(msbc_dec_env.rx_postProcessBuf, 0, HFP_PCM_FRAME_SIZE);
	}

	*pout = msbc_dec_env.rx_postProcessBuf;
	*plen = HFP_PCM_FRAME_SIZE;


	return DECODE_SUCCESS;
}

static const DECODE_DEVICE_STRU msbc_decode = 
{
    DECODE_MSBC,
	"msbc decode",	
	msbc_dec_init,
	msbc_dec_fini,
	msbc_dec_decode_frame,
    msbc_dec_info_get
};

void msbc_decocde_set(void)
{
    decode_set_msgSend(&msbc_decode);
}


U32 hfp_msbc_encData_read(U8 *ptr, U32 len)
{
	static U8 seq_num = 0x08;
	u32 dlen,wlen;

	ptr[0] = 0x01;
	ptr[len-1] = 0;

	switch (seq_num)
	{
		case 0x08:
			seq_num = 0x38;
			break;
		case 0x38:
			seq_num = 0xc8;
			break;
		case 0xc8:
			seq_num = 0xf8;
			break;
		case 0xf8:
			seq_num = 0x08;
			break;
		default :
			seq_num = 0x08;
			break;
	}
	ptr[1] = seq_num;

	if (dlen >= 57) 
	{
		dlen = fifo_get_data (msbc_dec_env.upstream_fifo, ptr+2,57);
	}
	else 
	{
		DBG_Printf ("MIC data underflow 1\n\r");
		memset(ptr,0,len);
		return len;		
	}

	return len;
}

#endif
