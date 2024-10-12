#include "os_config.h"

#ifdef WAV_DEC
#include "app_main.h"
//#include "audio_dec.h" 
#include "audio_com.h" 
//#include "app_dac.h" 
#include "app_media_common.h"
#include "wave_decode.h"
#include "optek_src.h"
#include "app_media.h"
#include "optek_link.h"

#define SUPPORT_MESSAGE_DISPLAY


#ifndef AUDIO_OUTPUT_24BIT
#define WAVE_24BIT_TO_16BIT_ENABLE
#endif


#define	FORMAT_TAG_BLUERAY    0xFFFE

U32 dec_src_flag;

extern volatile U8 wRequestDecodeFrameExit;

U8 wav_jump_adjust = FALSE;
U32 wav_bytes_played;


void pack_32_to_24_bits (U8 * pb_out_ptr, U32 input);


WAVE_DECODE_DATA *pwave;

typedef struct 
{
    // RIFF header
    char cRIFFHeader[4];		/* "RIFF" */
    unsigned int nRIFFBytes;	/* pcmbytes + 44 - 8, length in bytes without header */

    // data type
    char cDataTypeID[4];		/* "WAVE" */
} __attribute__((packed)) WAVE_HEADER1;

typedef struct 
{
    char type[4];		/* "RIFF" */
    unsigned int Bytes;	/* pcmbytes + 44 - 8, length in bytes without header */
} __attribute__((packed)) WAVE_CHUNK;

typedef struct 
{
    unsigned short nFormatTag;	    /* is pcm?, 2 bytes */
    unsigned short nChannels;	    /* 2, 2 bytes */
    unsigned int nSamplesPerSec;	/* 44100, 4 bytes */
    unsigned int nAvgBytesPerSec;	/* 44100 * 2 * ((bits + 7) / 8) */
    unsigned short nBlockAlign;		/* channels * ((bits + 7) / 8) */
    unsigned short nBitsPerSample;	/* bits = 16 */
} __attribute__((packed)) WAVE_PARA;

int i_bitrate, i_samp_freq, i_num_chan, i_pcm_wd_sz;


//return 0 success
static WAVE_STATUS xa_wave_dec_init (void)
{
	int len;
	WAVE_HEADER1 *header;
	WAVE_CHUNK chunk;

	DBG_Printf("xa wav dec int enter\n\r");

//------------------------------------------------------mem init
	//init mem
	codec_malloc_init ();

	entry_id3_flag = ID3_NO_TAG;

	pwave = (WAVE_DECODE_DATA *)codec_malloc (sizeof(WAVE_DECODE_DATA));

	pwave->out_buf = (U8 *)codec_malloc (WAVE_DECODE_OUTPUT_BYTES*4);

#ifdef SRC_SIMPLE_VERSION
	Sacro_Init();	//init sample rate conversion
#endif

//------------------------------------------------------read header
	header = (WAVE_HEADER1 *)codec_malloc (sizeof(WAVE_HEADER1));
	len = xa_fread(header, 1, sizeof(WAVE_HEADER1), NULL);
	
	if (memcmp(header->cRIFFHeader,"RIFF",4) !=0)
	{
		DBG_Printf("not RIFF\n\r");
		return WAVE_HEADER_ERR;
	}

	if (memcmp(header->cDataTypeID, "WAVE", 4) !=0)
	{
		DBG_Printf("not WAVEfmt \n\r");
		return WAVE_HEADER_ERR;
	}

	app_main_data.playing_stream_vbr = 0;

//------------------------------------------------------read chunk
	while(1)
	{
		if (wRequestDecodeFrameExit)
		{
			return WAVE_READ_ERR;
		}

		len = xa_fread(&chunk, 1, sizeof(WAVE_CHUNK), NULL);

//------------------------------------------------------read parameter
		if (memcmp(chunk.type, "fmt ", 4) == 0)
		{
			WAVE_PARA *para = (WAVE_PARA *)codec_malloc (sizeof(WAVE_PARA));

			 xa_fread(para, 1, sizeof(WAVE_PARA), NULL);

			if ( (para->nFormatTag != 1) && (para->nFormatTag != FORMAT_TAG_BLUERAY) )
			{
				DBG_Printf("not PCM\n\r");
				return WAVE_NOT_PCM;
			}

			pwave->nBlockAlign = para->nBlockAlign;
			
			app_main_data.playing_stream_chans = para->nChannels;
			app_main_data.playing_stream_sample_rate = para->nSamplesPerSec;
			app_main_data.playing_stream_bitrate = para->nAvgBytesPerSec; //byte rate
			app_main_data.playing_stream_sample_bits = para->nBitsPerSample;

			DBG_Printf("wav_format = %d\n", para->nFormatTag);
			DBG_Printf("wav_blockAlign = %d\n", para->nBlockAlign);
			DBG_Printf("wav_Channels = %d\n", para->nChannels);
			DBG_Printf("wav_samp_rate = %d\n", para->nSamplesPerSec);
			DBG_Printf("wav_byterate = %d\n", para->nAvgBytesPerSec);
			DBG_Printf("wav_sample_bits = %d\n", para->nBitsPerSample);

			if (chunk.Bytes > sizeof(WAVE_PARA))
			{
				xa_fseek( NULL, chunk.Bytes - sizeof(WAVE_PARA), 1 );
			}
		}
//------------------------------------------------------find data
		else if (memcmp(chunk.type, "data", 4) == 0)
		{
			pwave->bytes = chunk.Bytes;

			pwave->data_pos = xa_ftell (NULL);
			app_main_data.playing_stream_media_offset = pwave->data_pos;
			app_main_data.playing_stream_total_time = chunk.Bytes / app_main_data.playing_stream_bitrate;
			DBG_Printf("Wave Header ok\n\r");
			DBG_Printf("Channels = %d\n", app_main_data.playing_stream_chans);
			DBG_Printf("samp_freq = %d\n", app_main_data.playing_stream_sample_rate);
			DBG_Printf("bitrate = %d\n", app_main_data.playing_stream_bitrate);
			DBG_Printf("Total Time = %d\n", app_main_data.playing_stream_total_time);
			DBG_Printf("pcm_wd_sz = %d\n\r", app_main_data.playing_stream_sample_bits);
			break;
		}
//------------------------------------------------------skip chunk
		
		else
		{
			xa_fseek( NULL, chunk.Bytes, 1 );
		}
	}


#ifdef SRC_NEW_VER
#ifdef WAV_96K_SUPPORT
	dec_src_flag = app_main_data.playing_stream_sample_rate;
	if (app_main_data.playing_stream_sample_rate == 96000)
	{
		app_main_data.playing_stream_sample_rate = 48000;
	}
	else if (app_main_data.playing_stream_sample_rate == 88200)
	{
		app_main_data.playing_stream_sample_rate = 44100;
	}
#endif


	if( src_init( app_main_data.playing_stream_sample_rate, app_main_data.playing_stream_chans ) )
	{
		DBG_Printf("src init error for sample rate: %d\n\r", i_samp_freq);
	}

#elif defined OPTEK_SRC_VER

	dec_src_flag = app_main_data.playing_stream_sample_rate;

#ifdef WAV_192K_SUPPORT
	if (app_main_data.playing_stream_sample_rate == 96000*2)
	{
#ifdef AUDIO_192K_DOWN_SAMPLE
#ifdef AUDIO_192K_DOWN_48K_SAMPLE
	app_main_data.playing_stream_sample_rate = 48000;
#else
	app_main_data.playing_stream_sample_rate = 96000;
#endif
#endif
	}
	else if (app_main_data.playing_stream_sample_rate == 88200*2)
	{
#ifdef AUDIO_176P4K_DOWN_SAMPLE
#ifdef AUDIO_176P4K_DOWN_44P1K_SAMPLE
		app_main_data.playing_stream_sample_rate = 44100;
#else
		app_main_data.playing_stream_sample_rate = 88200;
#endif
#endif
	}
	else if (app_main_data.playing_stream_sample_rate == 64000*2)
	{
#ifdef AUDIO_128K_DOWN_SAMPLE
#ifdef AUDIO_128K_DOWN_32K_SAMPLE
		app_main_data.playing_stream_sample_rate = 32000;
#else
		app_main_data.playing_stream_sample_rate = 64000;
#endif
#endif
	}
#endif //WAV_192K_SUPPORT

#ifdef WAV_96K_SUPPORT
	if (app_main_data.playing_stream_sample_rate == 96000)
	{
#ifdef AUDIO_DOWN_SAMPLE
		app_main_data.playing_stream_sample_rate = 48000;
#endif
	}
	else if (app_main_data.playing_stream_sample_rate == 88200)
	{
#ifdef AUDIO_DOWN_SAMPLE
		app_main_data.playing_stream_sample_rate = 44100;
#endif
	}
	else if (app_main_data.playing_stream_sample_rate == 64000)
	{
#ifdef AUDIO_DOWN_SAMPLE
		app_main_data.playing_stream_sample_rate = 32000;
#endif
	}
#endif //WAV_96K_SUPPORT

#ifdef OPTEK_DSRC_ENABLE
	//for testing d2as

	app_main_data.stream_in_sample_rate = app_main_data.playing_stream_sample_rate;
	app_main_data.stream_out_sample_rate = SAMPLE_RATE_48K;

	d2as_need_flag = TRUE;
#endif

#endif //SRC_NEW_VER

#ifdef OPTEK_DSRC_ENABLE
	app_audio_clock_freq_setting(app_main_data.stream_out_sample_rate);
#else
	app_audio_clock_freq_setting(app_main_data.playing_stream_sample_rate);
#endif

#if defined WAV_192K_SUPPORT
	if (global_vari.audio_source == MEDIA_SDMMC)
	{
		if (app_main_data.playing_stream_sample_rate > 48000 || app_main_data.playing_stream_sample_bits == 32 || app_main_data.playing_stream_sample_bits == 8)
		{

		}
	}
	else
	{
		if (app_main_data.playing_stream_sample_rate > 192000 || app_main_data.playing_stream_sample_bits == 32 || app_main_data.playing_stream_sample_bits == 8)
		{
			return WAVE_READ_ERR;
		}
	}
#elif defined WAV_96K_SUPPORT
	if (global_vari.audio_source == MEDIA_SDMMC)
	{
		if (app_main_data.playing_stream_sample_rate > 48000 || app_main_data.playing_stream_sample_bits == 32 || app_main_data.playing_stream_sample_bits == 8)
		{
			return WAVE_READ_ERR;
		}
	}
	else
	{
		if (app_main_data.playing_stream_sample_rate > 96000 || app_main_data.playing_stream_sample_bits == 32 || app_main_data.playing_stream_sample_bits == 8)
		{
			DBG_Printf("playing_stream_sample_rate = %d\n\r", app_main_data.playing_stream_sample_rate);
			return WAVE_READ_ERR;
		}
	}	
#else
	if (app_main_data.playing_stream_sample_rate > 48000 || app_main_data.playing_stream_sample_bits == 32 || app_main_data.playing_stream_sample_bits == 8)
	{
		return WAVE_READ_ERR;
	}
#endif


	U32 temp;
	U32 temp1;
	U32 offset;


	//get the current stream positon
	offset = audio_env.stream_fifo.rLen;

	temp = audio_env.stream_fifo.stream_len;

	DBG_Printf("file len:%d\n\r", temp);
	delayms(1);
	DBG_Printf("music len:%d\n\r", pwave->bytes);
	delayms(1);

	if (pwave->bytes == 0)
	{
		return WAVE_READ_ERR;
	}

	temp1 = offset + pwave->bytes;
	if (temp > temp1)
	{
		audio_env.stream_fifo.stream_len = temp1;
		pMediaRead->lenth = temp1;

		delayms(10);
		DBG_Printf("wav stream len changed:%d\n\r", temp1);
		delayms(1);
	}


	app_main_data.playing_stream_media_offset = offset;



	return WAVE_SUCCESS;
}

U32 app_dec_wave_get_pos (U32 time)
{
	U32 pos;
	pos = ((time * app_main_data.playing_stream_bitrate) / pwave->nBlockAlign) * pwave->nBlockAlign + pwave->data_pos;
	return pos;
}

U32 app_dec_wave_adjust_pos (U32 pos)
{
	U32 tmp;


	tmp = pos;
	wav_bytes_played = pos;


	tmp = (tmp/(2048*6)) * (2048*6);   //for 24 bit
	wav_bytes_played = tmp;


	return tmp;
}

void app_wave_dec_init (void)
{
	int ret;

	DBG_Printf("%s\r\n", __func__);

	wRequestDecodeFrameExit = FALSE;

#ifdef FOR_ESD_PROTECT
	resume_trk_info.need_resume_play = FALSE;
#endif

	wav_bytes_played = 0;

	ret = xa_wave_dec_init();
	if (wRequestDecodeFrameExit)
	{
		DBG_Printf("wav dec exit\r\n");
		return;
	}

	if (ret)
	{
		DBG_Printf("wav init fail\r\n");

		if ( (skip_key_flag == enMP3_PLAY_FLAG) || (skip_key_flag == enMP3_SKIPF_FLAG) )
		{
			uMsgSend (UI_CD_INTERFACE, UI_MP3_INIT_FAIL, 0);
		}
		else
		{
#ifdef SUPPORT_MESSAGE_DISPLAY
			//for displaying "Not Support" on LCD 
			uMsgSend (UI_CD_INTERFACE, UI_MP3_INIT_FAIL, 0);
#else
			uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_B, 0);
#endif
		}
	}
	else
	{

		if (app_main_data.playing_stream_sample_bits == 24)
		{
			app_main_data.audio_src_bit = AUDIO_SRC_24BIT;
		}
		else if (app_main_data.playing_stream_sample_bits == 16)
		{
			app_main_data.audio_src_bit = AUDIO_SRC_16BIT;
		}
		else
		{
			app_main_data.audio_src_bit = AUDIO_SRC_UNKNOWBIT;
#ifdef SUPPORT_MESSAGE_DISPLAY
			//for displaying "Not Support" on LCD 
			uMsgSend (UI_CD_INTERFACE, UI_MP3_INIT_FAIL, 0);
#else
			uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_B, 0);
#endif			
			return;
		}


		skip_key_flag = enMP3_PLAY_FLAG;

		DBG_Printf("wav init ok\r\n");
		delayms(10);
		uMsgSend (UI_CD_INTERFACE, UI_MP3_INIT_SUCCESS, 0);

	}
}

void app_wave_dec_fini(int reason)
{
	DBG_Printf("%s\r\n", __func__);
	if (reason == DECODE_END)
	{
		DBG_Printf("wav stream eof\r\n");

		skip_key_flag = enMP3_PLAY_FLAG;

		//playing to stream end
		uMsgSend (UI_CODEC, UI_DECODE_STREAM_EOF, 0);
	}
}


int xa_wave_dec_frame(U8 **pout,U16 *plen)
{
	U32 i_bytes_read,rlen;
	U8 *pb_out_buf = pwave->out_buf;

	U32 eventMask;
	U8 *read_buf = (U8 *) pb_out_buf;	//codec_malloc (WAVE_DECODE_OUTPUT_BYTES);
	unsigned int i, j, k, ui_nsamples;
	U8 ret;
	
	do 
	{


		i_bytes_read = 0;
		ret = DECODE_SUCCESS;
		do
		{
			rlen = xa_fread(read_buf+i_bytes_read, 1, WAVE_DECODE_OUTPUT_BYTES-i_bytes_read, NULL);

			if (rlen == 0)
			{
				*pout = pb_out_buf;
				*plen = i_bytes_read;				
				ret =  WAVE_READ_ERR;
				goto WAV_DECODE_EXIT;
			}
			i_bytes_read += rlen;
		} while(i_bytes_read != WAVE_DECODE_OUTPUT_BYTES && (wav_bytes_played+i_bytes_read < pwave->bytes));
		
		wav_bytes_played += i_bytes_read;
		k = i_bytes_read;

		
#if defined SRC_NEW_VER

	#ifdef WAV_96K_SUPPORT
		//for test 96k sample rate
		if ( (dec_src_flag == 96000) || (dec_src_flag == 88200) )
		{
			ui_nsamples = i_bytes_read / (4*2); //pwave->nBlockAlig:4 bytes


			for (i=0,j=0,k=0; i<ui_nsamples; i++,j+=(4*2))
			{
				//96k sample rate to 48k sample rate
				pb_out_buf[k++] = read_buf[j+0];
				pb_out_buf[k++] = read_buf[j+1];

				pb_out_buf[k++] = read_buf[j+2];
				pb_out_buf[k++] = read_buf[j+3];
			}
		}
		else
	#endif

#elif defined OPTEK_SRC_VER

	#if (defined WAV_192K_SUPPORT && defined AUDIO_192K_DOWN_SAMPLE)
		//for test 192k sample rate
		if ( (dec_src_flag == 96000*2) || (dec_src_flag == 88200*2) || (dec_src_flag == 64000*2))
		{
			#ifdef AUDIO_192K_DOWN_48K_SAMPLE
			ui_nsamples = i_bytes_read / ((4*2)*2); //pwave->nBlockAlig:4 bytes
			

			for (i=0,j=0,k=0; i<ui_nsamples; i++,j+=((4*2)*2))
			{
				//192k sample rate to 48k sample rate
				pb_out_buf[k++] = read_buf[j+0];
				pb_out_buf[k++] = read_buf[j+1];
				
				pb_out_buf[k++] = read_buf[j+2];
				pb_out_buf[k++] = read_buf[j+3];
			}
			#else
			ui_nsamples = i_bytes_read / (4*2); //pwave->nBlockAlig:4 bytes
			

			for (i=0,j=0,k=0; i<ui_nsamples; i++,j+=(4*2))
			{
				//192k sample rate to 96k sample rate
				pb_out_buf[k++] = read_buf[j+0];
				pb_out_buf[k++] = read_buf[j+1];
				
				pb_out_buf[k++] = read_buf[j+2];
				pb_out_buf[k++] = read_buf[j+3];
			}			
			#endif //AUDIO_192K_DOWN_48K_SAMPLE

		}
	#endif //(defined WAV_192K_SUPPORT && defined AUDIO_192K_DOWN_SAMPLE)

	#if (defined WAV_96K_SUPPORT && defined AUDIO_96K_DOWN_SAMPLE)
		//for test 192k sample rate
		else if ( (dec_src_flag == 96000) || (dec_src_flag == 88200) || (dec_src_flag == 64000))
		{
			ui_nsamples = i_bytes_read / (4*2); //pwave->nBlockAlig:4 bytes
			

			for (i=0,j=0,k=0; i<ui_nsamples; i++,j+=(4*2))
			{
				//192k sample rate to 96k sample rate
				pb_out_buf[k++] = read_buf[j+0];
				pb_out_buf[k++] = read_buf[j+1];
				
				pb_out_buf[k++] = read_buf[j+2];
				pb_out_buf[k++] = read_buf[j+3];
			}
			

		}
	#endif //(defined WAV_96K_SUPPORT && defined AUDIO_96K_DOWN_SAMPLE)
#endif //SRC_NEW_VER

		*pout = pb_out_buf;
		*plen = k;
		if (wav_bytes_played >= pwave->bytes)
		{	
			//decode end
			ret = DECODE_END;
		}

	} while(0);

WAV_DECODE_EXIT:

	return ret;

} /* End xa_main_process() */


U32 app_wav_get_data_pos (void)
{
	return pwave->data_pos;
}

#endif


