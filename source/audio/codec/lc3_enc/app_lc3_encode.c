#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"
#include "mem_reloc.h"
#include "app_main.h"

#if 0//def LC3_ENCODE_ENABLE
extern U32 __lc3_enc_text_rom_start[];
extern U32 __lc3_enc_text_ram_start[];
extern U32 __lc3_enc_text_ram_end[];

extern U32 __lc3_dec_text_rom_start[];
extern U32 __lc3_dec_text_ram_start[];
extern U32 __lc3_dec_text_ram_end[];

static int encode_txet_buf[17*1024/4];// __attribute__(( aligned (64) ));	
static U32 text_len;
int app_lc3_encoding_code_text_copy_init(void)
{
	int len;
	
	text_len = 17*1024;//(__lc3_dec_text_ram_end - __lc3_dec_text_ram_start)*4;
	CFasm_memcpy(encode_txet_buf, __lc3_enc_text_rom_start,text_len);

	app_main_data.iram_code_flag = SHARE_IRAM_UNKNOW_CODE;
	app_lc3_encoding_code_text_copy();
}

//int app_lc3_encoding_code_text_copy(void) __INTERNAL_RAM_TEXT;
int app_lc3_encoding_code_text_copy(void)
{
	int len;

    //if (app_main_data.iram_code_flag != SHARE_IRAM_LC3_ENCODE_CODE)
    {
		len = __lc3_enc_text_ram_end - __lc3_enc_text_ram_start;
		CFasm_memcpy(__lc3_enc_text_ram_start, __lc3_enc_text_rom_start, len*4);
        app_main_data.iram_code_flag = SHARE_IRAM_LC3_ENCODE_CODE;   
    }	
}

int app_lc3_part_encoding_code_text_copy(void) __INTERNAL_RAM_TEXT;
int app_lc3_part_encoding_code_text_copy(void)
{
//	int len;
    if (app_main_data.iram_code_flag != SHARE_IRAM_LC3_ENCODE_CODE)
    {
	//	len = __lc3_dec_text_ram_end - __lc3_dec_text_ram_start;
		CFasm_memcpy(__lc3_enc_text_ram_start, encode_txet_buf,text_len);
        app_main_data.iram_code_flag = SHARE_IRAM_LC3_ENCODE_CODE;   
    }	
}
#endif
#if 0//def LC3_ENCODE_ENABLE
#include "regmap.h"

#include "mem_reloc.h"

#include "message.h"

#include "app_sdram.h"
#include "audio_dec.h"
#include "app_dac.h"
#include "fifo.h"
#include "app_main.h"
#include "app_lcd.h"
#include "app_lc3_encode.h"
#include "optek_src.h"

#include "xa_error_standards.h"

#ifdef CD_ENABLE
#include "app_cdrom.h"
#endif


#if 0
#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#endif


extern U8 __TEXT_START38[];
extern U8 __TEXT_ROM_START38[];
extern U8 __TEXT_START38_END[];


#if 0
FIFO record_fifo;
#else
//FIFO record_fifo;
extern FIFO record_fifo;
//FIFO record_fifo __attribute__ ((section (".record_fifo_bss")));
#endif

//FIFO record_in_fifo;
//extern FIFO record_in_fifo;

#define RECORD_BUF_POINTER_USED

#ifdef RECORD_BUF_POINTER_USED

//U8 *record_buf = (U8 *)RECORD_FIFO_BUF_START;

#define RECORD_BUF_SIZE    (RECORD_FIFO_BUF_END - RECORD_FIFO_BUF_START)

#else

U8 record_buf[RECORD_BUF_SIZE];
//U8 record_buf[RECORD_BUF_SIZE] __attribute__ ((section (".record_fifo_bss")));

#endif


//U8 record_buf_overflow;
extern U8 record_buf_overflow;
//U8 record_buf_overflow __attribute__ ((section (".sdram_bss")));


#ifdef AUDIO_OUT_BUF_IS_HALF_OF_MP3_ENC_FRAME_SAMPLE

static U8 pcm_in_data_index;
//U8 pcm_in_data_buf[MP3_ENC_FRAME_SAMPLE*4];
static U8 *pcm_in_data_buf;

//U32 enc_data_len;
#else

#if 0//for testing
//U8 pcm_in_data_buf[2];
U8 pcm_in_data_buf[1152*4];
#endif

#endif

static U8 lc3_enc_1st_frame;


void app_lc3_encode_fifo_init (void)
{
	int i;
	U8 *pBuf1;
	int size = RECORD_FIFO_BUF_END - RECORD_FIFO_BUF_START;

	lc3_enc_1st_frame = TRUE;

	pBuf1 = (U8 *)RECORD_FIFO_BUF_START;
	size = RECORD_FIFO_BUF_END - RECORD_FIFO_BUF_START;
	fifo_init (&record_fifo, (U8 *)pBuf1, size, TRUE);
	
#if 0
	pBuf1 = (U8 *)RECORD_DATA_BUF_START;
	size = RECORD_DATA_BUF_END - RECORD_DATA_BUF_START;
	fifo_init (&record_in_fifo, (U8 *)pBuf1, size, TRUE);
#endif

#if 0
	int *pBuf = pcm_in_data_buf;
	for (i=0; i<(1152*4)/(4*72); i++)
	{
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;

		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;

		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;

		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;
		*pBuf++ = 0x7fff7fff;

		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;

		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;

		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;

		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
		*pBuf++ = 0x80018001;
	}
#endif
}

/*******************************************************************************
 *
 *   FUNCTION NAME - app_mp3_encode_init
 *
 *   ARGUMENTS     - :
 *
 *   RETURN VALUE  - Error type
 *
 *******************************************************************************
 *
 *   DESCRIPTION
 *   This function initializes the MP3 encoder. It copies the MP3 encoder code
 *   to SRAM and changes clock to higher frequency.
 ******************************************************************************/
BOOL app_lc3_encode_init (void)
{
#if 1
	//eEncReturnCode eRetCode;
	U8 *pucSource, *pucDest;
	U32 count;
	U32 length;
	int ret;

	DBG_Printf("%s\n\r", __func__);

	debug_pin_init();

	pcm_in_data_index = 0;
	//enc_data_len = 0;
	pcm_in_data_buf = (U8 *)optek_src_buf;

	record_buf_overflow = FALSE;
	app_main_data.record_time = 0;
	//app_main_data.record_timeset = 0;

	lc3_enc_1st_frame = TRUE;

#if 0
	app_main_data.playing_stream_sample_rate = 44100;
	app_main_data.record_bitrate = 160000;//128000;//160000;//192000;
#else
	app_main_data.playing_stream_sample_rate = 48000;
	app_main_data.record_bitrate = 128000;

	//app_main_data.playing_stream_sample_rate = 48000;
	//app_main_data.record_bitrate = 160000;

	//app_main_data.playing_stream_sample_rate = 48000;
	//app_main_data.record_bitrate = 192000;
#endif

	app_lc3_encode_fifo_init();

	ret = xa_lc3_enc_main_init ();
	if (ret = XA_NO_ERROR)
	{
		DBG_Printf("lc3 enc init error:%d\n\r", ret);
	}

	delayms(100);
	DBG_Printf("%s exit\n\r", __func__);

	return TRUE;
#else
	return FALSE;
#endif
}


/*******************************************************************************
 *
 *   FUNCTION NAME - xa_mp3_enc_frame
 *
 *   ARGUMENTS     - psMessage: pointer to the input buffer
 *
 *   RETURN VALUE  - Error type
 *
 *******************************************************************************
 *
 *   DESCRIPTION
 *   This function encodes one frame of data that is sent to the encoder module.
 *   The function assumes that the buffer given is continuous and does not wrap.
 *   The buffer is also assumed to have 1152 stereo samples. The input is
 *   assumed to the interleaved and so it is de-interleaved and then encoded.
 *   This function also increases the system clock to have sufficient MIPS for
 *   encoding. Once the encoding is over, the system clock is decreased back to
 *   the previous value.
 *
 ******************************************************************************/

static U16 ticks = 0;

BOOL app_ad_pcm_lc3_encode_frame (I16 *ppBuf, I32 len) __INTERNAL_RAM_TEXT;
BOOL app_ad_pcm_lc3_encode_frame (I16 *ppBuf, I32 len)
{
#if 1
	//U8 uiIndex = 0;
	//I32 dwEncLen;
	U32 dwEncLen;
	U32 w_size, r_size;
	//static U8 ticks = 0;
	U8 *lc3_enc_outbuf;

	//release buf to receive
	//sDacOutIsrBuf[wDacOutIndex].wSize = 0;

	//DBG_Printf("Encode start %d\n\r");

	//Encode one frame


#if 0
	//for testing
	fifo_put_data(&record_in_fifo, ppBuf, len);

	//w_size = fifo_get_fifo_data_wlen(&record_in_fifo);
	//if (w_size < 0x100000/2)
	//if ( w_size < (RECORD_FIFO_BUF_END-RECORD_FIFO_BUF_START) / 2)
	//{
	//	return;
	//}
	r_size = fifo_get_fifo_data_len(&record_in_fifo);
	if (r_size < (MP3_ENC_FRAME_SAMPLE*4) )
	{
		//DBG_Printf("rec in buf underflow\n\r");
		return;
	}
	r_size = fifo_get_data (&record_in_fifo, pcm_in_data_buf, (MP3_ENC_FRAME_SAMPLE*4));

	//when lc3's encoding(sample rate:44.1kHz, bps:192k) one frame, the dwEncLen'value is 652, 712, 510 or 654 bytes.
	//when lc3's encoding(sample rate:48kHz, bps:192k) one frame, the dwEncLen'value is 576, 613, 539 or 576 bytes.
	xa_lc3_enc_frame((U16 *)pcm_in_data_buf, MP3_ENC_FRAME_SAMPLE, &lc3_enc_outbuf, &dwEncLen);
	return;
#endif


#if 0//def AUDIO_OUT_BUF_IS_HALF_OF_MP3_ENC_FRAME_SAMPLE
	DBG_Assert( (len == (MP3_ENC_FRAME_SAMPLE*2) ) );

	pcm_in_data_index++;
	if (pcm_in_data_index == 1)
	{
		CFasm_memcpy((void *)pcm_in_data_buf, (U16 *)ppBuf, (MP3_ENC_FRAME_SAMPLE*2));
		return TRUE;
	}
	else if (pcm_in_data_index == 2)
	{
		CFasm_memcpy((void *)(pcm_in_data_buf+(MP3_ENC_FRAME_SAMPLE*2)), (void *)ppBuf, (MP3_ENC_FRAME_SAMPLE*2));
		//xa_lc3_enc_frame((U16 *)pcm_in_data_buf, MP3_ENC_FRAME_SAMPLE, &lc3_enc_outbuf, &dwEncLen);
		pcm_in_data_index = 0;
	}
#else
	DBG_Assert( (len == (MP3_ENC_FRAME_SAMPLE*4) ) );
	int samples = len/4;
	xa_lc3_enc_frame((U16 *)ppBuf, samples, &lc3_enc_outbuf, &dwEncLen);
#endif


	app_main_data.playing_stream_pcm_sample += dwEncLen;
	//DBG_Printf("len:%d\n\r", dwEncLen);

	//DBG_Printf("Encode end\n\r");

	//set copyright bit
	//lc3enc_outbuf[3] |= 0x08;

	{
		U32	free_space;
		STREAM_ID s;

		free_space = fifo_get_free_space (&record_fifo);
		//DBG_Printf("free space =0x%x\r\n", free_space);

		if (lc3_enc_1st_frame)
		{
			lc3_enc_1st_frame = FALSE;
			fifo_put_data (&record_fifo, lc3_enc_header, sizeof(lc3_enc_header));
		}

		//put the data to enc fifo
		if (free_space >= dwEncLen)
		{
			fifo_put_data (&record_fifo, lc3_enc_outbuf, dwEncLen);
		}
		else
		{
			//encoded buf underflow
			//DBG_Assert (FALSE);
			//record_buf_overflow = TRUE;

			ticks ++;
			//if ( (ticks % 1000) == 0)
			//if ( (ticks % 10) == 0)
			{
				DBG_Printf("rec fifo buf overflow\n\r");
			}
		}

		//if (free_space < sizeof (record_buf)/2)
		//{
		//	there are enough encoded data in fifo, write it to media
		//	DBG_Assert (FALSE);
		//}
	}

	return TRUE;
#endif
}

#endif //MP3_ENCODE_ENABLE
