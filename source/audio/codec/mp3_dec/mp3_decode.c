#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"


#ifdef MP3_DECODE_ENABLE

//#include "hw_da_pp.h"

#include "fifo.h"
#include "app_sdram.h"
#include "app_main.h"
#include "app_media.h"
//#include "app_dac.h"
//#include "app_cmd.h"
#include "mp3_decode.h"
#include "audio_com.h"
#include "audio_codec.h"
#include "codec.h"
#include "tools.h"
#include "id3v2.h"
//#include "app_optek_d2as.h"

//#include "optek_link.h"

#include "xa_error_standards.h"
//#include "xa_error_handler.h"
#include "xa_type_def.h"

#ifdef TIME_SCALE_CODE
#include "tscale.h"
int tscaleSpeed;
#endif

#include "app_media_com.h"
#include "app_media_process.h"


//#undef TIME_SCALE_CODE

#define app_audio_clock_freq_setting  audio_clock_freq_set

#define ukParmSend                    func_w_one_param_msgSend

#define ukMsgSend                     func_w_o_param_msgSend

#define uMsgSend(source,parm1,parm2)  system_msgSend(parm1)

MP3_DEC_ENV_STRU mp3_dec_env;



void mp3_scr_init (void)
{
}

void mp3_decode_buf_init (void)
{
}

int xa_mp3_dec_init (void);

void app_dec_mp3_init (void)
{
#ifdef MP3_DECODE_ENABLE
	I32 iResult;

	DBG_Printf ("%s\n\r", __func__);
	//delayms(10);

	//codec_malloc_init ();
	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

	codec_init_fail = FALSE;
	wRequestDecodeFrameExit = FALSE;

	#if (SHARE_LINK_ROLE & SL_ROLE_SLAVE)
	if (app_main_data.share_link_role == SL_ROLE_SLAVE)
	{
		iResult = xa_mp3_dec_init();
		//app_audio_clock_freq_setting(app_media_data.playing_stream_sample_rate);

		return iResult;
	}
	#endif

	//da_pp_channel_setting(MI2S_OUTPUT_CHANNEL_DEFAULT);
	
	#if 1
	entry_id3_flag = ID3_NO_TAG;

	#if 1//def MP3_ID3_ENABLE
	{
		DBG_Printf ("Seek ID3 V2.x\n\r");
		
		if (ID3v2_process(&id3_tag) == ID3_TAG_OK)
		{
			_unicode_to_oem(id3_tag.artist, id3_tag.artist);
			_unicode_to_oem(id3_tag.title, id3_tag.title);
			_unicode_to_oem(id3_tag.album, id3_tag.album);
			
			entry_id3_flag = ID3_TAG_PRESENT;
		}
		else
		{
			DBG_Printf ("Seek ID3 V1.x\n\r");

			if (ID3v1_process(&id3_tag) == ID3_TAG_OK )
			{
				DBG_Printf ("ID3 TAG V1.x Ok\n\r");
				entry_id3_flag = ID3_TAG_PRESENT;
			}
			else
			{
				DBG_Printf ("NO ID3\n\r");
			}
		}
		

		if (wRequestDecodeFrameExit == TRUE)
		{
			//wCodecBusy = FALSE;
			return;
		}
	
		if (entry_id3_flag == ID3_TAG_PRESENT)
		{
			DBG_Printf ("Artist: %s\n\r", id3_tag.artist);
			DBG_Printf ("Title: %s\n\r", id3_tag.title);

			DBG_Printf ("Album: %s\n\r", id3_tag.album);
			#ifdef ID3ALL
			DBG_Printf ("Genre: %s\n\r", id3_tag.genre);
			DBG_Printf ("Year: %s\n\r", id3_tag.year);
			#endif

			//adding id3 tag to disp info
			app_add_song_id3tag (app_media_data.playing_track);
		}
		else
		{
			//DBG_Printf("No ID3\n\r");
		}
	}
	#endif //MP3_ID3_ENABLE

	if (wRequestDecodeFrameExit == TRUE)
	{
		//wCodecBusy = FALSE;
		return;
	}
	
	//check mp3 header and vbr info

	DBG_Printf ("seek MP3 Header\n\r");

	#if 1
	if (mpeg_find_header () == FALSE)
	{
		DBG_Printf("Can not find mp3 header\n\r");

		if (wRequestDecodeFrameExit != TRUE)
		{
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

				//app_lcd_disp_protect();
				//app_nav_sleep(1000);
			#endif
			}
		}

		//wCodecBusy = FALSE;
		return;
	}

	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);
	#endif

	U32 size = (MP3_DECODE_BLOCK_SIZE*4*4);
    mp3_dec_env.rx_postProcessBuf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

	void mp3_decode_init(void);
	mp3_decode_init();
	iResult = xa_mp3_dec_init();

	if (iResult != XA_NO_ERROR)
	{
		DBG_Printf("mp3 dec init error\n\r");
		uMsgSend (UI_CD_INTERFACE, UI_MP3_INIT_FAIL, 0);
	}
	else
	{
		if (wRequestDecodeFrameExit == FALSE)
		{
			//checking the sample rate freq, now only support 32k, 44.1k and 48k)
			if (app_media_data.playing_stream_sample_rate == 44100 ||
				app_media_data.playing_stream_sample_rate == 48000 ||
				app_media_data.playing_stream_sample_rate == 32000)
			{
				#if 1
				app_audio_clock_freq_setting(app_media_data.playing_stream_sample_rate);

				#ifdef APP_OPTEK_D2AS
				app_optek_dsrc_convert_open(app_media_data.playing_stream_sample_rate, AUDIO_OUT_SR_FIXED);
				#endif
				#endif

				#ifdef FOR_ESD_PROTECT
				resume_trk_info.need_resume_play = FALSE;
				#endif
				
				usb_msc_process_id = USB_MSC_MP3_PROCESS;
				//uMsgSend (UI_CD_INTERFACE, UI_MP3_INIT_SUCCESS, 0);
				uMsgSend (UI_CD_INTERFACE, UI_DEC_INIT_SUCCESS, 0);
			}
			else
			{
				usb_msc_process_id = USB_MSC_MP3_PROCESS;
				uMsgSend (UI_CD_INTERFACE, UI_MP3_INIT_FAIL, 0);
			}

			//DBG_Printf("mp3 dec init ok\n\r");
		}
	}

	//wCodecBusy = FALSE;

	//delayms(10);
	//DBG_Puts ("mp3 dec init exit\n\r");
	#endif

#endif //MP3_DECODE_ENABLE
}

void mp3_dec_init( void )
{
	app_media_data.audio_src_bit = AUDIO_SRC_16BIT;
	app_dec_mp3_init();
}

void mp3_dec_fini( int reason )
{
	DBG_Printf("mp3 dec fini:%d\n\r", reason);
	
	usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

	//if (reason == DECODE_END)
	{
		// Reached the end of bit stream.
		//playing to stream end
		uMsgSend (UI_CODEC, UI_DECODE_STREAM_EOF, 0);
	}

    audio_in_stream.status = STREAM_DECOCDE_FINISH;


}

void mp3_dec_decode( void )
{
	int ret;
		
	// Reached the end of bit stream.
	if (decode_exit_req_get() == TRUE)
	{
		//playing to stream end
		uMsgSend (UI_CODEC, UI_DECODE_STREAM_EOF, 0);
	}
}

void mp3_decode_init(void)
{
#ifdef TIME_SCALE_CODE
	tscaleSpeed	=	app_media_data.play_speed;
	
	tscale_init();
	//tscale_setSpeed( en_SPEED_50, 2 );
	tscale_setSpeed( tscaleSpeed, 2 );
	
	//tscale_initFifoChk();
#endif
}

#if 0
void mp3_decode (void)
{
#ifdef FOR_ESD_PROTECT
	resume_trk_info.need_resume_play = FALSE;
#endif

	xa_mp3_decode();

	// Reached the end of bit stream.
	if (wRequestDecodeFrameExit != TRUE)
	{
		//playing to stream end
		uMsgSend (UI_CODEC, UI_DECODE_STREAM_EOF, 0);					
	}
}
#endif


/************************************************************************************
*							MPEG FIND SYNC											*
*************************************************************************************/

#if 1

typedef struct {
    U8 stereo;
    U8 lsf;
    U8 mpeg25;
    U8 lay;
    U8 error_protection;
    U8 bitrate_index;
    U8 sampling_frequency;
    U8 padding;
    U8 extension;
    U8 mode;
    U8 mode_ext;
    U8 copyright;
    U8 original;
    U8 emphasis;
    U16 framesize; /* computed framesize */
	
	U8 *frame_header;
	//int frame_header_bitpos;
	//unsigned int *main_data_header;
	//int main_data_bitpos;

	U8 *NextRead;
	//int NextBitpos;

	//int src_index;

	U8 vbr;
	U8 vbr_toc;
	unsigned long vbr_frames;
	unsigned long vbr_bytes;
	
	//unsigned int pre_mpeg_header_status;
	//unsigned int pre_mpeg_header[2];
	//int match_count;
	//int decoded_data_ready; 
	//int decode_frames;
	U16 bit_rate;

} FRAME;

typedef struct {
	unsigned int syncword				:	11;
	unsigned int IDex					:	1;
	unsigned int ID						:	1;
	unsigned int layer					:	2;
	unsigned int protection_bit			:	1;
	unsigned int bitrate_index			:	4;
	unsigned int sample_frequencsy		:	2;
	unsigned int padding_bit			:	1;
	unsigned int private_bit			:	1;
	unsigned int mode					:	2;
	unsigned int mode_extension			:	2;
	unsigned int copyright				:	1;
	unsigned int original				:	1;
	unsigned int emphasis				:	2;
} __attribute__ ((packed)) MPEG_HEADER;


const U16 Bit_Rate [2][15] = {
	{0,4000,5000,6000,7000,8000,10000,12000,14000,16000, 20000,24000,28000,32000,40000},
	{0,1000,2000,3000,4000,5000,6000,7000,8000,10000,12000,14000,16000,18000,20000}
};

const U16 freqs[9] = {
	44100, 48000, 32000, 22050, 24000, 16000, 11025, 12000, 8000
};

const U16 tabsel_123[2][3][16] = {
   { {0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,},
     {0,32,48,56, 64, 80, 96,112,128,160,192,224,256,320,384,},
     {0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320,} },

   { {0,32,48,56,64,80,96,112,128,144,160,176,192,224,256,},
     {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,},
     {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,} }
};

#define         MPG_MD_STEREO           0
#define         MPG_MD_JOINT_STEREO     1
#define         MPG_MD_DUAL_CHANNEL     2
#define         MPG_MD_MONO             3

#define			MAXFRAMESIZE 1792

#define FRAMES_FLAG     0x0001
#define BYTES_FLAG      0x0002
#define TOC_FLAG        0x0004
#define VBR_SCALE_FLAG  0x0008

#define NUMTOCENTRIES	100

enum {
	MPEG1_STREAM = 0,
	MPEG2_STREAM = 1
};

#define MASK_IDex					0x10				//:	1;
#define MASK_ID						0x08				//:	1;
#define MASK_layer					0x06				//:	2;
#define MASK_protection_bit			0x01				//:	1;
	
#define MASK_bitrate_index			0xF0				//:	4;
#define MASK_sample_frequencsy		0x0C				//:	2;
#define MASK_padding_bit				0x02				////:	1;
#define MASK_private_bit				0x01				//:	1;

#define MASK_mode					0xC0				//:	2;
#define MASK_mode_extension			0x30				//:	2;
#define MASK_copyright				0x08				//:	1;
#define MASK_original				0x04				//:	1;
#define MASK_emphasis				0x03				//:	2;



int decode_header(U8 *header, FRAME *mpeg_frame)
{
	long framesize;
	U8 tmp;

//	mpeg_frame->single = -1;

//	if (header->IDex) {
	if ( (header[1] & MASK_IDex) != 0x0)
	{
		mpeg_frame->mpeg25 = FALSE;

	//	if (header->ID) {
		if ( (header[1] & MASK_ID) != 0x0) {
			mpeg_frame->lsf = MPEG1_STREAM;				/*MPEG 1*/
		}
		else {
			mpeg_frame->lsf = MPEG2_STREAM;				/*MPEG 2*/
		}
	}
	else
	{
		/*MPEG 2.5*/
		//if (header->ID)
		if ( (header[1] & MASK_ID) != 0x0)
			return FALSE;					/*MPEG2.5 MUST BE MPEG 2.0*/

			
		mpeg_frame->lsf = MPEG2_STREAM;
		mpeg_frame->mpeg25 = TRUE;
	}
	
	tmp = (header[1] & MASK_layer) >> 1;
	
	if (tmp == 0) {

		return FALSE;
	}

	mpeg_frame->lay = 4 - tmp;

	tmp = header[1] & MASK_protection_bit;
	mpeg_frame->error_protection = tmp ^ 1;

	tmp = (header[2] & MASK_bitrate_index) >> 4;
	mpeg_frame->bitrate_index = tmp;

	tmp = (header[2] & MASK_sample_frequencsy) >> 2;

	if (tmp == 3) {
		return FALSE;
	}

	//if ((U8) header->sample_frequencsy == 3) {
	//	return FALSE;
	//}

	if (mpeg_frame->mpeg25) {
		mpeg_frame->sampling_frequency = 6 + tmp;
	}
	else {
		mpeg_frame->sampling_frequency = tmp + 3 * mpeg_frame->lsf;
	}

//	mpeg_frame->error_protection = (U8) header->protection_bit ^ 1;
//	mpeg_frame->bitrate_index = (U8) header->bitrate_index;

	tmp = (header[2] & MASK_padding_bit) >> 1;
	mpeg_frame->padding = tmp;
	tmp = (header[2] & MASK_private_bit);
	mpeg_frame->extension = tmp;

	tmp = (header[3] & MASK_mode) >> 6;
	mpeg_frame->mode = tmp;
	tmp = (header[3] & MASK_mode_extension) >> 4;
	mpeg_frame->mode_ext = tmp;
	tmp = (header[3] & MASK_copyright) >> 3;
	mpeg_frame->copyright = tmp;
	tmp = (header[3] & MASK_original) >> 2;
	mpeg_frame->original = tmp;
	tmp = (header[3] & MASK_emphasis);
	mpeg_frame->emphasis = tmp;

	mpeg_frame->stereo = ((U8) mpeg_frame->mode == MPG_MD_MONO) ? 1 : 2;

	if ( mpeg_frame->bitrate_index == 0 ||  mpeg_frame->bitrate_index == 15) {
		/*we don't support free running format*/
		//DBG_Printf ("Can not play free running stream\n\r");
		return FALSE;
	}

	/*caculate frame size*/
	if ((U8) mpeg_frame->lsf == 0)
	{
		framesize  = (long) ((long) tabsel_123[mpeg_frame->lsf][mpeg_frame->lay-1][mpeg_frame->bitrate_index]) * 144000l;
		framesize /= (long) (freqs[mpeg_frame->sampling_frequency]);
		framesize = framesize + (long) mpeg_frame->padding - 4l;
		mpeg_frame->framesize = (U16) framesize;			
	}
	else
	{
		framesize  = (long) ((long) tabsel_123[mpeg_frame->lsf][mpeg_frame->lay-1][mpeg_frame->bitrate_index]) * 72000l;
		framesize /= (long) (freqs[mpeg_frame->sampling_frequency]);
		framesize = framesize + (long) mpeg_frame->padding - 4l;
		mpeg_frame->framesize = (U16) framesize;			
	}


	return TRUE;
}

int mpeg_find_header (void)
{
	U8 tmp1, tmp2, num_readbuf = 0;
	long len;
	U8 mpeg_header [4], xing_buf[4];
	STREAM_ID s;

	FRAME *mpeg_frame, *second_mpeg_frame;
	U32 pos,tmp_pos;
	U32 times,stream_len;
	MYFILE *pFile;
	U32 rlen;
	int ret;

	DBG_Printf ("%s\n\r", __func__);

	//pFile = app_media_data.pMediaRead; //just pass one parm, no meaning
	pFile = pMediaRead; //just pass one parm, no meaning

	//codec_malloc_init ();

	#ifdef SLOW_MEMORY_USEDpc
	mpeg_frame = (FRAME *)codec_malloc( sizeof (FRAME) );
	#else
	mpeg_frame = (FRAME *)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, sizeof (FRAME), CODEC_MEM_ALIGN_4);
	#endif

	DBG_Assert (mpeg_frame != NULL);

	
	#ifdef SLOW_MEMORY_USEDpc
	second_mpeg_frame = (FRAME *)codec_malloc( sizeof (FRAME) );
	#else
	second_mpeg_frame = (FRAME *)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, sizeof (FRAME), CODEC_MEM_ALIGN_4);
	#endif

	DBG_Assert (second_mpeg_frame != NULL);
	
	app_media_data.playing_stream_total_time = 0;
	app_media_data.playing_stream_vbr = 0;

	times = 0;

	//debug
	//xa_fseek (pFile, 0, SEEK_SET);

	pos = xa_ftell (pFile);
	rlen = xa_fread (&tmp1, 1, 1, pFile);
	if (rlen == 0)
	{
		return FALSE;
	}

	while (1)
	{
		while (1)
		{
			if (wRequestDecodeFrameExit == TRUE)
			{
				return FALSE;
			}

			times++;
			if (times > 0x2000)
			{
				times = 0;
				app_nav_sleep (100);
			}

			if (tmp1 == 0xFF)
			{
				rlen = xa_fread (&tmp2, 1, 1, pFile);
				if (rlen == 0)
				{
					return FALSE;
				}
			
				if ( (tmp2 & 0xE0) == 0xE0 )
				{
					//can not be layer 4

					/*Header is found*/
					mpeg_header[0] = tmp1;
					mpeg_header[1] = tmp2;

					rlen = xa_fread (&mpeg_header[2], 1, 2, pFile);
					if (rlen == 0)
					{
						return FALSE;
					}
						
					/* decode header */
					if (decode_header(mpeg_header, mpeg_frame) == TRUE)
					{
						break;							
					}
					else
					{
						pos += 1;
						//ret = xa_fseek (pFile, pos, SEEK_SET);
						ret = xa_fseek (pFile, (pos+1), SEEK_SET);
						if (ret == -1)
						{
							return FALSE;
						}

						tmp1 = tmp2;
					}
				}
				else
				{
					pos = xa_ftell (pFile);
					rlen = xa_fread (&tmp1, 1, 1, pFile);
					if (rlen == 0)
					{
						return FALSE;
					}
				}
			}
			else
			{
				pos = xa_ftell (pFile);
				rlen = xa_fread (&tmp1, 1, 1, pFile);
				if (rlen == 0)
				{
					return FALSE;
				}
			}
		}

		app_media_data.playing_stream_media_offset = pos;

		if ( mpeg_frame->lsf == 0 )
		{
			/*MPEG 1*/
			if(mpeg_frame->stereo == 1)
			{
				/*siggle channel*/
				//mpeg_streambuf_rpoint (17+4);
				ret = xa_fseek (pFile, (pos+17+4), SEEK_SET);
				if (ret == -1)
				{
					return FALSE;
				}
			}
			else
			{
				/*two channels*/
				//mpeg_streambuf_rpoint (32+4);
				ret = xa_fseek (pFile, (pos+32+4), SEEK_SET);
				if (ret == -1)
				{
					return FALSE;
				}
			}
		}
		else
		{
			/*MPEG 2*/
			if(mpeg_frame->stereo == 1)
			{
				/*single channel*/
				//mpeg_streambuf_rpoint (9+4);
				ret = xa_fseek (pFile, (pos+9+4), SEEK_SET);
				if (ret == -1)
				{
					return FALSE;
				}
			}
			else
			{
				/*two channels*/
				//mpeg_streambuf_rpoint (17+4);
				ret = xa_fseek (pFile, (pos+17+4), SEEK_SET);
				if (ret == -1)
				{
					return FALSE;
				}
			}

		}

		rlen = xa_fread (xing_buf, 1, 4, pFile);
		if (rlen == 0)
		{
			return FALSE;
		}

		if (  (xing_buf[0] != 0x58 ) ||
				(xing_buf[1] != 0x69 )  ||		//Xing
				(xing_buf[2] != 0x6E )  ||
				(xing_buf[3] != 0x67) )
		{			
			//not vbr stream
			//xa_fseek (pFile, pos, SEEK_SET);
			//return;
		}
		else
		{
			//vbr stream
			unsigned int head_flags;
			unsigned int i;

			mpeg_frame->vbr = TRUE;

			head_flags = 0;

			rlen = xa_fread (&tmp1, 1, 1, pFile);
			if (rlen == 0)
			{
				return FALSE;
			}
			head_flags |= tmp1;
			head_flags <<= 8;

			rlen = xa_fread (&tmp1, 1, 1, pFile);
			if (rlen == 0)
			{
				return FALSE;
			}

			head_flags |= tmp1;
			head_flags <<= 8;
				
			rlen = xa_fread (&tmp1,1,1,pFile);
			if (rlen == 0)
			{
				return FALSE;
			}
			head_flags |= tmp1;
			head_flags <<= 8;

			rlen = xa_fread (&tmp1,1,1,pFile);
			if (rlen == 0)
			{
				return FALSE;
			}

			head_flags |= tmp1;

			if ( head_flags & FRAMES_FLAG )
			{
				//mpeg_frame->vbr_frames   = ((unsigned long) read_bits (16)) << 16;
				//mpeg_frame->vbr_frames |= (unsigned long) read_bits (16);
				
				//xa_fread (&(mpeg_frame->vbr_frames),1,4,pFile);
				//mpeg_frame->vbr_frames = dw_Little_Big_Endian (&(mpeg_frame->vbr_frames));

				mpeg_frame->vbr_frames = 0;

				rlen = xa_fread (&tmp1, 1, 1, pFile);
				if (rlen == 0) {
					return FALSE;
				}

				mpeg_frame->vbr_frames |= tmp1;
				mpeg_frame->vbr_frames <<= 8;

				rlen = xa_fread (&tmp1, 1, 1, pFile);
				if (rlen == 0)
				{
					return FALSE;
				}

				mpeg_frame->vbr_frames |= tmp1;
				mpeg_frame->vbr_frames <<= 8;

				rlen = xa_fread (&tmp1, 1, 1, pFile);
				if (rlen == 0)
				{
					return FALSE;
				}

				mpeg_frame->vbr_frames |= tmp1;
				mpeg_frame->vbr_frames <<= 8;

				rlen = xa_fread (&tmp1, 1, 1, pFile);
				if (rlen == 0)
				{
					return FALSE;
				}

				mpeg_frame->vbr_frames |= tmp1;

				if( mpeg_frame->lsf == MPEG1_STREAM ) {
					mpeg_frame->vbr_frames <<= 1;
				}

				app_media_data.playing_stream_vbr = 1;

				app_media_data.playing_stream_total_time =
				(mpeg_frame->vbr_frames * 576l) / freqs[mpeg_frame->sampling_frequency];

				stream_len = AUDIO_FIFO_STREAM_STREAM_LEN() - pos;
				//app_media_data.playing_stream_media_offset = pos;

			#if 0
				//average bitrate
				app_media_data.playing_stream_bitrate = (U16) (stream_len / app_media_data.playing_stream_total_time);
			#else
				if (app_media_data.playing_stream_total_time != 0)
				{
					//average bitrate
					app_media_data.playing_stream_bitrate = (U16) (stream_len / app_media_data.playing_stream_total_time);
				}
				else
				{
					DBG_Assert(FALSE);
				}
			#endif

				DBG_Printf ("VBR total playing time %d\n\r", app_media_data.playing_stream_total_time);
				DBG_Printf ("VBR average bitrate %d\n\r", app_media_data.playing_stream_bitrate);
			}

			ret = xa_fseek (pFile, pos, SEEK_SET);
			if (ret == -1)
			{
				return FALSE;
			}
		}

		//check the second header
		ret = xa_fseek (pFile, (pos + mpeg_frame->framesize + 4) , SEEK_SET);
		if (ret == -1)
		{
			return FALSE;
		}

		tmp_pos = xa_ftell (pFile);
		rlen = xa_fread (&tmp1,1,1,pFile);
		if (rlen == 0)
		{
			return FALSE;
		}

		if (tmp1 == 0xFF)
		{
			rlen = xa_fread (&tmp2, 1, 1, pFile);
			if (rlen == 0)
			{
				return FALSE;
			}
			if ( (tmp2 & 0xE0) == 0xE0) 
			{
				rlen = xa_fread (&mpeg_header[2], 1, 2, pFile);
				if (rlen == 0) {
					return FALSE;
				}

				/* decode header */
				if (decode_header(mpeg_header, second_mpeg_frame) == TRUE) {

					if ( (mpeg_frame->sampling_frequency == second_mpeg_frame->sampling_frequency) &&
						(mpeg_frame->mpeg25 == second_mpeg_frame->mpeg25) &&
						(mpeg_frame->lsf == second_mpeg_frame->lsf) )
					{
						
						//header is find, jump the the second header of mp3, missing the first header?
						ret = xa_fseek (pFile, (pos + mpeg_frame->framesize + 4) , SEEK_SET);
						if (ret == -1) {
							return FALSE;
						}

						DBG_Printf ("Find mp3 header\n\r");

						return TRUE;
					}
				}
			}
		}
		

		{
			DBG_Printf ("The first head is err\n\r");
					
			//Search from last pos

			tmp_pos += 1;
			//ret = xa_fseek (pFile, pos, SEEK_SET);
			pos = app_media_data.playing_stream_media_offset + 1;
			ret = xa_fseek (pFile, pos, SEEK_SET);
			if (ret == -1)
			{
				return FALSE;
			}

			rlen = xa_fread (&tmp1, 1, 1, pFile);
			if (rlen == 0)
			{
				return FALSE;
			}

			//tmp1 = tmp2;
		}
	}
}

#endif



#ifdef TIME_SCALE_CODE

U16 srcTmpBuffer[MP3_DEC_FRAME_SIZE*2] __attribute__((section(".tscale_bss")));

int mp3GetPcmData(void )
{
	int ret;
	U32 frame_len;
	U32 *decoder_buf;
				
	ret = xa_mp3_dec_frame(&decoder_buf, &frame_len);
	if (!ret)
	{
		return 1;
	}
	
	if (frame_len > 0)
	{
		ret =  Decode_OutputBuffer(decoder_buf, frame_len);
		if (!ret)
		{
			return 1;
		}
	}
	
	if (wRequestDecodeFrameExit == TRUE)
	{
		return TRUE;
	}

#if 0
	wSrcSlice = getSrcSliceSize( out_dwords, in_dwords, &wSrcSliceSize );
	for (i=0; i<wSrcSlice; i++)
	{
		tscale_input( srcTmpBuffer, *pframe_len );
	}
#endif

	return 0;
}

#endif //TIME_SCALE_CODE


int mp3_dec_info_get(DECODE_INFO_STRU *info)
{
}


#if 0
static const DECODE_DEVICE_STRU aux_decode = 
{
    DECODE_PCM,
	"aux decode",	
	pcm_dec_init,
	pcm_dec_fini,
	pcm_dec_decode_frame,
    pcm_dec_info_get
};
#endif


int xa_mp3_dec_frame (U32 **out_buf, U32 *out_len);

int	mp3_dec_frame(U32 **out_buf, U32 *out_len)
{
	int ret;

	ret = xa_mp3_dec_frame(out_buf, out_len);

	app_media_data.playing_stream_pcm_sample += *out_len/4;
	return ret;
}

void mp3_dec_init( void );
void mp3_dec_fini( int reason );
void mp3_dec_decode( void );

static const DECODE_DEVICE_STRU mp3_decode =
{
	DECODE_MP3,
	"mp3 decode",
	mp3_dec_init,
	mp3_dec_fini,
	mp3_dec_frame,
	mp3_dec_info_get
};

void mp3_decocde_set(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_set_msgSend((DECODE_DEVICE_STRU *)&mp3_decode);
}

void mp3_decocde_init(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_init_msgSend();
}

void mp3_decocde_start(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_start_msgSend();
}

void mp3_decocde_start_w_init(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_start_w_init_msgSend();
}

#endif //MP3_DECODE_ENABLE
