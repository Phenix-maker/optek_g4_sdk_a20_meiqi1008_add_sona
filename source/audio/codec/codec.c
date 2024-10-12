#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "fifo.h"
#include "audio_task.h"
#include "audio_com.h"
#include "codec.h"
#include "CFasm_memcpy.h"
#include "app_media_config.h"
#include "flash_boot.h"


#ifdef CD_ENABLE
#ifdef CD_SERVO_ENABLE
#include "app_servo.h"
#include "iso9660.h"
#endif
#endif

#ifdef MP3_ENCODING
#include "app_mp3_encode.h"
#endif

#ifdef TIME_SCALE_CODE
#include "tscale.h"
#endif


#ifdef MP3_DECODE_ENABLE
//#include "hihat_128_mp3.h"
//#include "hihat_16bit_wav.h"
#endif

#ifdef MP3_ENCODE_ENABLE
#include "hihat_wav_data.h"
#include "hihat_bps_128k_mp3.h"
#endif


#ifdef SBC_DECODE_ENABLE
#include "sbc_test_02_sbc.h"
#endif

#ifdef ATMOS_CODEC_ENABLE
#include "atmos_config.h"
	#if defined ATMOS_THD_MLP_OBJECT_AUDIO_TEST
	#include "atmos_detection_mlp.h"
	#elif defined ATMOS_THD_OBJECT_AUDIO_TEST
	#include "Zion_plus3db_96k_short_mat.h"
	#elif defined ATMOS_DDP_OBJECT_AUDIO_TEST
	#include "stresszeus_400_599_ec3.h"
	#elif defined ATMOS_MAT_OBJECT_AUDIO_TEST
	#include "32_Chan_16_48k_DRC_mat.h"
	#else
	#error "Must be defined!
	#endif

	#if defined ATMOS_THD_OBJECT_AUDIO_CHECK_OUT
	#include "Zion_plus3db_96k_short_mat_wav.h"
	#elif defined ATMOS_DDP_OBJECT_AUDIO_CCHECK_OUT
	#include "stresszeus_400_599_ec3_wav.h"
	#elif defined ATMOS_MAT_OBJECT_AUDIO_CHECK_OUT
	#include "32_Chan_16_48k_DRC_mat_wav.h"
	#else
	#error "Must be defined!
	#endif
#endif //ATMOS_CODEC_ENABLE

#ifdef DOLBY_DDPLUS71_DEC_ENABLE
//#define EC3_TEST_SOURC3_1
//#define EC3_TEST_SOURC3_2
//#define EC3_TEST_SOURC3_3
//#define EC3_TEST_SOURC3_4
//#define EC3_TEST_SOURC3_5

	#if defined DOLBY_DDPLUS71_DEC_TEST
	
	#if defined EC3_TEST_SOURC3_1
	#include "sample_ec3.h"
	#elif defined EC3_TEST_SOURC3_2
	#include "convsync_ec3.h"
	#elif defined EC3_TEST_SOURC3_3
	#include "8ch_addbsi_21_ec3.h"
	#elif defined EC3_TEST_SOURC3_4
	#include "6ch_addbsi_7_ac3.h"
	#elif defined EC3_TEST_SOURC3_5
	#include "stresszeus_400_599_ec3.h"
	#endif

	#if defined DOLBY_DDPLUS71_DEC_CHECK_OUT

	#if defined EC3_TEST_SOURC3_1
	#include "sample_ec3_pcm.h"
	#elif defined EC3_TEST_SOURC3_5
	#include "stresszeus_400_599_ec3_wav.h"
	#endif

	#endif

	#endif
#endif //DOLBY_DDPLUS71_DEC_ENABLE


#undef DOLBY_EC3_TEST_BY_USB
//#define DOLBY_EC3_TEST_BY_USB
//#define BYTE_SWAP

extern U8 CODEC_MEMORY_START[];

//#undef TIME_SCALE_CODE
//#define POINTER_USED


#define MP3_ENC_FRAME_SAMPLE    1152

#ifdef MP3_ENCODE_ENABLE
char codec_enc_in_buf[MP3_ENC_FRAME_SAMPLE*4];
#endif

unsigned long total_read_bytes;
unsigned long codec_read_len;
unsigned long codec_dec_cnt;
char *pAudioOrginalOut;

U32 g_w_codec_mem_used;
U8 wNoMoreFrames;

#ifdef MP3_FILE_COPY
extern U8 mp3_file_copy;
#endif


long xa_ftell(void *ptr)
{
	return	AUDIO_FIFO_STREAM_FIFO_RLEN();
}

int xa_feof(void *stream)
{
	return (AUDIO_FIFO_STREAM_FIFO_RLEN()>=AUDIO_FIFO_STREAM_STREAM_LEN());
}

long xa_flength(void *ptr)
{
	return	AUDIO_FIFO_STREAM_STREAM_LEN();
}

int xa_fseek( void *stream, long offset, int origin )
{
#if (defined USB_HOST_ENABLE || defined SD_ENABLE)
	U32 pos,curr_pos,len,new_pos,wlen;
	TX_INTERRUPT_SAVE_AREA;
	int ret;
	U32 fifo_buf_len;

	DBG_Printf("%s\n\r", __func__);
	
	if (AUDIO_FIFO_STREAM_IS_VAILD() == FALSE)
	{
		DBG_Printf("XA Seek Fatal Err1!\n\r");
		decode_exit_req_set(); //quit from codec
		return -1;
	}

	curr_pos = AUDIO_FIFO_STREAM_FIFO_RLEN();
	len = AUDIO_FIFO_STREAM_STREAM_LEN();
	wlen = AUDIO_FIFO_STREAM_FIFO_WLEN();
	fifo_buf_len = AUDIO_FIFO_STREAM_FIFO_LEN();

	switch (origin)
	{
		case 0: //SEEK_SET
			//DBG_Assert (pos >= offset); //the seek function can only seek forward
			pos = offset;
			break;
			
		case 1: //SEEK_CURR
			pos = curr_pos + offset;
			break;
			
		case 2: //SEEK_END
			pos = len - offset;
			break;
		
		default: //ERR
			DBG_Assert(FALSE);
			break;
	}

	if (pos > len)
	{
		DBG_Printf ("curr pos=%d offset=%d org=%d err\n\r", curr_pos, offset, origin);
		DBG_Printf ("pos=%d > len=%d err\n\r", pos, len);
		//if to jump next track, the set will be dead.
		//app_dac_mute_enable();
		//uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_F, 0);

		return -1;
		//return 0;
	}

	if (pos == curr_pos)
	{
		return 0;
	}
	else if (pos < curr_pos)
	{
		//DBG_Puts ("Seek b\n\r");
		//DBG_Printf ("Seek backward:0x%x\n\r", curr_pos - pos);
	}
	else
	{
		//DBG_Puts ("Seek f\n\r");
		//DBG_Printf ("Seek forward:0x%x\n\r", pos - curr_pos);
	}

	ret = AUDIO_FIFO_STREAM_SEEK(pos);

	if (ret == 0)
	{
		return 0;
	}
	else
	{
		//if (pos > (wlen + 0x20000) || pos < curr_pos)
		//if (pos < curr_pos)
		if (pos > (wlen + fifo_buf_len) || (pos < curr_pos) )
		{	
			//no read file from usb
			//app_nav_lock_stream_mutex ();
			//app_nav_unlock_stream_mutex ();
			app_media_mute_enable();
			STREAM_MUTEX_LOCK;

			AUDIO_FIFO_STREAM_FLUSH ();

			//new_pos = (pos / 0x2000) * 0x2000; 
			new_pos = (pos / fifo_buf_len) * fifo_buf_len; 
			wlen = new_pos;

			//only for usb
			AUDIO_FIFO_STREAM_CREATE(new_pos, len, app_media_data.playing_index, audio_env.stream_fifo.stream_margin);

			app_media_mute_disable ();
			STREAM_MUTEX_UNLOCK;

			uHddMsgSend (HDD_READ_FILE, new_pos, 0);
			//DBG_Assert(FALSE);
		}		
	}
	
	while (pos > wlen)
	{
		AUDIO_FIFO_STREAM_SEEK(wlen); //feed buf

		#if 1
		//HDD_READ_FILE_EVENT_GET;
		HDD_READ_FILE_EVENT_TIME_OUT_GET(pdMS_TO_TICKS(25));
		#else
		app_nav_sleep(50);
		#endif

		/*
		if ( (decode_exit_req_get() == TRUE) )
		{
			DBG_Printf("XA Seek Fatal Err2!\n\r");
			return 0;
		}
		*/
		
		wlen = AUDIO_FIFO_STREAM_FIFO_WLEN();
	}

	ret = AUDIO_FIFO_STREAM_SEEK(pos);
	DBG_Assert(ret == 0);
	
	DBG_Printf("%s exit\n\r", __func__);
	
	return 0;

#else //(defined USB_HOST_ENABLE || defined SD_ENABLE)

	return 0;

#endif //(defined USB_HOST_ENABLE || defined SD_ENABLE)
}

#ifdef FLAC_DEC
U32 file_len_cnt = 0;
int xa_flac_fseek( void *stream, long offset, int origin )
{
	STREAM_ID s;
	U32 pos,curr_pos,len,new_pos,wlen;
	TX_INTERRUPT_SAVE_AREA;
	int ret;

	s = app_sdram_stream_get_rstream();
		
	if (s == INVALID_STREAM_ID) {
		DBG_Printf("XA Read Fatal Err!!!\n\r");
		wRequestDecodeFrameExit = TRUE;		/*Quit from codec*/
		return -1;
	}

	curr_pos = app_sdram_stream_get_stream_pos (s);
	len = app_sdram_stream_get_stream_len (s);
	wlen = app_sdram_stream_get_stream_wlen (s);

	switch (origin)
	{
		case 0:				//SEEK_SET
			//the seek function can only seek forward
			pos = offset;
			break;
		case 1:				//SEEK_CURR
			pos = curr_pos + offset;
			break;
		case 2: 			//SEEK_END
			pos = len - offset;
			break;
		default:			//ERR
			break;
	}

	if (pos > len)
	{
		DBG_Printf ("curr pos=%d offset=%d org=%d err\n\r", curr_pos, offset,origin);
		DBG_Printf ("pos=%d > len=%d err\n\r", pos,len);
		//if to jump next track, the set will be dead.
		return -1;
	}

	while (pos > wlen)
	{
		app_sdram_stream_rstream_seek (s, wlen);			//feed buf

		{
			U32 eventMask;
			
			tx_event_flags_get(&event_grop, AUDIO_DECODE_EVENT, TX_OR_CLEAR, &eventMask, TX_WAIT_FOREVER);
			if (wRequestDecodeFrameExit == TRUE)
			{
				return -1;
			}
		}
		
		wlen = app_sdram_stream_get_stream_wlen (s);
	}


	ret = app_sdram_stream_rstream_seek (s, pos);

	return 0;
}
#endif

int xa_fread_bt(void *buffer, int size, int count, void *fp)
{
	//STREAM_ID s;
	U32 len;
	U32 bytes;
	
	bytes = count * size;

	if (bytes == 0)
	{
		return 0;
	}

	//while (1)
	{
		len = AUDIO_FIFO_STREAM_DATA_LEN();

		if (len < bytes)
		{
			bytes = len;
		}

		len = AUDIO_FIFO_STREAM_GET_DATA( (unsigned char *)buffer, bytes);

		return len;
	}
}

#ifndef DOLBY_DDPLUS71_DEC_CHECK_OUT
int xa_fread(void *buffer, int size, int count, void *fp)
{
	U32 bytes;
	U32 len;
	U32 to = 100;

	bytes = size*count;

	if (bytes == 0)
	{
		return 0;
	}

	while(1)
	{
		#if 0//for test
		if (bytes > 512)
		{
			bytes = 512;
		}
		#endif

		len = AUDIO_FIFO_STREAM_GET_DATA((U8 *)buffer, bytes);

		if (len > 0)
		{
			#ifdef BYTE_SWAP
			int i;
			U16 *pU16 = buffer;
			
			for (i=0; i<(len/2); i++)
			{
				pU16[i] = (pU16[i] << 8) | (pU16[i] >> 8);
			}
			#endif

			return len/size;
		}
		else
		{
			/************steam underflow****************/
			DBG_Printf ("Buf underflow 1\n\r");

			#if 0
			if (--to == 0)
			{
				DBG_Printf("fs read data timeout!\r\n");
				return 0;
			}
			#endif

			if ( (AUDIO_FIFO_STREAM_STREAM_LEN() != STREAM_LENTH_UNKNOWN) &&
				(AUDIO_FIFO_STREAM_STREAM_LEN() == AUDIO_FIFO_STREAM_FIFO_WLEN()) )
			{
				DBG_Printf ("xa_fread return 1\n\r");
				return 0;
			}

			#if 1
			//clear event first
			//HDD_READ_FILE_EVENT_CLR;
			#ifdef USB_HOST_ENABLE
			HDD_READ_FILE_EVENT_GET;
			#endif
			#else
			app_nav_sleep(25);
			#endif

			if (!AUDIO_FIFO_STREAM_IS_VAILD())
			{
				DBG_Printf ("xa_fread return 2\n\r");
				return 0;
			}
			
			/*
			if ( (decode_exit_req_get() == TRUE) )
			{
				DBG_Printf ("xa_fread return 3\n\r");
				return 0;
			}
			*/

			//DBG_Printf ("Buf underflow 2\n\r");
		}
	}
}
#else
int xa_fread(void *buffer, int size, int count, void *fp)
{
	char *pSrcData = NULL;

#ifdef MP3_DECODE_ENABLE
	total_read_bytes = sizeof(hihat_128_mp3_data);
	char *pSrcStart = hihat_128_mp3_data;
#elif defined MP3_ENCODE_ENABLE
	total_read_bytes = sizeof(hihat_wav_data);
	char *pSrcStart = hihat_wav_data;
#elif defined LC3_DECODE_ENABLE
    total_read_bytes = sizeof(hifi3_abba_44100_80k_dms_7_5_lc3);
	char *pSrcStart = hifi3_abba_44100_80k_dms_7_5_lc3;
#elif defined SBC_DECODE_ENABLE
	total_read_bytes = sizeof(sbc_test_02_sbc);
	char *pSrcStart = sbc_test_02_sbc;
#elif defined ATMOS_CODEC_ENABLE

	#if defined ATMOS_THD_MLP_OBJECT_AUDIO_TEST
	total_read_bytes = sizeof(atmos_detection_mlp);
	char *pSrcStart = atmos_detection_mlp;
	#elif defined ATMOS_THD_OBJECT_AUDIO_TEST
	total_read_bytes = sizeof(atmos_zion_plus3db_96k_short_mat);
	char *pSrcStart = atmos_zion_plus3db_96k_short_mat;
	#elif defined ATMOS_DDP_OBJECT_AUDIO_TEST
	total_read_bytes = sizeof(atmos_stresseus_400_599_ec3);
	char *pSrcStart = atmos_stresseus_400_599_ec3;
	#elif defined ATMOS_MAT_OBJECT_AUDIO_TEST
	total_read_bytes = sizeof(atmos_32_chan_16_48k_drc_mat);
	char *pSrcStart = atmos_32_chan_16_48k_drc_mat;
	#else
	#error "Must be defined!
	#endif
	
#elif defined DOLBY_DDPLUS71_DEC_TEST
	#if defined EC3_TEST_SOURC3_1
	total_read_bytes = sizeof(sample_ec3);
	char *pSrcStart = sample_ec3;
	#elif defined EC3_TEST_SOURC3_2
	total_read_bytes = sizeof(convsync_ec3);
	char *pSrcStart = convsync_ec3;
	#elif defined EC3_TEST_SOURC3_3
	total_read_bytes = sizeof(ec3_8ch_addbsi_21_ec3);
	char *pSrcStart = ec3_8ch_addbsi_21_ec3;
	#elif defined EC3_TEST_SOURC3_4
	total_read_bytes = sizeof(ac3_6ch_addbsi_7_ac3);
	char *pSrcStart = ac3_6ch_addbsi_7_ac3;
	#elif defined EC3_TEST_SOURC3_5
	total_read_bytes = sizeof(atmos_stresseus_400_599_ec3);
	char *pSrcStart = atmos_stresseus_400_599_ec3;
	#else
	char *pSrcStart = FALSE;
	#endif

#else

	char *pSrcStart = FALSE;
#endif

	int byte_size = count * size;

	REG_MISC_STATUS = 0x50000001;
	REG_MISC_STATUS = codec_read_len;
	REG_MISC_STATUS = 0x50000002;

	if (codec_read_len > 4096)
	{

	}

	if (pSrcStart == NULL)
	{
		return 0;
	}

	pSrcData = pSrcStart + codec_read_len;
	CFasm_memcpy((void *)buffer, (void *)pSrcData, byte_size);

	codec_read_len += count * size;
    if (codec_read_len > total_read_bytes)
	{
        codec_read_len = total_read_bytes;
		return 0;
	}

	return byte_size;
}
#endif


#ifdef SPDIF_ENABLE
extern U8 SPDIF_STREAM_START[];
extern U8 SPDIF_STREAM_END[];

void app_nav_spdif_stream_reinit (void)
{
	U8 *pStreamBuf;
	U32 size;
	//STREAM_ID s;
	long len;

	DBG_Printf ("Spdif stream reinit\n\r");

	if ( !(global_vari.audio_source == SOURCE_HDMI) )
	{
		Spdif_rcv_disable();
	}
	
	//app_cmd_DecoderExit();
	decode_exit_req_set();
	
	//DMA_Channel3_Disable();
	//DMA_Channel1_Disable();

	pStreamBuf = SPDIF_STREAM_START;
	size = SPDIF_STREAM_END - SPDIF_STREAM_START;

	AUDIO_FIFO_STREAM_FLUSH();
	
	AUDIO_FIFO_STREAM_OPEN(pStreamBuf, size);
	
	AUDIO_FIFO_STREAM_CREATE(0,STREAM_LENTH_UNKNOWN,0,0);
	
	if ( !(global_vari.audio_source == SOURCE_HDMI) )
	{
		Spdif_rcv_enable();
	}

	spdif_stream_preProcess_init();
}

int xa_spdif_fread(void *buffer, int size, int count, void *fp) __INTERNAL_RAM_TEXT;
int xa_spdif_fread(void *buffer, int size, int count, void *fp)
{
	//STREAM_ID s;
	U32 len;
	U32 bytes;
	
	bytes = count * size;

	if (bytes == 0)
	{
		return 0;
	}

	while (1)
	{
		len = AUDIO_FIFO_STREAM_DATA_LEN();

		if (len >= bytes)
		{
			len = AUDIO_FIFO_STREAM_GET_DATA( (unsigned char *)buffer, bytes);
		}
		else
		{
			len = 0;

			/*
             * func_wo_param_msgSend (app_nav_spdif_stream_reinit);
             * system_msgSend(UI_SPDIF_STREAM_UNDERFLOW);
			 */

			DBG_Printf ("Spdif Rx underFlow 1\n\r");
		}

		#ifdef BYTE_SWAP
		U16 *pBuf = buffer;
		int i;

		for (i=0; i<bytes/2; i++)
		{
			*pBuf = ( (*pBuf>>8) | (*pBuf<<8) );
			pBuf++;
		}
		#endif

		return len;
	}
}
#endif //SPDIF_ENABLE


int xa_dolby_ac3_fread(void *buffer, int size, int count, void *fp) __INTERNAL_RAM_TEXT;
int xa_dolby_ac3_fread(void *buffer, int size, int count, void *fp)
{
	U32 ret = 0;

	if (global_vari.audio_source == SOURCE_USB_MSC)
	{
		ret = xa_fread(buffer, size, count, fp);
	}
	else
	{
		#ifdef SPDIF_ENABLE
		ret = xa_spdif_fread(buffer, size, count, fp);
		#endif
	}

	return ret;
}

int xa_audio_parameter_get(U32 sample_rate, U32 input_channel, U32 output_channel)
{
	DBG_Printf("Input sample rate: %d\n\r", sample_rate);
	DBG_Printf("Input channel: %d\n\r", input_channel);
	DBG_Printf("Output channel: %d\n\r", output_channel);

	#if 0
	if (input_channel == 0)
	{
		input_channel = 2;
	}
	audio_in_stream.channel = input_channel;
	#endif
	audio_out_stream.input_channel = input_channel;
	audio_out_stream.sample_rate = sample_rate;
	audio_out_stream.decode_out_channel = output_channel;
	audio_out_stream.sample_speed = 1;
}

void xa_fwrite(void *buffer, int size, int length, void *pointer)
{

}


U32 codec_get_mem_used(void)
{
	return g_w_codec_mem_used;
}

void codec_set_mem_used(U32 val)
{
	g_w_codec_mem_used = val;
}



int audio_data_check_out_status(int *pSrc, int *pDest, int byte_size)
{
	int ret = 0;
	int i;
	int temp1, temp2;

	codec_dec_cnt++;
	for (i=0; i<byte_size/4; i++)
	{
		temp1 = *pSrc++;
		temp2 = *pDest++;
		if (temp1 != temp2)
		{
			ret = 1;
			DBG_Assert(FALSE);
			break;
		}
	}

	return ret;
}

int audio_data_byte_check_out_status(char *pSrc, char *pDest, int byte_size)
{
	int ret = 0;
	int i;
	int temp1, temp2;

	codec_dec_cnt++;

	for (i=0; i<byte_size; i++)
	{
		temp1 = *pSrc++;
		temp2 = *pDest++;
		if (temp1 != temp2)
		{
			ret = 1;
			DBG_Assert(FALSE);
			break;
		}
	}
	
	return ret;
}

int audio_data_convert_from_32bit_to_24bit(int *pSrc, int byte_size)
{
    int i, j;

    char *pChar1 = pSrc;
    char *pChar2;

    if (pSrc == NULL)
    {
        return FALSE;
    }    

    for (i=0; i<byte_size/4; i++)
    {
        pChar2 = pSrc;
        pSrc++;
        for (j=0; j<3; j++)
        {
            *pChar1++ = pChar2[j+1];
        }
    }
	
    return TRUE;
}

enum 
{
	WIDTH_SIZE_UNKOWN,
	WIDTH_SIZE_8_BIT = 8,
	WIDTH_SIZE_16_BIT = 16,
	WIDTH_SIZE_24_BIT = 24,
	WIDTH_SIZE_32_BIT = 32,
};

void audio_data_direct_out_process( int *pBuf, U16 len, U16 width_size, U16 out_channel_number ) __INTERNAL_RAM_TEXT;
void audio_data_direct_out_process( int *pBuf, U16 len, U16 width_size, U16 out_channel_number)
{
	short int *pOut16_buf = (short *)pBuf;
	short int *pSrc = (short *)pBuf;
	int *pIn32_buf = pBuf;
	int i;

	if (width_size == WIDTH_SIZE_32_BIT)
	{
		//convert to 16-bit
		for (i=0; i<len/4; i++)
		{
			*pOut16_buf++ = (*pIn32_buf++)>>16;
		}

		len = len/2;
		
		optek_src_output((short *)pSrc, len, out_channel_number);
	}
	else if (width_size == WIDTH_SIZE_16_BIT)
	{
		optek_src_output((short *)pSrc, len, out_channel_number);
	}
	else if (width_size == WIDTH_SIZE_8_BIT)
	{
		DBG_Assert(FALSE);
	}
	else
	{
		DBG_Assert(FALSE);
	}
}
