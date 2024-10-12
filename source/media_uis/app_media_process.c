#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "regmap.h"

//#include "audio_pll.h"
//#include "hw_da_pp.h"
#include "hw_pll.h"

#include "mem_reloc.h"

//#include "message.h"
#include "app_timer.h"
#include "app_main.h"
#include "app_sdram.h"
#include "audio_com.h"
#include "audio_codec.h"
#include "app_media.h"
//#include "app_cmd.h"
#include "key_scan.h"
#include "Id3V2.h"
#include "codec.h"
//#include "app_dac.h"
//#include "app_lcd.h"
//#include "audio_dec.h"

#include "app_string_OEM.h"
#include "app_media_com.h"
#include "app_media_process.h"
//#include "app_gpio_oem.h"
//#include "app_optek_d2as.h"

#include "system_task.h"
#include "cpu_soft_reset.h"

#ifdef USB_HOST_ENABLE
#define app_timer_window_time_set     app_window_time_set

#define ukParmSend                    func_w_one_param_msgSend

#define ukMsgSend                     func_wo_param_msgSend

#define app_timer_window_time_set     app_window_time_set

#define uMsgSend(source,parm1,parm2)  system_msgSend(parm1)

#define uDecSend_Init(x,y)            mp3_decocde_init()

#define uDecSend_Frame(x,y)           mp3_decocde_start()

#define decode_type                   decode_type_get()

#define app_timer_get_idle_time()     0

#define app_timer_idle_time_reset()

#define app_lcd_disp_mp3_playback_time()

//#define app_media_data.window          app_window_current_get()


U8 usb_msc_process_id;

//U32 pre_ticks = 0;
//U32 pre1_ticks = 0;

#if 1//def MP3CD

U16 guiBusy = 0;
U32 pre_ticks = 0;
//U32 pre1_ticks = 0;
//U8 randomAllFinish = 0;

extern U8 decode_block_process_timer;

extern U16 gwEntryNum;
extern U16 gwFolderNum;


#if 0
enum {
	NO_SKIP_REQUEST,
	SKIP_FORWARD,
	SKIP_BACKWARD,
	FAST_FORWARD,
	FAST_BACKWARD
};
#endif

//ID3_TAG id3_tag;

extern U16 song_information[];


#define MIN_STREAM_SIZE		(4096/2)

#if 0
void ukParmSend (FUNCTION key_func, U32 parm1)
{
#if 0
	U32 data[WIDTH_OF_KFUNC_QUEUE];

	data[0] = (U32)key_func;
	data[1] = enKEY_FUNC_1PARAM;
	data[2] = (U32)parm1;

	QUEUE_KEYFUNC_SEND;
#endif
}
#endif


/****************************************************************************************
 *						Song Infomation on LCD											*
 ****************************************************************************************/

BOOL isEnoughData (long margin)
{	
	long len,wlen,pos;

	len = AUDIO_FIFO_STREAM_STREAM_LEN();
	wlen = AUDIO_FIFO_STREAM_FIFO_WLEN();
	pos = AUDIO_FIFO_STREAM_FIFO_RLEN();

	if ( (len == wlen) || ((wlen - pos) >= margin) ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void app_copy_song_pathname (U16 index)
{
#if 0
	int j = 0;

	song_information[j++] = 'c';
	song_information[j++] = 'h';
	song_information[j++] = 'e';
	song_information[j++] = 'c';
	song_information[j++] = 'k';
	song_information[j++] = '.';
	song_information[j++] = '.';
	song_information[j++] = '.';
	song_information[j++] = 0x0;
	song_information[j++] = 0x0;
#endif
}

void app_add_song_id3tag (U16 index)
{
#if 0//ndef STORE_ALL_ID3
	if (app_media_data.disp_info == DISP_INFO_ARTIST) {
		if (entry_id3_flag == ID3_TAG_PRESENT) {
			strcpy ((char *) song_information, (const char *) entry_id3_tag.artist);		
		}
		else {
			song_information[0] = 0x0;
		}
		
		strAddUnicodeNull ( (U8 *) song_information); 
	}
	else if (app_media_data.disp_info == DISP_INFO_TITLE) {
		
		if (entry_id3_flag == ID3_TAG_PRESENT) {
			strcpy ((char *) song_information, (const char *) entry_id3_tag.title);		
		}
		else {
			song_information[0] = 0x0;
		}
		
		strAddUnicodeNull ( (U8 *) song_information); 
	}
#endif //STORE_ALL_ID3
}

void app_nav_mp3_process (system_msg_stru *process_msg)
{
#if 1
	U16 entry,index;
	long len,pos;
	long start_addr;
	I16 i;
	ID3_RETURN id3_result;
	U8 skip_mode = NO_SKIP_REQUEST;
	U16 tmp;

	//DBG_Assert (app_media_data.media == MEDIA_USB_DEVICE);

	switch (process_msg->parm)
	{	
	case UI_STREAM_UNDERFLOW:
		if (app_media_data.fast_processing != TRUE) {
			app_dac_mute_enable ();
			app_media_data.playing_stream_status = STREAM_WAITING_DATA;
		}
		break;

	case UI_STREAM_EOF:
		#ifdef SD_ENABLE
		if ( (global_vari.audio_source == SOURCE_USB_MSC) || (global_vari.audio_source == SOURCE_USB_SD) )
		#else
		if (global_vari.audio_source == SOURCE_USB_MSC)
		#endif
		{
			//can not mute,for process has changed,
			#if 0
			//for usb device, no need to pre read data
			app_media_mute_enable ();
			break;
			#endif
		}
		break;
	
	case UI_PCM_OUT_UNDERFLOW:
		DBG_Printf ("UI_PCM_OUT_UNDERFLOW\n\r");
		{
			BOOL stream_end = FALSE;
			
			//how to judge wma decoder end
			if ( app_media_data.playing_stream_type == STREAM_WMA )
			{
				if ( (AUDIO_FIFO_STREAM_FIFO_RLEN()  == AUDIO_FIFO_STREAM_STREAM_LEN() ) ||
					(wNoMoreFrames == TRUE) )
				{
					stream_end = TRUE;			
				}
			}
			else
			{
				if (AUDIO_FIFO_STREAM_FIFO_RLEN()  == AUDIO_FIFO_STREAM_STREAM_LEN() )
				{
					stream_end = TRUE;
				}
			}
			
			if (stream_end == TRUE)
			{
				app_dac_mute_enable ();
				uMsgSend (UI_CD_INTERFACE,UI_DECODE_STREAM_EOF,0);
			}
			else
			{
				if (app_media_data.fast_processing)
				{
					app_dac_mute_enable ();
					app_media_data.playing_stream_status = STREAM_WAITING_DATA;
				}
				else
				{
					app_dac_mute_enable ();
					app_media_data.playing_stream_status = STREAM_WAITING_DATA;
				}
			}
		}
		break;

	case UI_DECODE_SKIP_B:
		skip_mode = SKIP_BACKWARD;
		//ukMsgSend(app_nav_usb_mp3_skipb_1);
		app_nav_usb_mp3_skipb_1();
		skip_key_flag = enMP3_PLAY_FLAG;
		break;

	case UI_DECODE_SKIP_F:
		DBG_Printf ("UI_DECODE_SKIP_F\n\r");
		skip_mode = SKIP_FORWARD;
		//no break, continue to next code

	case UI_DECODE_STREAM_EOF:
		DBG_Printf ("UI_DECODE_STREAM_EOF\n\r");
		if ( (app_media_data.fast_processing) && (skip_mode != SKIP_FORWARD) )
		{
			DBG_Assert(FALSE);
			break;
		}
		
		app_dac_mute_enable ();
		uiDecDecodeFrameExit ();

		//write enc stream to usb device may need a lot of time
		decode_block_process_timer = TRUE;
		app_media_mute_enable();
		STREAM_MUTEX_LOCK;
		STREAM_MUTEX_UNLOCK;
		decode_block_process_timer = FALSE;

		entry_id3_flag = ID3_UNKNOWN;
		
		if( (app_media_data.playback_mode == REPEAT_ONE)/* && (skip_mode != SKIP_FORWARD)*/)
		{
			DBG_Printf ("Repeat One\n\r");

			AUDIO_FIFO_STREAM_FLUSH();

			//app_nav_mp3_play_track (app_media_data.playing_index);
			ukParmSend(app_nav_mp3_play_track,app_media_data.playing_index);
		}
		else if(app_media_data.playback_mode == REPEAT_DIR)
		{
			U16 entry = app_file_get_entry_in_folder(app_media_data.playing_index);
			U16 folder = app_file_get_folder_of_entry(app_media_data.playing_index);
			U16 total = app_file_get_folder_entries(folder);			
			DBG_Printf ("Repeat Dir\n\r");

			if (++entry < total)
			{
				//app_media_data.playing_index = entry;
				app_media_data.playing_index = app_file_get_first_entrie_of_folder(folder) + entry;
			}
			else
			{
				app_media_data.playing_index = app_file_get_first_entrie_of_folder(folder);
			}
			
			AUDIO_FIFO_STREAM_FLUSH();

			//app_nav_mp3_play_track (app_media_data.playing_index);
			ukParmSend(app_nav_mp3_play_track,app_media_data.playing_index);
		}
		else if ( (app_media_data.playback_mode == RANDOM_ALL) || (app_media_data.playback_mode == RANDOM_REPEAT_ALL) ) 
		{
			AUDIO_FIFO_STREAM_FLUSH();
		
			app_media_data.random_played_tracks++;
		
			if (app_media_data.random_played_tracks >= playing_info.total_of_playlist)
			{
				if (app_media_data.playback_mode == RANDOM_ALL)
				{
					app_media_data.playing_track = 0;			//resume start from beginning

					#ifdef MP3_MODULE
					cEV_Display_Info_Changed = TRUE;
					cEV_Display_Info_Changed_Info = 0x07;
					#endif
					
					app_media_data.playing_stream_status = STREAM_IDLE;
					//app_media_data.ui_background = app_nav_dummy_process;
					usb_msc_process_id = USB_MSC_DUMMY_PROCESS;
					app_media_data.PlayDiscNum = 3;
					ukMsgSend (app_nav_mp3_playback_over);
				}
				else {
					//random repeat all
					app_media_data.random_played_tracks = 0;
					random_set_base (random ());
					//app_nav_mp3_play_track ( random_track(playing_info.total_of_playlist) );
					ukParmSend(app_nav_mp3_play_track,random_track(playing_info.total_of_playlist));
				}
			}
			else
			{
				DBG_Printf ("Ramdom play track\n\r");
				//app_nav_mp3_play_track (random_track(playing_info.total_of_playlist));			
				ukParmSend(app_nav_mp3_play_track,random_track(playing_info.total_of_playlist));
			}
		}
		else if ( (app_media_data.playback_mode == RANDOM_IN_DIR) || (app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) )
		{
			U16 start_index, index, tracks, dir;

			AUDIO_FIFO_STREAM_FLUSH();

			dir = app_list_playlist_dir (app_media_data.playing_index);
			tracks = app_list_playlist_tracks_in_dir (dir);
			start_index = app_list_playlist_dir_start_index (dir);

			DBG_Printf ("dir = %d\n\r", dir);
			DBG_Printf ("tracks = %d\n\r", tracks);
			DBG_Printf ("start_index = %d\n\r", start_index);
			DBG_Printf ("random_tracks = %d\n\r", app_media_data.random_played_tracks);

			app_media_data.random_played_tracks++;
			if (app_media_data.random_played_tracks >= tracks)
			{
				if (app_media_data.playback_mode == RANDOM_IN_DIR)
				{
					app_media_data.playing_track = 0;			//resume start from beginning

					#ifdef MP3_MODULE
					cEV_Display_Info_Changed = TRUE;
					cEV_Display_Info_Changed_Info = 0x07;
					#endif
					
					app_media_data.playing_stream_status = STREAM_IDLE;
					//app_media_data.ui_background = app_nav_dummy_process;
					usb_msc_process_id = USB_MSC_DUMMY_PROCESS;
					ukMsgSend (app_nav_mp3_playback_over);
				}
				else
				{
					//random in dir repeat all
					app_media_data.random_played_tracks = 0;
					random_set_base (random());
				}
			}

			index = random_track (tracks);
			DBG_Printf ("index = %d\n\r", index);

			//app_nav_mp3_play_track (start_index + index);			
			ukParmSend(app_nav_mp3_play_track,start_index + index);
		}
		else if ( /*(app_media_data.playback_mode == REPEAT_DIR) ||*/ (app_media_data.playback_mode == DIR_RANDOM) ||
			(app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) ) 
		{
		#ifdef MULTI_FOLDER_LEVEL
			i = app_media_data.playing_index;
			
			i++;
			if (i >= playing_info.total_of_playlist)
			{
				i = 0;
			}

			if ( app_list_playlist_dir (app_media_data.playing_index) !=  app_list_playlist_dir (i) )
			{
				//next track folder is different with current track folder

				app_sdram_stream_flush ();
				
				if (app_media_data.playback_mode == REPEAT_DIR)
				{
					i = app_list_playlist_dir (app_media_data.playing_index);
					i = app_list_playlist_dir_start_index (i);
					//app_nav_mp3_play_track (i);			
					ukParmSend(app_nav_mp3_play_track,i);
				}
				else
				{
					//dir random
					U16 playlist_dirs;
					
					playlist_dirs = app_list_playlist_total_dirs ();

					app_media_data.random_played_tracks++;
					if (app_media_data.random_played_tracks >= playlist_dirs)
					{
						if (app_media_data.playback_mode == DIR_RANDOM)
						{
							app_media_data.playing_track = 0;			//resume start from beginning

							#ifdef MP3_MODULE
							cEV_Display_Info_Changed = TRUE;
							cEV_Display_Info_Changed_Info = 0x07;
							#endif

							app_media_data.playing_stream_status = STREAM_IDLE;
							//app_media_data.ui_background = app_nav_dummy_process;
							usb_msc_process_id = USB_MSC_DUMMY_PROCESS;
							ukMsgSend (app_nav_mp3_playback_over);
						}
						else
						{
							//rnadom dir repeat all
							app_media_data.playing_track = 0;			//resume start from beginning

							#ifdef MP3_MODULE
							cEV_Display_Info_Changed = TRUE;
							cEV_Display_Info_Changed_Info = 0x07;
							#endif

							i = random_track (playlist_dirs);

							i = app_list_playlist_dir_start_index (i);
							//app_nav_mp3_play_track (i);			
							ukParmSend(app_nav_mp3_play_track,i);
						}
					}
					else
					{
						i = random_track (playlist_dirs);

						i = app_list_playlist_dir_start_index (i);
						//app_nav_mp3_play_track (i);
						ukParmSend(app_nav_mp3_play_track, i);
					}					
				}
			}
			else
			{
				//same folder
				goto Process_Next_Stream;
			}
		#endif
		}
		else
		{
Process_Next_Stream:
	
			#if 0
			s = app_sdram_stream_get_rstream ();
		
			if ( (s != INVALID_STREAM_ID) ) {
				
				app_sdram_stream_close_rstream (s);
								
				if ( (app_sdram_stream_get_wstream ()) == s) {

					app_sdram_stream_close_wstream (s);
					app_sdram_stream_flush ();
				}
				else {
					app_sdram_stream_del_stream (s);
				}
			}
			#else
			//AUDIO_FIFO_STREAM_FLUSH();
			#endif
		
			i = app_media_data.playing_index;
			app_media_data.playing_index++;
			
			if (app_media_data.playing_index >= playing_info.total_of_playlist)
			{
				#ifdef PLAYBACK_CIRCLES_IN_NORMAL_MODE
				if ( (app_media_data.playback_mode == REPEAT_ALL) || (skip_mode == SKIP_FORWARD) )
				#else
				if ( (app_media_data.playback_mode == REPEAT_ALL) )
				#endif
				{
					app_media_data.playing_index = 0;
				}
			}
		
			if (app_media_data.playing_index >= playing_info.total_of_playlist)
			{
				#ifdef MP3_MODULE
				cEV_Display_Info_Changed = TRUE;
				cEV_Display_Info_Changed_Info = 0x07;
				#endif

				app_media_data.playing_track = 0;			//resume start from beginning
				app_media_data.playing_stream_status = STREAM_IDLE;
				//app_media_data.ui_background = app_nav_dummy_process;
				usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

				ukMsgSend (app_nav_mp3_playback_over);
				//app_nav_mp3_playback_over();
			}
			else
			{
				app_media_data.playing_track = app_list_get_list_content(app_media_data.playing_index);

				#ifdef MP3_MODULE
				cEV_Display_Info_Changed = TRUE;
				cEV_Display_Info_Changed_Info = 0x07;
				#endif

				if (1)
				{
					AUDIO_FIFO_STREAM_FLUSH();
					app_media_data.playing_stream_status = STREAM_IDLE;
					//app_media_data.ui_background = app_nav_dummy_process;
					usb_msc_process_id = USB_MSC_DUMMY_PROCESS;
					ukParmSend(app_nav_mp3_play_track, app_media_data.playing_index);
					//app_nav_mp3_play_track (app_media_data.playing_index);			
				}
			}
		}
		break;

	case UI_CD_LEAD_OUT:
		break;


	case UI_WMA_INIT_SUCCESS:
		//ukParmSend(app_audio_clock_freq_setting, app_media_data.playing_stream_sample_rate);
		
		if (app_media_data.resume_jump_request == FALSE)
		{
			uDecSend_Frame(DECODE_Frame, 0); 	/*start to decode*/
			app_dac_mute_disable ();
			app_media_data.playing_stream_status = STREAM_MEDIA;

			if ( (app_window_current_get() == (WINDOW *) &nav_mp3_window ) ||
				(app_window_current_get() == (WINDOW *) &mp3_mode_window) )
			{
				app_lcd_disp_mp3_playback_time();
			}
		}
		break;

	case UI_WMA_INIT_FAIL:
		break;

	case UI_WMA_INIT_DRM:
		//app_nav_window_set(&nav_mp3_prot_window, app_window_data.window_index+1);
		app_nav_window_set(&nav_mp3_prot_window, 1);
		break;

	case UI_MP3_INIT_SUCCESS:
		#if 0
		DBG_Printf("mp3 init success\n\r");
		#else
		DBG_Printf("dec init success\n\r");
		#endif

		//ukParmSend(app_audio_clock_freq_setting, app_media_data.playing_stream_sample_rate);

		#ifndef RESUME_PLAYBACK_ENABLE
		//find mp3 header may cut some off data, need to reloead again
		app_media_data.resume_jump_request = FALSE;
		app_media_data.playing_time = 0;
		#endif

		#ifdef RESUME_PLAYBACK_ENABLE
		//if ( (app_media_data.resume_jump_request == TRUE) && (app_media_data.resume_vbr == 0) )
		if ( (app_media_data.resume_jump_request == TRUE) && (app_media_data.playing_stream_vbr == 0) )
		{
			//resueme play
			pos = app_media_data.playing_time * app_media_data.playing_stream_bitrate + app_media_data.playing_stream_media_offset;
			app_media_data.playing_stream_pcm_sample = app_media_data.playing_stream_sample_rate * app_media_data.playing_time;
			ukParmSend(app_cmd_mp3_jump, pos);
		}
		else
		#else
		if (app_media_data.resume_jump_request == FALSE)
		#endif
		{
			#ifdef RESUME_PLAYBACK_ENABLE
			app_media_data.playing_stream_pcm_sample = 0; //for vbr resume
			#endif

			DBG_Puts ("codec dec start\n\r");
			
			uDecSend_Frame(DECODE_Frame, 0); 	//start to decode

			if (app_media_data.playback_state == PLAYING_MODE_PLAY)
			{
				app_dac_mute_disable ();
			}
			app_media_data.playing_stream_status = STREAM_MEDIA;

			if (app_window_current_get() == (WINDOW *) &nav_mp3_window)
			{
				app_lcd_disp_mp3_playback_time();
			}
		}
		break;

	case UI_MP3_INIT_FAIL:
		//DBG_Printf("UI_MP3_INIT_FAIL\r\n");
		//DBG_Printf("UI_MP3_INIT_FAIL:%d\r\n", wRequestDecodeFrameExit);
		
		app_media_data.playing_stream_status = STREAM_IDLE;
		codec_init_fail = TRUE;

		if (wRequestDecodeFrameExit == FALSE)
		{
			//not cuased by key
			#ifdef SUPPORT_MESSAGE_DISPLAY
			app_nav_window_set(&nav_mp3_prot_window, app_window_data.window_index+1);
			#else
			
			#if 1
			if ( (skip_key_flag == enMP3_PLAY_FLAG) || (skip_key_flag == enMP3_SKIPF_FLAG) )
			{
				uMsgSend (UI_CD_INTERFACE, UI_DECODE_STREAM_EOF, 0);
			}
			else
			{
				if (app_media_data.playing_index == 0)
				{
					app_nav_mp3_stop();
				}
				else
				{
					uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_B, 0);
				}
			}
			#else
			uMsgSend (UI_CD_INTERFACE, UI_DECODE_STREAM_EOF, 0);
			#endif
			
			#endif //SUPPORT_MESSAGE_DISPLAY
		}
		break;


	#if 1//(defined AAC_DEC || defined WAV_DEC || defined FLAC_DEC || defined APE_DEC  || defined SBC_DEC)
	case UI_DEC_INIT_SUCCESS:
		DBG_Printf ("dec start waiting\n\r");
		
		#ifndef RESUME_PLAYBACK_ENABLE
		//find mp3 header may cut some off data, need to reloead again
		app_media_data.resume_jump_request = FALSE;
		app_media_data.playing_time = 0;
		#endif

		#ifdef RESUME_PLAYBACK_ENABLE
		if (1)
		#else
		if (app_media_data.resume_jump_request == FALSE)
		#endif
		{
			app_media_data.playing_stream_status = STREAM_WAITING_DATA;
		}
		break;

	case UI_DEC_INIT_FAIL:
		DBG_Printf("UI_DEC_INIT_FAIL\r\n");
		//DBG_Printf("UI_DEC_INIT_FAIL:%d\r\n", wRequestDecodeFrameExit);

		app_media_data.playing_stream_status = STREAM_IDLE;
		codec_init_fail = TRUE;

		if (wRequestDecodeFrameExit == FALSE)
		{
			//not cuased by key
			#ifdef SUPPORT_MESSAGE_DISPLAY
			app_nav_window_set(&nav_mp3_prot_window, app_window_data.window_index+1);
			#elif defined NOT_SUPPORT_MESSAGE_DISPLAY
			if ( (skip_key_flag == enMP3_PLAY_FLAG) || (skip_key_flag == enMP3_SKIPF_FLAG) )
			{
				uMsgSend (UI_CD_INTERFACE, UI_DECODE_STREAM_EOF, 0);
			}
			else
			{
			if (app_media_data.playing_index == 0)
			{
				app_nav_mp3_stop();
			}
			else
			{
				uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_B, 0);
			}
			}
			#else
			uMsgSend (UI_CD_INTERFACE, UI_DECODE_STREAM_EOF, 0);
			#endif //SUPPORT_MESSAGE_DISPLAY
		}
		break;
	#endif //(defined AAC_DEC || defined WAV_DEC || defined FLAC_DEC || defined APE_DEC || defined SBC_DEC)
	
	
	case UI_FIT_TIMER:
		//DBG_Printf ("status:%s\n\r", app_media_data.playing_stream_status);

		#ifdef AUDIO_LEVEL_METER
		app_lcd_device.display_level(peak_level_L, peak_level_R, 0);
		#endif

		if (app_media_data.playing_stream_status == STREAM_SEARCH_IDV1)
		{
			//app_media_data.playing_stream_status = STREAM_SEARCH_IDV2;
		}
		else if (app_media_data.playing_stream_status == STREAM_SEARCH_IDV2)
		{
			//id3 process is changed in mp3 init process
			app_media_data.playing_stream_status = STREAM_MP3_INIT;
		}
		else if (app_media_data.playing_stream_status == STREAM_MP3_INIT)
		{
			if (app_media_data.playback_state == PLAYING_MODE_PLAY)
			{
				U32 wlen;
				wlen = AUDIO_FIFO_STREAM_DATA_LEN ();

				//if ( (wlen >= (MP3_STREAM_END - MP3_STREAM_START)/2) ||
				//	(wlen >= pMediaRead->lenth) )
				{
					DBG_Puts ("Mp3 Init Tx1\n\r");

					//init mp3
					uDecSend_Init(DECODE_INIT, 0);
					app_media_data.playing_stream_status = STREAM_MP3_INIT_WAIT;
				}
			}
		}
		else if (app_media_data.playing_stream_status == STREAM_MP3_INIT_WAIT)
		{
			//AUDIO_DECODE_EVENT_SET;
			HDD_READ_FILE_EVENT_SET;
		}

	#if 1//(defined AAC_DEC || defined WAV_DEC || defined FLAC_DEC || defined APE_DEC || defined SBC_DEC)
		//for new dec such as aac,wav,flac,ape...
		else if (app_media_data.playing_stream_status == STREAM_DEC_INIT)
		{
			U32 wlen;
			
			if (!AUDIO_FIFO_STREAM_IS_VAILD())
			{
				DBG_Assert (FALSE);
				//break;
			}
			
			wlen = AUDIO_FIFO_STREAM_DATA_LEN ();

			DBG_Printf ("Dec Init Tx\n\r");
			//DBG_Printf ("Dec Init Msg Tx0:%d\n\r", wlen);

			if ( (app_media_data.playback_state == PLAYING_MODE_PLAY) ||
				(app_media_data.playback_state == PLAYING_MODE_PAUSE) )
			{
				if (app_media_data.playing_stream_type == STREAM_WAV)
				{
				#ifdef WAV_DEC
					#if 1
					if (wlen >= (WAV_STREAM_END - WAV_STREAM_START)/2)
					//if ( (wlen >= (WAV_STREAM_END - WAV_STREAM_START)/2) ||
					//if ( (wlen >= (1152*8)) ||
					//	(wlen >= pMediaRead->lenth) )
					#else
					if ( (wlen >= (STREAM_BUF_MAX_SIZE)/2) ||
						(wlen >= pMediaRead->lenth) )
					#endif
					{
						DBG_Puts ("Dec Init Tx1\n\r");

						uDecSend_Init(DECODE_INIT, 0);
						app_media_data.playing_stream_status = STREAM_DEC_INIT_WAIT;
					}
					else if ( (wlen >= pMediaRead->lenth) )
					{
						DBG_Puts ("Dec Init Tx2\n\r");

						uDecSend_Init(DECODE_INIT, 0);
						app_media_data.playing_stream_status = STREAM_DEC_INIT_WAIT;
					}
				#endif
				}
				#ifdef FLAC_DEC
				else if (app_media_data.playing_stream_type == STREAM_FLAC)
				{
					//if (wlen >= (SDRAM_FLAC_STREAM_END - SDRAM_FLAC_STREAM_START)/2)
					if ( (wlen >= (SDRAM_FLAC_STREAM_END - SDRAM_FLAC_STREAM_START)/2) ||
						(wlen >= pMediaRead->lenth) )
					{
						DBG_Puts ("Dec Init Tx3\n\r");
					
						uDecSend_Init(DECODE_INIT, 0);
						app_media_data.playing_stream_status = STREAM_DEC_INIT_WAIT;
					}
				}
				#endif
				#ifdef DOLBY_AC3_DEC
				else if (app_media_data.playing_stream_type == STREAM_AC3)
				{
					//if (wlen >= (SPDIF_DOLBY_AC3_STREAM_END - SPDIF_DOLBY_AC3_STREAM_START)/2)
					if ( (wlen >= (SPDIF_DOLBY_AC3_STREAM_END - SPDIF_DOLBY_AC3_STREAM_START)/2) ||
						(wlen >= pMediaRead->lenth) )
					{
						DBG_Puts ("Dec Init Tx4\n\r");
					
						uDecSend_Init(DECODE_INIT, 0);
						app_media_data.playing_stream_status = STREAM_DEC_INIT_WAIT;
					}
				}
				#endif
				#ifdef DTSDMP_DEC_EANBLE
				else if (app_media_data.playing_stream_type == STREAM_DTSHD)
				{
					//if (wlen >= (SPDIF_DTS_DMP_STREAM_END - SPDIF_DTS_DMP_STREAM_START)/2)
					if ( (wlen >= (SPDIF_DTS_DMP_STREAM_END - SPDIF_DTS_DMP_STREAM_START)/2) ||
						(wlen >= pMediaRead->lenth) )
					{
						DBG_Puts ("Dec Init Tx5\n\r");
					
						uDecSend_Init(DECODE_INIT, 0);
						app_media_data.playing_stream_status = STREAM_DEC_INIT_WAIT;
					}
				}
				#endif
				#if 0//def DOLBY_DDPLUS71_DEC_ENABLE
				else if (app_media_data.playing_stream_type == STREAM_EC3)
				{
					//if (wlen >= (SPDIF_DOLBY_EC3_STREAM_END - SPDIF_DOLBY_EC3_STREAM_START)/2)
					if ( (wlen >= (SPDIF_DOLBY_EC3_STREAM_END - SPDIF_DOLBY_EC3_STREAM_START)/2) ||
						(wlen >= pMediaRead->lenth) )
					{
						DBG_Puts ("Dec Init Tx6\n\r");
					
						uDecSend_Init(DECODE_INIT, 0);
						app_media_data.playing_stream_status = STREAM_DEC_INIT_WAIT;
					}
				}
				#endif
				else
				{
					DBG_Puts ("Dec Init Tx9\n\r");

					if (decode_type != DECODE_DD_AC3)
					{
						uDecSend_Init(DECODE_INIT, 0);
					}
					app_media_data.playing_stream_status = STREAM_DEC_INIT_WAIT;
				}
			}
			else
			{
				DBG_Printf ("Dec Init Tx4:%d\n\r", app_media_data.playback_state);
			}
		}
		else if (app_media_data.playing_stream_status == STREAM_DEC_INIT_WAIT)
		{
			//AUDIO_DECODE_EVENT_SET;
			HDD_READ_FILE_EVENT_SET;
			app_media_data.playing_stream_status = STREAM_WAITING_DATA;
		}
	#endif //(defined AAC_DEC || defined WAV_DEC || defined FLAC_DEC || defined APE_DEC)
		
		else if (app_media_data.playing_stream_status == STREAM_WMA_INIT)
		{
			if (app_media_data.playback_state == PLAYING_MODE_PLAY)
			{
				if (!AUDIO_FIFO_STREAM_IS_VAILD())
				{
					DBG_Assert (FALSE);
					break;
				}

				//len = app_sdram_get_stream_buf_len ();
				len = AUDIO_FIFO_STREAM_FIFO_LEN();

				{
					//init wma, please note wma_init and wma_decoder must be same task
					uDecSend_Init(DECODE_INIT, 0);

					//waitting for decode init result
					app_media_data.playing_stream_status = STREAM_WMA_INIT_WAIT;
				}
			}
		}
		else if (app_media_data.playing_stream_status == STREAM_WMA_INIT_WAIT)
		{
			//AUDIO_DECODE_EVENT_SET;
			HDD_READ_FILE_EVENT_SET;
		}
		else if ( app_media_data.playing_stream_status == STREAM_WAITING_DATA )
		{
			if (!AUDIO_FIFO_STREAM_IS_VAILD())
			{
				DBG_Assert (FALSE);
				//break;
			}

			if (app_media_data.fast_processing)
			{
				len = MIN_STREAM_SIZE;
			}
			else
			{
				#if 1
				len = MIN_STREAM_SIZE;
				#else
				len = MIN_STREAM_SIZE*2;
				#endif
			}

			if ( isEnoughData (len) )
			{
				if (app_media_data.playback_state == PLAYING_MODE_PLAY)
				{
					DBG_Puts ("codec dec start\n\r");

					app_media_data.playing_stream_status = STREAM_MEDIA;
					if (decode_type == DECODE_DD_AC3)
					{
						uDecSend_Frame(DECODE_Frame, TRUE); //start to decode
					}
					else
					{
						uDecSend_Frame(DECODE_Frame, 0); //start to decode
					}
					app_dac_mute_disable ();

					if ( (app_window_current_get() == (WINDOW *) &nav_mp3_window ) ||
						(app_window_current_get() == (WINDOW *) &mp3_mode_window) )
					{
						app_lcd_disp_mp3_playback_time();
					}
				}
			}
		}
		{
			//lcd display
			if ( (app_media_data.playing_stream_status == STREAM_MEDIA) && (app_media_data.fast_processing == FALSE) &&
				(app_media_data.playback_state != PLAYING_MODE_PAUSE) )
			{
				U16 time;
				U16 sample_rate_temp;

				DBG_Assert (app_media_data.playing_stream_sample_rate != 0);
				
				#if 0//def OPTEK_DSRC_ENABLE
				sample_rate_temp = app_media_data.stream_out_sample_rate;
				#else
				sample_rate_temp = app_media_data.playing_stream_sample_rate;
				#endif
				
				time = (U16) (app_media_data.playing_stream_pcm_sample / sample_rate_temp);
				{
					if (time != app_media_data.playing_time)
					{
						watchdog_time_reset();

						app_media_data.fatal_err = 0;			//clear
					
						app_media_data.playing_time = time;

						#ifdef MP3_MODULE
						cEV_Display_Info_Changed = TRUE;
						cEV_Display_Info_Changed_Info = 0x01;			//time
						#endif

						if (app_window_current_get() == (WINDOW *) &nav_mp3_window )
						{
							app_lcd_disp_mp3_playback_time();
						}

					}
				}
			}
			
			if ( (app_media_data.playing_stream_status == STREAM_MEDIA) && 
				(app_media_data.playback_state != PLAYING_MODE_PAUSE) )
			{
				#if 0
				if (app_window_current_get() != (WINDOW *) &volume_window)
				{
					if (pre1_ticks != (timer_data.ticks / TICK_400ms) )
					{
						pre1_ticks = (timer_data.ticks / TICK_400ms);
						app_lcd_disp_playing();
						app_lcd_disp_sleep_icon_flash();
					}
				}
				else
				{
					if (pre1_ticks != (timer_data.ticks / TICK_400ms) )
					{
						pre1_ticks = (timer_data.ticks / TICK_400ms);
						app_lcd_disp_disc_rotary ();
					}
				}
				#endif
			}
			else
			{
				#if 0
				if (pre1_ticks != (timer_data.ticks / TICK_400ms) )
				{
					pre1_ticks = (timer_data.ticks / TICK_400ms);
					app_lcd_disp_disc_rotary();
					app_lcd_disp_sleep_icon_flash();
				}
				#endif
			}

			if ( (app_media_data.playback_state == PLAYING_MODE_PLAY) )
			{
				long no_sound_time;
				no_sound_time = app_timer_get_idle_time ();

				if (no_sound_time > 30000)
				{
					app_media_data.fatal_err++;

					#if 0
					if (1)
					{
						if ( !(audio_vari.volume == 0) )
						//if ( !(audio_vari.volume == 0) || (amp_mute_flag) )
						{
							app_timer_idle_time_reset();
						}
						else
						{
							//skip to next track, ???
							uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_F, 0);
						}
					}
					else
					#endif
					{
						app_nav_mp3_stop ();
					}
				}
			}
		}
		break;
		
	default:
		break;
	}
#endif	
}
#endif /* MP3CD */

void app_no_dummy_process (system_msg_stru *process_msg)
{
}

void app_nav_dummy_process (system_msg_stru *process_msg)
{
}


extern U8 pre_cEV_State;
extern volatile U8 noDiscNum;


void app_nav_usb_read_fs_process (system_msg_stru *process_msg)
{
#if 1//(defined SD_ENABLE || defined USB_HOST_ENABLE)
	
	switch (process_msg->parm)
	{
		case UI_NO_DISC:
			//ANALOG_MUTE_OFF;

			//the no disc nav is used
			#ifdef MP3_MODULE
			app_media_data.status = enSTATE_NO_DISC;
			cEV_State = MSTATE_IDLE;
			cEV_Disc_Contents = TRUE;
			#endif
		
			//app_media_data.ui_background = app_nav_dummy_process;

			DBG_Printf ("usb No Disc\n\r");
			break;

		case UI_FS_SEARCH_END:
			DBG_Printf ("UI_FS_SEARCH_END\n\r");

			#ifdef FOLDER_SWITCH
			app_media_data.fold_switch = FALSE;
			#endif

			app_id3_state_init();

			app_media_data.total_tracks = gwEntryNum;
			app_media_data.total_folders = gwFolderNum;

			{
				//SDRAM_STREAM_CONFIG stream_config;
				
				app_dac_mute_enable ();
				//app_sdram_stream_flush ();

				//stream_config.start_addr = (long) SDRAM_MP3_STREAM_START;
				//stream_config.end_addr = (long) SDRAM_MP3_STREAM_END;
				//stream_config.overwrite = FALSE;
				//app_sdram_stream_open (&stream_config);

				AUDIO_FIFO_STREAM_FLUSH();
		
				U8 *pStreamBuf;
				U32 size;
				
				#ifdef STREAM_BUFFER_SIZE_UNFIXED
				pStreamBuf = MP3_STREAM_START;
				size = MP3_STREAM_END - MP3_STREAM_START;
				#else
				pStreamBuf = stream_buffer;
				#ifdef STREAM_BUFFER_USED_POINTER
				size = STREAM_BUF_MAX_SIZE;
				#else
				size = sizeof(stream_buffer);
				#endif
				#endif

				//AUDIO_FIFO_STREAM_OPEN(pStreamBuf, size);
				fifo_init(&audio_env.stream_fifo, pStreamBuf, size, TRUE);

				//AUDIO_FIFO_STREAM_CREATE(0, STREAM_LENTH_UNKNOWN, 0, 0);
				//fifo_create_file_stream(&audio_env.stream_fifo, 0, STREAM_LENTH_UNKNOWN, 0, 0);
			}

			//uDecSend(DECODE_SET, DECODE_NONE);
			
			#ifdef FILE_SYSTEM_ENABLE

			#ifdef SD_ENABLE
			if (wFileSysPresent || sd1_FileSysPresent) 
			#else
			if (wFileSysPresent)
			#endif

			#if 1
			{
				MYFILE *upgrade_fp;
				//changing to the System folder, find the "upgrade.bin" file
				Fs_change_dir((U16 * ) ROOT_DIR);
				if (Fs_change_dir((U16 *) SYSTEM_FOLDER) == TRUE)
				{
					upgrade_fp = Fs_file_open((U16 *)upgrade_bin, READ_ONLY);
					if (upgrade_fp !=NULL)
					{
						#ifdef UPGRADE_PROJECT_ENABLE
						app_nav_upgrade_project();
						#else

						#ifdef FLASH_PROGRAM_ENABLE
						#if 0
						//upgrade in the same project
						app_flash_program_init();
						app_lcd_disp_upgrade();
						app_nav_sleep(1000);
						
						//app_nav_flash_program_text_copy();
						app_nav_flash_program();

						while (1);
						#else
						uHddMsgSend (SFLASH_UPGRADE_BY_USB, 0, 0);
						app_media_data.total_tracks = 0;
						app_media_data.total_folders = 0;
						app_nav_window_set(&upgrade_window, 0);
						break;
						#endif
						#endif //FLASH_PROGRAM_ENABLE

						#endif //UPGRADE_PROJECT_ENABLE
					}
				}
			}
			#endif

			#endif //FILE_SYSTEM_ENABLE


			if (app_media_data.total_tracks == 0)
			{
				//no digital songs

				#ifdef MP3_MODULE
				app_media_data.status = enSTATE_NO_AUDIO_FILES;
				cEV_State = MSTATE_IDLE;
				cEV_Disc_Contents = TRUE;
				#endif

				//app_media_data.ui_background = app_nav_dummy_process;
				usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

				#if 0
				app_nav_window_set(&nav_no_songs,0);
				#endif
			}
			else
			{
				app_timer_window_time_set (0);
				app_timer_idle_time_reset();

				#if 0//def USB_HOST_ENABLE
				if (getDeviceVID() == iPOD_VID)
				{
				}
				#endif //USB_HOST_ENABLE


				DBG_Printf ("Total Track = %d : Total Folder = %d\n\r", app_media_data.total_tracks, app_media_data.total_folders);
				

				//the file system data is corrupted, init gain
				app_media_data.entry = 0;
				app_media_data.fold = 0;
			
				playing_info.total_of_playlist = 0;

				#ifdef MP3_PROGRAM_ENABLE
				playing_info.total_of_program_list = 0;
				#endif

				//app_sdram_stream_flush();
				AUDIO_FIFO_STREAM_FLUSH();

				app_media_data.playing_stream_status = STREAM_IDLE;
				app_media_data.playback_state = PLAYING_MODE_STOP;

				#ifdef MP3_MODULE
				app_media_data.status = enSTATE_STOP;
				app_media_data.status = enSTATE_DISC_READY;
				cEV_State = MSTATE_STOP;
				cEV_Disc_Contents = TRUE;
				app_media_data.disc_content_ready = TRUE;
				#endif

				//set nav window
				//app_media_data.ui_background = app_nav_dummy_process;
				usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

				#if 0//def FOR_ESD_PROTECT
				if (resume_trk_info.need_resume_play)
				{
					//For ESD
					DBG_Printf("Resume play\n\r");
					//resume_trk_info.need_resume_play = false;

					app_list_create_nomral_playlist ();
					
					ukParmSend(app_nav_mp3_play_track,resume_trk_info.resume_playing_index);

					app_nav_sleep (250);

					ukParmSend(app_cmd_mp3_jump,resume_trk_info.resume_pos);
					ukParmSend(app_cmd_set_playtime,resume_trk_info.resume_playback_time);
					
				}
				else if (app_media_data.auto_play == AUTO_PLAY_OFF)
				{
				#else
				if (app_media_data.auto_play == AUTO_PLAY_OFF)
				{
				#endif
					//set nav window
					//app_media_data.ui_background = app_nav_dummy_process;
					usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

				#ifdef MP3_MODULE
					app_nav_mp3_stop_skipf ();
				#else
					app_nav_window_set(&nav_mp3_stop_window, 0);
				#endif
				}
				else
				{
					//asm("break 1,1");
					app_media_data.auto_play = AUTO_PLAY_OFF;

					//show disc info, then play
					//@app_window_prompt_window_init ();

					#if 0
					prompt_window.window_time = 1000; //1000ms
					prompt_window.draw_region = app_lcd_disp_digital_audio_disc_toc;
					prompt_window.time = app_nav_mp3_play;
					#endif

					//app_media_data.ui_background = app_nav_dummy_process;
					usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

					#if 0
					app_nav_window_set(&prompt_window, 0);
					#endif
				}
			}
			break;

		case UI_FS_ERR:
			/*no file system*/
			DBG_Assert (FALSE);
			break;

		case UI_FIT_TIMER:
			#ifdef LCD_DISP_SEARCH_PROCESS
			if (app_media_data.total_tracks != gwEntryNum)
			{
				app_timer_idle_time_reset();
			
				app_media_data.total_tracks = gwEntryNum;
				app_media_data.total_folders = gwFolderNum + 1;

				app_timer_window_time_set (app_window_current_get()->window_time);
				#if 0
				app_lcd_disp_digital_audio_disc_toc ();
				#endif
			}
			#endif
			break;

		default:
			break;
	}
#endif			
}

#endif
