#include "os_config.h"


#include "app_media_config.h"
#if ( (defined USB_HOST_ENABLE) && (defined USB_HOST_MSC_ENABLE) )
#include "regmap.h"

#include "mem_reloc.h"

//#include "audio_dec.h"
#include "audio_com.h"

#include "hw_pll.h"

#include "hw_uart.h"
#include "hw_pll.h"
#include "hw_timer.h"


#include "app_string_oem.h"

#include "app_main.h"

#include "flash_boot.h"
#include "usbd.h"
#include "app_main.h"
#include "app_key.h"

#include "i2c_cmd.h"
#include "key_scan.h"
#include "app_media.h"
#include "app_list.h"
#include "app_sdram.h"
#include "id3v2.h"
#include "app_window.h"
#include "app_task.h"

#ifdef HDMI_CEC_BY_OPTEK
#include "arc_process.h"
#endif

#ifdef SD_ENABLE
#include "sd_switch.h"
#endif

#ifdef WAV_DEC
#include "wave_decode.h"
#endif

#include "app_media_com.h"

#include "app_media_process.h"
#include "audio_device.h"

#include "app_source.h"
#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif

#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif

#ifdef SAVE_MODULE_ENABLE
#include "app_save.h"
#endif


//#define USB_MP3_PLAY_TEST


#define app_timer_window_time_set     app_window_time_set

#define ukParmSend                    func_w_one_param_msgSend

#define ukMsgSend                     func_wo_param_msgSend

#define app_timer_window_time_set     app_window_time_set

#define uMsgSend(source,parm1,parm2)  system_msgSend(parm1)

#define uDecSend_Set(x,y)             mp3_decocde_set()


#define decode_type                   decode_type_get()

#define app_audio_clock_freq_setting(x)\
                                      audio_clock_freq_set(x)

#define app_nav_media_finish()       0

#define app_timer_key_hold_set_timer(x)

#define app_timer_idle_time_reset()

#define app_lcd_cancel_roll_function()

#define app_lcd_set_roll_index(x)

#define app_lcd_disp_play()

#define app_lcd_disp_mp3_playback_time()

#define app_lcd_disp_program_icon()

#define app_lcd_disp_play(x)

#define app_lcd_stream_type_icon(x)

#define app_lcd_disp_mode(x)

#define app_lcd_disp_mode_icon(x)

#define app_lcd_disp_mp3_Folder_Number(x,y)

#define app_lcd_disp_mp3_fast_time(x)

#define app_lcd_disp_stop()

#define app_lcd_disp_protect()

#define app_lcd_disp_icon_id3_clea()

#define app_lcd_disp_icon_id3_clear()



APP_MEDIA_DATA app_media_data;

U8 skip_key_flag;
volatile U16 keysInQueue = 0;
const U8 STRING_USB_APP[] = {"USBAPP"};
U8 LcdDisp_Roll = FALSE;
U8 codec_init_fail;
volatile U8 wRequestDecodeFrameExit;
ID3_TAG id3_tag;
extern U8 wNoMoreFrames;
U32 pre1_ticks = 0;

U8 decode_block_process_timer;



extern U8 mp3_prog_folder_skipf;
extern U8 mp3_prog_folder_skipb;
extern U8 mp3_playing_folder_skip;

//volatile U8 usbDevType;
//volatile U8 gwUsbDeviceReady;
volatile U8 sdmmcReady;

volatile U16 DispFlashCounter;

U16 pause_ticks;									
U8 flash_ticks = 0;
U8 disp_folder_flash;							
U8 disp_folder_num = 0;

U8 program_roll_tick;							
U16 general_ticks = 0;

U8 program_full = FALSE;

extern U8 prog_num_count;
extern U8 program_add_timer_tick;

U8 pre_digit_key_count = 0;

U8 digit_key_count = 0;
U8 digit_key_phase = 0;

#if 1//def FOR_ESD_PROTECT
U32 ESDTimerout = 0;
RESUME_TRK_INFO resume_trk_info;
#endif

U8 gUSBMesg;

U16 file_name [MAX_DISP_FILE_FOLDER_NAME+1];
U16 folder_name[MAX_DISP_FILE_FOLDER_NAME+1];

//for converting unicode to oem
U16 unicode_name_to_oem[MAX_DISP_FILE_FOLDER_NAME+1];

U16 song_information[MAX_DISP_LENTH+1];

#ifdef FAST_SEARCH_ENABLE
#define FAST_SEARCH_INTERVAL_MS    100
#define FAST_SEARCH_STEP_MS        1000

extern U16 tKeyValue;
extern U8 wma_skip_flag;
#endif


U16	dynamic_list_entry, dynamic_list_attribute;



/************************usb temp**********************/

void app_cmd_clear_snooze_flag(void)
{
#ifdef SNOOZE_FUNC_ENABLE
	Snooze = enSNOOZE_OFF;
#endif
}

void app_cmd_clear_timer_convert_flag(void)
{
#ifdef TIMER_ALARM_ENABLE
	timer_alarm_vol_auto_plus_state = FALSE;
	timer_alarm_power_on = FALSE;
	timer_alarm_power_on_num = 0;
#endif
}

void app_nav_lock_stream_mutex (void)
{
	app_media_mute_enable();
	STREAM_MUTEX_LOCK;
}


void app_nav_unlock_stream_mutex (void)
{
	STREAM_MUTEX_UNLOCK;
}

void app_nav_sleep (U16 ms) 
{
	/*
	*TX_SLEEP_MS(ms);
	*/
	TASK_SLEEP_MS(ms);
}

void uiDecDecodeFrameExit (void)
{	
	if (decode_flag_get())
	{
		decode_exit_req_set();
	}

	decode_exit_wait_forever_msgSend();
}

/****************************************************************************************
*									window fuction										*
*****************************************************************************************/

void app_window_program_roll_init (void)
{
	program_roll_tick = 0;
}

void app_window_program_roll_end (void)
{
	program_roll_tick = 60;
}

void app_window_nav_next_roll (void)
{
	I16 m;
	I16 cursor_item;
	I16 title_line;
	I16 items;

	#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (SkipKey == enKEY_REMOTE_SKIPFAST)
	{
		SkipKey = enKEY_SKIP_UNKNOWN;
		return;
	}
	#endif

	#ifdef MP3_FOLDER_KEY_SUPPORT
	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch && (app_window_current_get() == (WINDOW *)&mp3_folder_nav_window))
	{
		return;
	}
	#endif
	#endif

	app_lcd_cancel_roll_function();
	app_timer_window_time_set (app_window_current_get()->window_time);

	if (app_media_data.pgm_edit) {
		app_window_program_roll_init ();
	}

	m = app_window_data.window_index;
	
	cursor_item = app_window_data.app_window[m].list_index + 
		app_window_data.app_window[m].list_cursor;

	if (app_window_data.app_window[m].window->list_entry == 0)
	{
		//dynamic create list
		items = dynamic_list_entry;							
	}
	else
	{
		items = app_window_data.app_window[m].window->list_entry;
	}
	
	if ( (cursor_item + 1) < items )
	{
		
		//cursor is not in last item
		if (app_window_data.app_window[m].window->title != NULL)
		{
			title_line = 1;
		}
		else
		{
			title_line = 0;
		}

		if ( (app_window_data.app_window[m].window->region.start_line + title_line +
			app_window_data.app_window[m].list_cursor + 1) < 
			app_window_data.app_window[m].window->region.end_line)
		{	
			app_window_data.app_window[m].list_cursor++;
		}
		else
		{
			app_window_data.app_window[m].list_index++;
		}
	
		cursor_item++;

		app_window_data.app_window[m].window->draw_region();
	}
	else
	{
		app_window_data.app_window[app_window_data.window_index].list_index = 0;
		app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
		app_window_data.app_window[m].window->draw_region();
	}

	app_window_program_roll_init ();
	app_window_disp_track_init();
	app_lcd_set_roll_index(3);
}

void app_window_nav_prev_roll (void)
{
	I16 m;
	I16 cursor_item;
	I16 items;

	#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (SkipKey == enKEY_REMOTE_SKIPFAST)
	{
		SkipKey = enKEY_SKIP_UNKNOWN;
		return;
	}
	#endif

	#ifdef MP3_FOLDER_KEY_SUPPORT
	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch && (app_media_data.window == (WINDOW *)&mp3_folder_nav_window))
	{
		return;
	}
	#endif
	#endif

	app_lcd_cancel_roll_function();
	app_timer_window_time_set (app_media_data.window->window_time);

	if (app_media_data.pgm_edit) {
		app_window_program_roll_init ();
	}

	m = app_window_data.window_index;

	cursor_item = app_window_data.app_window[m].list_index + 
		app_window_data.app_window[m].list_cursor;

	if (app_window_data.app_window[m].window->list_entry == 0)
	{
		//dynamic create list
		items = dynamic_list_entry;							
	}
	else
	{
		items = app_window_data.app_window[m].window->list_entry;
	}

	if ( cursor_item > 0 )
	{		
		//cursor is not in the first item
		if ( app_window_data.app_window[m].list_cursor > 0)
		{
			app_window_data.app_window[m].list_cursor--;
		}
		else
		{
			app_window_data.app_window[m].list_index--;	
		}
	
		cursor_item--;

		app_window_data.app_window[m].window->draw_region();
	}
	else
	{
		app_window_data.app_window[app_window_data.window_index].list_index = items - 1;
		app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
		app_window_data.app_window[m].window->draw_region();
	}

	app_window_program_roll_init ();
	app_window_disp_track_init();
	app_lcd_set_roll_index(3);	
}


#ifdef MP3_FOLDER_KEY_SUPPORT
#ifdef LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
void app_window_nav_folder_next_roll (void)
{
	I16 m;
	I16 cursor_item;
	I16 title_line;
	I16 items;

	#ifdef FILE_DELETE_ENABLE
	app_media_data.MediaDeleting = FALSE;
	#endif

	#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (rmt_keySkipType == enKEY_REMOTE_SKIPFAST)
	{
		rmt_keySkipType = enKEY_SKIP_UNKNOWN;
		return;
	}
	#endif

	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch && (app_media_data.window == (WINDOW *) &mp3_folder_nav_window))
	{
		return;
	}
	#endif

	app_lcd_cancel_roll_function();
	app_timer_window_time_set (app_media_data.window->window_time);
	app_window_timer_count_init();

	if (app_media_data.pgm_edit) {
		app_window_program_roll_init ();
	}

	m = app_window_data.window_index;
	
	cursor_item = app_window_data.app_window[m].list_index + 
		app_window_data.app_window[m].list_cursor;

	if (app_window_data.app_window[m].window->list_entry == 0) {
		//dynamic create list
		items = dynamic_list_entry;							
	}
	else {
		items = app_window_data.app_window[m].window->list_entry;
	}
	
	if ( (cursor_item + 1) < items )
	{
		//cursor is not in last item
		if (app_window_data.app_window[m].window->title != NULL)
		{
			title_line = 1;
		}
		else
		{
			title_line = 0;
		}

		if ( (app_window_data.app_window[m].window->region.start_line + title_line +
			app_window_data.app_window[m].list_cursor + 1) < 
			app_window_data.app_window[m].window->region.end_line) {
			
			app_window_data.app_window[m].list_cursor++;
		}
		else {
			app_window_data.app_window[m].list_index++;
		}
	
		cursor_item++;

		app_window_data.app_window[m].window->draw_region();
	}
	else {
		app_window_data.app_window[app_window_data.window_index].list_index = 0;
		app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
		app_window_data.app_window[m].window->draw_region();
	}

	app_window_program_roll_init ();
	app_window_disp_track_init();
	app_lcd_set_roll_index(3);

#ifdef LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
	window_disp_state = enDISP_FOLDER_NAME_ROLLING_START;
#endif
}

void app_window_nav_folder_prev_roll (void)
{
	I16 m;
	I16 cursor_item;
	I16 items;

#ifdef FILE_DELETE_ENABLE
	app_media_data.MediaDeleting = FALSE;
#endif

#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (rmt_keySkipType == enKEY_REMOTE_SKIPFAST)
	{
		rmt_keySkipType = enKEY_SKIP_UNKNOWN;
		return;
	}
#endif

#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch && (app_media_data.window == (WINDOW *) &mp3_folder_nav_window))
	{
		return;
	}
#endif

	app_lcd_cancel_roll_function();
	app_timer_window_time_set (app_media_data.window->window_time);
	app_window_timer_count_init();

	if (app_media_data.pgm_edit) {
		app_window_program_roll_init ();
	}

	m = app_window_data.window_index;

	cursor_item = app_window_data.app_window[m].list_index + 
		app_window_data.app_window[m].list_cursor;

	if (app_window_data.app_window[m].window->list_entry == 0) {
		//dynamic create list
		items = dynamic_list_entry;							
	}
	else {
		items = app_window_data.app_window[m].window->list_entry;
	}

	if ( cursor_item > 0 ) {
		
		//cursor is not in the first item
		if ( app_window_data.app_window[m].list_cursor > 0) {
			app_window_data.app_window[m].list_cursor--;
		}
		else {
			app_window_data.app_window[m].list_index--;	
		}
	
		cursor_item--;

		app_window_data.app_window[m].window->draw_region();
	}
	else {
		app_window_data.app_window[app_window_data.window_index].list_index = items - 1;
		app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
		app_window_data.app_window[m].window->draw_region();
	}

	app_window_program_roll_init ();
	app_window_disp_track_init();
	app_lcd_set_roll_index(3);
	
#ifdef LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
	window_disp_state = enDISP_FOLDER_NAME_ROLLING_START;
#endif
}
#endif //LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
#endif //MP3_FOLDER_KEY_SUPPORT


void app_window_nav_next_hold_roll (void)
{
#if 1

#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (SkipKey == enKEY_REMOTE_SKIPFAST)
	{
		SkipKey = enKEY_SKIP_UNKNOWN;
		return;
	}
#endif
	
	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_key_hold_set_timer (200);
	app_window_nav_next_roll ();

	app_timer_window_time_set (app_media_data.window->window_time);
#else
	U8 temp = app_media_data.disp_info;

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;
	keysInQueue = 0;
	while (keysInQueue == 0)
	{
		app_window_nav_next_roll ();
		app_timer_window_time_set (0);
		app_nav_sleep (250); 
		
		if (app_media_data.pgm_edit)
		{
			app_window_program_roll_init ();
		}
	}

	app_timer_window_time_set (app_media_data.window->window_time);
#endif
}

void app_window_nav_prev_hold_roll (void)
{	
#if 1

#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (SkipKey == enKEY_REMOTE_SKIPFAST)
	{
		SkipKey = enKEY_SKIP_UNKNOWN;
		return;
	}
#endif

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_key_hold_set_timer (200);
	app_window_nav_prev_roll ();

	app_timer_window_time_set (app_media_data.window->window_time);
#else
	U8 temp = app_media_data.disp_info;

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;
	keysInQueue = 0;
	while (keysInQueue == 0) {
		app_window_nav_prev_roll ();
		app_timer_window_time_set (0);
		app_nav_sleep (250); 

		if (app_media_data.pgm_edit) {
			app_window_program_roll_init ();
		}
	}

	app_timer_window_time_set (app_media_data.window->window_time);
#endif
}

/************************usb temp end******************/

#ifdef USB_HOST_ENABLE

// devType: enum_USB_DEV_TYPE
void app_cmd_usbdev_disconnect_callbk(int devType )
{
	DBG_Printf("usb disconnect devType:%d\n\r",  devType );

	if( enUSB_DEV_HUB == devType )
	{
		//display message
	}
}

void app_nav_usb_device_disconnected(void)
{	
	DBG_Puts ("app usb dev disconnected\n\r");

	/*
	*if (app_media_data.standby_status)
	*{
	*	return;
	*}
	*/

#ifdef SD_ENABLE
	if ( (global_vari.audio_source != SOURCE_USB_MSC) && (global_vari.audio_source != SOURCE_SD) )
#else
	if (global_vari.audio_source != SOURCE_USB_MSC)
#endif
	{
		return;
	}

	if (app_window_current_get() == (WINDOW *)&readingcd_window)
	{
		return;
	}

	//app_media_data.ui_background = app_nav_dummy_process;
	usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

	app_timer_window_time_set (0);

	app_cmd_DecoderExit();

	app_lcd_stream_type_icon (STREAM_UNKNOWN);

	app_media_data.playing_stream_status = STREAM_IDLE;

	#ifdef RESUME_PLAYBACK_ENABLE
	app_media_data.resume_track = app_media_data.playing_track;
	app_media_data.resume_playing_index = app_media_data.playing_index;
	app_media_data.resume = RESUME_ON;
	app_media_data.resume_playback = TRUE;
	app_media_data.resume_playback_request = FALSE;
	app_media_data.resume_playback_time = app_media_data.playing_time;
	app_media_data.resume_sample_rate = app_media_data.playing_stream_sample_rate;
	
	if (app_media_data.playing_stream_total_time == 0) {
		//unknown the stream total playing time
		app_media_data.resume_playback_time = 0;
	}
	#else
	app_media_data.resume = RESUME_OFF;
	app_media_data.resume_playback = FALSE;
	#endif
	
	app_media_data.playback_mode = NORMAL_PLAY;
	app_lcd_disp_mode ();
	app_lcd_disp_mode_icon();
	app_lcd_disp_icon_id3_clear();
	app_media_data.playlist = PLAYLIST_OFF;
	app_lcd_disp_program_icon ();

	//flush sdram buf
	AUDIO_FIFO_STREAM_FLUSH();

	//random base
	random_set_base (random ());
	
	app_media_data.playback_state = PLAYING_MODE_STOP;
	app_media_data.status = enSTATE_OPEN;

	app_media_data.total_tracks = 0;
	app_media_data.total_folders = 0;

	app_media_data.disc_content_ready = FALSE;
	app_media_data.toc_ready = FALSE;
	
	app_nav_window_set_no_draw(&no_disc_window, 0);
}

void app_nav_usb_plug_in(void)
{
	DBG_Printf("app usb plug in enter\n\r");

	if (global_vari.audio_source != SOURCE_USB_MSC)
	{
		return;
	}

	app_cmd_DecoderExit();

	//the no disc nav is used
	if (app_window_current_get() != (WINDOW *) &readingcd_window)
	{
		app_nav_window_set(&readingcd_window, 0);
	}

	app_window_readingcd_window_init ();

	//DBG_Printf("app usb plug in exit\n\r");
}

void app_nav_usb_plug_in_2(void)
{
	DBG_Puts("usb plug in 2\n\r");
}
	
void app_nav_usb_plug_in_3(void)
{
	DBG_Puts("usb plug in 3\n\r");
}

void app_nav_usb_hub_plug_in(void)
{
	DBG_Printf("app usb hub plug in enter\n\r");
	
	if (global_vari.audio_source != SOURCE_USB_MSC)
	{
		return;
	}
			
	app_cmd_DecoderExit();
			
	//the no disc nav is used
	if (app_window_current_get() != (WINDOW *) &readingcd_window)
	{
		app_nav_window_set(&readingcd_window, 0);
	}

	app_window_readingcd_window_init ();
}

// devType: enum_USB_DEV_TYPE
void app_cmd_usbdev_connect_callbk(int devType )
{
#if 1
	DBG_Printf("usb connect devType:%d\n\r",  devType );

	if( enUSB_DEV_TEST == devType ) {
		// display message: "test device"
	}
	else if( enUSB_DEV_HUB == devType ) {
		// display message
	}
	else if( enUSB_DEV_TEST_UNKOWN == devType ) {
		// display message
	}
	else
	{
	}
#endif
}

void app_nav_usb_device_connected (void)
{
#if 1
	U8 i;

	DBG_Printf("%s\r\n",__func__);

#ifdef SD_ENABLE 
	if ( (global_vari.audio_source != SOURCE_USB_MSC) && (global_vari.audio_source != MEDIA_SDMMC) )
#else
	if (global_vari.audio_source != SOURCE_USB_MSC)
#endif
	{
		return;
	}


	//app_media_data.ui_background = app_nav_dummy_process;
	usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

	app_lcd_stream_type_icon (STREAM_UNKNOWN);
	
	app_media_data.auto_play = AUTO_PLAY_OFF;
	app_media_data.playback_mode = NORMAL_PLAY;
	//app_lcd_disp_mode ();
	app_lcd_disp_mode_icon();

	app_media_data.playlist = PLAYLIST_OFF;
	app_lcd_disp_program_icon ();

	app_dac_mute_enable ();	
	
	app_media_data.playback_intro = INTRO_OFF;
	app_media_data.playback_mode = NORMAL_PLAY;
	
	#ifdef RESUME_PLAYBACK_ENABLE
	asm("nop");
	#else
	app_media_data.resume_playback = FALSE;
	#endif
	
	app_media_data.resume_playback_request = FALSE;
	app_media_data.playing_stream_status = STREAM_IDLE;
	app_media_data.playback_state = PLAYING_MODE_STOP;
	app_media_data.total_playlist_entry = 0;
	app_media_data.status = enSTATE_OPEN;
	
	app_media_data.pgm_edit = FALSE;
	
	app_media_data.disc_content_ready = FALSE;
	app_media_data.toc_ready = FALSE;

	//flush sdram buf
	AUDIO_FIFO_STREAM_FLUSH ();
	
	//random base
	random_set_base (random ());

	//the no disc nav is used
	app_nav_window_set(&readingcd_window, 0);

	app_dac_mute_enable ();	

	//change clock to cdrom clock
	
	app_media_data.playing_time = 0;
	app_media_data.total_tracks = 0;
	app_media_data.fold = 0;
	app_media_data.entry = 0;

#ifdef MP3_MODULE
	cEV_State = MSTATE_TOCREAD;
	app_media_data.status = enSTATE_READING_TOC;
#endif

	//app_media_data.ui_background = app_nav_usb_read_fs_process;
	usb_msc_process_id = USB_MSC_READ_FS_PROCESS;

	app_media_mute_disable ();

#ifdef PTP_MTP_CLASS
	if (usbDevType == enUSBH_DEV_MTP)
	{
		uHddMsgSend (USBH_MTP_SEARCH_AUDIO_ENTRY, 0, 0);
	}
	else
#endif
	{
		if (global_vari.audio_source == SOURCE_USB_MSC)
		{
			uHddMsgSend (HDD_SEARCH_AUDIO_ENTRY, 0, 0);
		}
		else
		{
			//sdmmc
			uHddMsgSend (HDD_SEARCH_AUDIO_ENTRY, 0, 0);
		}
	}

#ifdef AUTO_PLAY_ENABLE
	app_media_data.auto_play = AUTO_PLAY_ON;
#endif
#endif
}

void app_nav_usb_disc_switch(void)
{
	U8 i;

	DBG_Printf("usb lun open switch\n\r");

	if (global_vari.audio_source != SOURCE_USB_MSC)
	{
		return;
	}

	app_cmd_DecoderExit();
	
#ifdef PTP_MTP_CLASS
	if (usbDevType == enUSBH_DEV_MTP)
	{
		return;
	}
	else
#endif
	{
		if (global_vari.audio_source == SOURCE_USB_MSC)
		{
			//app_media_data.ui_background = app_nav_usb_read_fs_process;
			usb_msc_process_id = USB_MSC_READ_FS_PROCESS;

			if (usb_lunIndex < (usb_lunCount-1))
			{
				usb_lunIndex ++;
			}
			else
			{
				usb_lunIndex = 0;
			}

			uHddMsgSend (USB_LUN_AUDIO_ENTRY, usb_lunIndex, 0);
		}
	}
}

#else
void app_nav_usb_device_connected (void)
{
}
void app_nav_usb_disc_switch(void)
{
}

// devType: enum_USB_DEV_TYPE
void app_cmd_usbdev_connect_callbk(int devType )
{
}

// devType: enum_USB_DEV_TYPE
void app_cmd_usbdev_disconnect_callbk(int devType )
{
}

#endif //USB_HOST_ENABLE


#ifdef SD_ENABLE
void app_nav_media_sd_insert(void)
{
	if (global_vari.audio_source == MEDIA_SDMMC)
	{
		app_media_mute_enable();
	}

	uHddMsgSend (SDMMC_INIT, 0, 0);
}

void app_nav_medid_sd_remove(void)
{
	if (global_vari.audio_source == MEDIA_SDMMC)
	{
		app_media_mute_disable();
	}

	uHddMsgSend (SDMMC_CLOSE, 0, 0);
}
#endif

/************************************************************/
/*				NODISC NAV FUCTION							*/
/************************************************************/

void app_nav_nodisc_enter (void)
{
}

void app_nav_nodisc_stop (void)
{
}

void app_nav_nodisc_menu (void)
{
}

void app_nav_mp3_disp_init(void)
{
#if 0
	switch (app_media_data.disp_info)
	{
	case DISP_INFO_PLAYBACK_TIME:
		app_media_data.disp_info = DISP_INFO_FILE_NAME;
		break;

	case DISP_INFO_PAUSE_TIME:
		app_media_data.disp_info = DISP_INFO_FILE_NAME;
		break;

	case DISP_INFO_FILE_NAME:
		app_media_data.disp_info = DISP_INFO_FOLDER_NAME;
		break;

	case DISP_INFO_FOLDER_NAME:
		app_media_data.disp_info = DISP_INFO_TITLE;
		break;

	case DISP_INFO_ID3:
		app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;
		break;

	case DISP_INFO_TITLE:
		app_media_data.disp_info = DISP_INFO_ARTIST;
		break;
	
	case DISP_INFO_ARTIST:
		app_media_data.disp_info = DISP_INFO_ALBUM;
		break;
	
	case DISP_INFO_ALBUM:
	default:
		if (app_media_data.playback_state == PLAYING_MODE_PLAY)
		{
			app_media_data.disp_info = DISP_INFO_PLAYBACK_TIME;
		}
		else
		{
			app_media_data.disp_info = DISP_INFO_PAUSE_TIME;
		}
		break;
	}	

	app_id3_state_set(app_media_data.disp_info);

	app_lcd_disp_mp3_info();
#endif
}

/**************************************************************************************/
/*				MP3  NAV FUCTION							                          */
/**************************************************************************************/

void app_nav_mp3_play_track (U16 index)
{
	app_nav_usb_mp3_play_track(index);
}

void app_nav_usb_mp3_play_track (U16 index)
{
#if ( (defined MP3_DECODE_ENABLE) || (defined WMA_DECODE_ENABLE) )
	U16 entry_index;
	long len;
	long start_addr, cluster;
	U8 *pStreamBuf;
	U32 size;


	DBG_Assert (index < playing_info.total_of_playlist);
	
	//app_media_data.ui_background = app_nav_dummy_process;
	usb_msc_process_id = USB_MSC_DUMMY_PROCESS;
	
	app_cmd_DecoderExit();
	
	app_timer_idle_time_reset (); //reset idle timer

	app_media_data.playing_stream_status = STREAM_IDLE;

	//flush stream buf	
	AUDIO_FIFO_STREAM_FLUSH();

	app_media_data.playback_state = PLAYING_MODE_PLAY;

#ifdef MP3_MODULE
	cEV_State = MSTATE_PLAY;
	app_media_data.status = enSTATE_PLAYING;
#endif

	app_media_data.cdrom_pre_baddr = 0;
	app_media_data.resume_jump_request = FALSE;
	app_media_data.fast_jump_request = FALSE;

	app_media_data.playing_index = index;
	app_media_data.stream_index = index;


	app_nav_lock_stream_mutex ();

	app_media_data.playing_track = app_list_get_list_content(app_media_data.playing_index);	 //playing_info.playlist[app_media_data.playing_index];
	app_media_data.stream_track = app_list_get_list_content(app_media_data.playing_index);	//playing_info.playlist[app_media_data.stream_index];

#ifdef MP3_MODULE
	cEV_Display_Info_Changed = TRUE;
	cEV_Display_Info_Changed_Info = 0x07;
#endif

	DBG_Printf ("playing_track %d\n\r", app_media_data.playing_track + 1);


	if (pMediaRead != NULL)
	{
#ifdef FILE_SYSTEM_ENABLE
		Fs_close_file (pMediaRead);
#endif
	}


	app_media_data.fold = app_file_get_folder_of_entry(app_media_data.playing_track);
	app_media_data.entry = app_file_get_entry_in_folder(app_media_data.stream_track);

	//DBG_Printf ("Playing entry %d in Folder %d\n\r",  app_media_data.entry, app_media_data.fold);

	{

#ifdef FILE_SYSTEM_ENABLE
#ifdef ONLY_DIGITAL_DISPLAY
		start_addr = folder_info[app_media_data.playing_track].cluster;
#else 
		app_file_search_folder (folder_info[app_media_data.fold].parent_cluster,
				folder_info[app_media_data.fold].index, folder_name, MAX_DISP_FILE_FOLDER_NAME, &start_addr);
#endif		
#endif

		//DBG_Printf ("Folder Cluster %d\n\r", start_addr);

#ifdef FILE_SYSTEM_ENABLE
		U16 cdate;
		U16 ctime;

#ifndef NEW_FS_LIB
		app_file_search_file (start_addr, (app_media_data.entry+1), file_name, MAX_DISP_FILE_FOLDER_NAME, &(app_media_data.playing_stream_type), &cluster, &len);
#else
		app_file_search_file (start_addr, (app_media_data.entry+1), file_name, MAX_DISP_FILE_FOLDER_NAME,
			&(app_media_data.playing_stream_type), &cluster, &len, &cdate, &ctime);
#endif
#endif //FILE_SYSTEM_ENABLE

		//DBG_Printf ("Playing Track Cluster %d and Len %d\n\r", cluster, len);

		char *pFileNaem = app_sdram_get_file_name(app_media_data.playing_track);
		DBG_Printf ("file name:%s\n\r", pFileNaem);

#ifdef FILE_SYSTEM_ENABLE
		pMediaRead = app_file_open_entry_by_cluster (cluster, len, READ_ONLY);
		DBG_Assert (pMediaRead != NULL);
#endif

	}

	app_nav_unlock_stream_mutex ();

	app_lcd_stream_type_icon (app_media_data.playing_stream_type);

	if ( (app_media_data.resume_playback_request == TRUE) &&
		(app_media_data.resume_playback_time > 0) )
	{

		app_media_data.resume_playback_request = FALSE;
		app_media_data.resume_jump_request = TRUE;
		app_media_data.playing_time = app_media_data.resume_playback_time;
		
		app_media_data.playing_stream_pcm_sample = (long) app_media_data.resume_sample_rate * (long) app_media_data.playing_time;
	}
	else 
	{
		app_media_data.resume_playback_request = FALSE;
		app_media_data.resume_jump_request = FALSE;
		app_media_data.playing_time = 0;
		app_media_data.playing_stream_pcm_sample = 0;
	}
	
	/*set nav window*/
	app_copy_song_pathname (app_media_data.playing_track);
	
	if (app_window_current_get() != (WINDOW *) &nav_mp3_window) 
	{		
		#ifdef DISP_SOURCE_PLAY_PAUSE
		app_lcd_disp_playing_status();
		#endif

		app_nav_mp3_play_window_init();
	}
	else if (app_window_data.window_index == 0) 
	{
		//app_nav_mp3_play_window_init();		
		app_nav_init_draw_playback_time();
	}
	else if (app_window_current_get() == (WINDOW*) &nav_mp3_prot_window)//For clear "Not Supptort"
	{
		app_nav_window_set(&nav_mp3_window, 0);
	}
	
	
	//add August 17, 2007 for display
	app_lcd_disp_mp3_Folder_Number(0, app_media_data.playing_track);

	if (app_media_data.playing_stream_type == STREAM_MP3)
	{
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

		//AUDIO_FIFO_STREAM_CREATE(0, len, 0, 0);
	    fifo_create_file_stream(&audio_env.stream_fifo, 0, len, 0, 0);

		#ifdef FOR_ESD_PROTECT
		if (!resume_trk_info.need_resume_play)
		#endif
		{
			ID3v2_init (&id3_tag);
			entry_id3_flag = ID3_UNKNOWN;	
		}
		
		app_media_data.playing_stream_status = STREAM_SEARCH_IDV2;

		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
		
		{
			//usb device
			app_nav_lock_stream_mutex ();
			
			app_media_data.wMediaStatus = WSTREAM_BUSY;

			uHddMsgSend(HDD_READ_FILE,0,0);
			app_media_mute_disable ();

			app_nav_unlock_stream_mutex ();
			app_nav_sleep(10);
		}
		
		uDecSend_Set(DECODE_SET, DECODE_MP3);

		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}
	else if (app_media_data.playing_stream_type == STREAM_WMA)
	{
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

		//AUDIO_FIFO_STREAM_CREATE(0, len, 0, 0);
	    fifo_create_file_stream(&audio_env.stream_fifo, 0, len, 0, 0);

		app_media_data.playing_stream_status = STREAM_WMA_INIT;
		entry_id3_flag = ID3_UNKNOWN;					
		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
			
		{
			//usb device
			app_nav_lock_stream_mutex ();

			#ifdef PTP_MTP_CLASS
			if (usbDevType == enUSBH_DEV_MTP)
			{
				//MTP
				app_media_data.wMediaStatus = WSTREAM_BUSY;

				//uHddMsgSend (HDD_POWER_ON,0,0);
				if (usbmtpif_isGetPartialObjFeature()) {
					uHddMsgSend (USBH_MTP_READ_PARTIAL_FILE,cluster,0);
				}
				else {
					uHddMsgSend (USBH_MTP_READ_FILE,cluster,0);
				}
				app_media_mute_disable ();
			}
			else
			#endif
			{	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				uHddMsgSend (HDD_READ_FILE,0,0);	
				app_media_mute_disable ();
			}
			
			app_nav_unlock_stream_mutex ();		
		}

		uDecSend_Set(DECODE_SET, DECODE_WMA);
		
		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}

	#ifdef AAC_DEC
	else if (app_media_data.playing_stream_type == STREAM_AAC_RAW ||
		app_media_data.playing_stream_type == STREAM_ADTS ||
		app_media_data.playing_stream_type == STREAM_ADIF ||
		app_media_data.playing_stream_type == STREAM_LOAS ||
		app_media_data.playing_stream_type == STREAM_M4A )
	{
		app_media_data.playing_stream_status = STREAM_DEC_INIT;
		
		entry_id3_flag = ID3_UNKNOWN;					
		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
			
		{
			//usb device
			//DBG_Assert (FALSE);
			app_nav_lock_stream_mutex ();

			{	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				//uHddMsgSend (HDD_POWER_ON,0,0);
				uHddMsgSend (HDD_READ_FILE,0,0);	
				app_media_mute_disable ();
			}
			
			app_nav_unlock_stream_mutex ();		
		}

		uDecSend_Set(DECODE_SET, DECODE_AAC);
		
		
		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}
	#endif //AAC_DEC

	#ifdef WAV_DEC
	else if (app_media_data.playing_stream_type == STREAM_WAV)
	{
		#ifdef STREAM_BUFFER_SIZE_UNFIXED
		pStreamBuf = WAV_STREAM_START;
		size = WAV_STREAM_END - WAV_STREAM_START;
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

		//AUDIO_FIFO_STREAM_CREATE(0, len, 0, 0);
		fifo_create_file_stream(&audio_env.stream_fifo, 0, len, 0, 0);

		app_media_data.playing_stream_status = STREAM_DEC_INIT;
		entry_id3_flag = ID3_UNKNOWN;					
		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
			
		{
			//usb device
			//DBG_Assert (FALSE);
			app_nav_lock_stream_mutex ();

			{	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				uHddMsgSend (HDD_READ_FILE,0,0);	
				app_media_mute_disable ();
			}
			
			app_nav_unlock_stream_mutex ();		
		}

		uDecSend_Set(DECODE_SET, DECODE_WAV);
		
		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}
	#endif //WAV_DEC

	#ifdef FLAC_DEC
	else if (app_media_data.playing_stream_type == STREAM_FLAC)
	{
		#ifdef STREAM_BUFFER_SIZE_UNFIXED
		pStreamBuf = FLAC_STREAM_START;
		size = FLAC_STREAM_END - FLAC_STREAM_START;
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

		//AUDIO_FIFO_STREAM_CREATE(0, len, 0, 0);
	    fifo_create_file_stream(&audio_env.stream_fifo, 0, len, 0, 0);


		app_media_data.playing_stream_status = STREAM_DEC_INIT;
		entry_id3_flag = ID3_UNKNOWN;					
		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
			
		{
			//usb device
			//DBG_Assert (FALSE);
			app_nav_lock_stream_mutex ();

			{	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				//uHddMsgSend (HDD_POWER_ON,0,0);
				uHddMsgSend (HDD_READ_FILE,0,0);	
				app_media_mute_disable ();
			}
			
			app_nav_unlock_stream_mutex ();		
		}

		uDecSend_Set(DECODE_SET, DECODE_FLAC);
		
		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}
	#endif //FLAC_DEC

	#ifdef APE_DEC
	else if (app_media_data.playing_stream_type == STREAM_APE)
	{
		#ifdef STREAM_BUFFER_SIZE_UNFIXED
		pStreamBuf = WAV_STREAM_START;
		size = WAV_STREAM_END - WAV_STREAM_START;
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

		//AUDIO_FIFO_STREAM_CREATE(0, len, 0, 0);
	    fifo_create_file_stream(&audio_env.stream_fifo, 0, len, 0, 0);

		app_media_data.playing_stream_status = STREAM_DEC_INIT;
		entry_id3_flag = ID3_UNKNOWN;					
		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
			
		{
			//usb device
			//DBG_Assert (FALSE);
			app_nav_lock_stream_mutex ();

			{	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				//uHddMsgSend (HDD_POWER_ON,0,0);
				uHddMsgSend (HDD_READ_FILE,0,0);	
				app_media_mute_disable ();
			}
			
			app_nav_unlock_stream_mutex ();		
		}				

		uDecSend_Set(DECODE_SET, DECODE_FLAC);

		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}
	#endif //APE_DEC

	#ifdef SBC_DEC
	else if (app_media_data.playing_stream_type == STREAM_SBC)
	{	
		#ifdef STREAM_BUFFER_SIZE_UNFIXED
		pStreamBuf = BT_STREAM_START;
		size = BT_STREAM_END - WAV_STREAM_START;
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

		//AUDIO_FIFO_STREAM_CREATE(0, len, 0, 0);
	    fifo_create_file_stream(&audio_env.stream_fifo, 0, len, 0, 0);

		app_media_data.playing_stream_status = STREAM_DEC_INIT;
		entry_id3_flag = ID3_UNKNOWN;					
		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
		
		{
			//usb device
			//DBG_Assert (FALSE);
			app_nav_lock_stream_mutex ();
			
			{	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				//uHddMsgSend (HDD_POWER_ON,0,0);
				uHddMsgSend (HDD_READ_FILE,0,0);	
				app_media_mute_disable ();
			}
			
			app_nav_unlock_stream_mutex ();		
		}				
		
		uDecSend_Set(DECODE_SET, DECODE_SBC);

		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}
	#endif //SBC_DEC

	#if (defined DOLBY_AC3_DEC_TEST && (defined DOLBY_AC3_DEC))
	else if (app_media_data.playing_stream_type == STREAM_AC3)
	{
		#ifdef STREAM_BUFFER_SIZE_UNFIXED
		#if 1
		pStreamBuf = SPDIF_DOLBY_AC3_STREAM_START;
		size = SPDIF_DOLBY_AC3_STREAM_END - SPDIF_DOLBY_AC3_STREAM_START;
		#else
		pStreamBuf = SPDIF_STREAM_START;
		size = SPDIF_STREAM_END - SPDIF_STREAM_START;
		#endif
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

		//AUDIO_FIFO_STREAM_CREATE(0, len, 0, 0);
	    fifo_create_file_stream(&audio_env.stream_fifo, 0, len, 0, 0);

   
		//app_media_data.playing_stream_status = STREAM_WAITING_DATA;
		app_media_data.playing_stream_status = STREAM_DEC_INIT;
		entry_id3_flag = ID3_UNKNOWN;					
		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
		
		{
			//usb device
			//DBG_Assert (FALSE);
			app_nav_lock_stream_mutex ();
			
			{	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				//uHddMsgSend (HDD_POWER_ON,0,0);
				uHddMsgSend (HDD_READ_FILE,0,0);
				app_media_mute_disable ();
			}
			
			app_nav_unlock_stream_mutex ();
			//app_nav_sleep(100);
			app_nav_sleep(1000);
		}

		uDecSend_Set(DECODE_SET, DECODE_DD_AC3);

		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}
	#endif //DOLBY_AC3_DEC_TEST
	#if (defined DTSDMP_DEC_TEST && (defined DTSDMP_DEC_EANBLE))
	else if (app_media_data.playing_stream_type == STREAM_DTSHD)
	{
		#ifdef STREAM_BUFFER_SIZE_UNFIXED
		#if 1
		pStreamBuf = SPDIF_DTS_DMP_STREAM_START;
		size = SPDIF_DTS_DMP_STREAM_END - SPDIF_DTS_DMP_STREAM_START;
		#else
		pStreamBuf = SPDIF_STREAM_START;
		size = SPDIF_STREAM_END - SPDIF_STREAM_START;
		#endif
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

		//AUDIO_FIFO_STREAM_CREATE(0, len, 0, 0);
	    fifo_create_file_stream(&audio_env.stream_fifo, 0, len, 0, 0);

   
		//app_media_data.playing_stream_status = STREAM_WAITING_DATA;
		app_media_data.playing_stream_status = STREAM_DEC_INIT;
		entry_id3_flag = ID3_UNKNOWN;					
		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
		
		{
			//usb device
			//DBG_Assert (FALSE);
			app_nav_lock_stream_mutex ();
			
			{	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				//uHddMsgSend (HDD_POWER_ON,0,0);
				uHddMsgSend (HDD_READ_FILE,0,0);
				app_media_mute_disable ();
			}
			
			app_nav_unlock_stream_mutex ();
			//app_nav_sleep(100);
			app_nav_sleep(1000);
		}

		uDecSend_Set(DECODE_SET, DECODE_DTSDMP);
		
		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}
	#endif //DTSDMP_DEC_TEST	

#if (defined DOLBY_DDPLUS71_DEC_TEST && (defined DOLBY_DDPLUS71_DEC_ENABLE))
	else if (app_media_data.playing_stream_type == STREAM_EC3)
	{
		#ifdef STREAM_BUFFER_SIZE_UNFIXED
		#if 1
		pStreamBuf = SPDIF_DOLBY_EC3_STREAM_START;
		size = SPDIF_DOLBY_EC3_STREAM_END - SPDIF_DOLBY_EC3_STREAM_START;
		#else
		pStreamBuf = SPDIF_STREAM_START;
		size = SPDIF_STREAM_END - SPDIF_STREAM_START;
		#endif
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

		//AUDIO_FIFO_STREAM_CREATE(0, len, 0, 0);
	    fifo_create_file_stream(&audio_env.stream_fifo, 0, len, 0, 0);

   
		//app_media_data.playing_stream_status = STREAM_WAITING_DATA;
		app_media_data.playing_stream_status = STREAM_DEC_INIT;
		entry_id3_flag = ID3_UNKNOWN;					
		app_media_data.coming_stream_status = STREAM_MEDIA;
		app_media_data.coming_stream_type = STREAM_DATA_CDROM;
		
		{
			//usb device
			//DBG_Assert (FALSE);
			app_nav_lock_stream_mutex ();
			
			{	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				//uHddMsgSend (HDD_POWER_ON,0,0);
				uHddMsgSend (HDD_READ_FILE,0,0);
				app_media_mute_disable ();
			}
			
			app_nav_unlock_stream_mutex ();
			//app_nav_sleep(100);
			app_nav_sleep(1000);
		}				

		uDecSend_Set(DECODE_SET, DECODE_DDPLUS71);
		
		//app_media_data.ui_background = app_nav_mp3_process;
		usb_msc_process_id = USB_MSC_MP3_PROCESS;
	}
#endif //OLBY_DDPLUS71_DE_TEST	

	else
	{
		DBG_Printf ("Playing Error Stream Type\n\r");
		DBG_Assert (FALSE);
	}
	
	if ((len < 1024*8) )
	{
		if ( (skip_key_flag == enMP3_PLAY_FLAG) || (skip_key_flag == enMP3_SKIPF_FLAG) )
		{
			DBG_Puts("force to jump next track\n\r");
			uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_F, 0);
		}
		else if (skip_key_flag == enMP3_SKIPB_FLAG)
		{
			DBG_Puts("force to jump previous track\n\r");
			//ukMsgSend(app_nav_skipb_released);
			uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_B, 0);
		}

		skip_key_flag = enMP3_PLAY_FLAG;
	}
	
	app_lcd_disp_program_icon();
	app_lcd_disp_play(TRUE);

	//AMPLIFIER_MUTE_OFF;
#endif //defined MP3_DECODE_ENABLE || defined WMA_DECODE_ENABLE
}

void app_window_mp3_pause_skipf (void)
{
	U8 temp;

	app_lcd_cancel_roll_function();
	
	//temp = app_media_data.disp_info;
	app_media_data.disp_info = DISP_INFO_FILE_NAME;

	app_window_disp_track_init();
	app_nav_mp3_skipf();

	app_dac_mute_enable ();
	app_media_data.playback_state = PLAYING_MODE_PAUSE;
	app_media_data.status = enSTATE_PAUSE;
}

void app_window_mp3_pause_skipb (void)
{

	U8 temp;

	app_lcd_cancel_roll_function();

	//temp = app_media_data.disp_info;
	app_media_data.disp_info = DISP_INFO_FILE_NAME;

	app_window_disp_track_init();
	app_nav_mp3_skipb ();

	app_dac_mute_enable ();
	app_media_data.playback_state = PLAYING_MODE_PAUSE;
	app_media_data.status = enSTATE_PAUSE;
}

void app_nav_mp3_pause_skipf (void)
{
	if (app_media_data.fast_processing == 0)
	{
		#ifdef SKIPFAST_SEPARATED_FROM_SKIP
		if (rmt_keySkipType == enKEY_REMOTE_SKIPFAST)
		{
			rmt_keySkipType = enKEY_SKIP_UNKNOWN;
			return;
		}
		#endif
		
		app_window_mp3_pause_skipf();
	}
	else
	{
		app_nav_mp3_start_fast(enSKIP_PLUS);
	}
}

void app_nav_mp3_pause_skipb (void)
{
	if (app_media_data.fast_processing == 0)
	{
		#ifdef SKIPFAST_SEPARATED_FROM_SKIP
		if (temp == enKEY_REMOTE_SKIPFAST || temp == enKEY_REMOTE_SKIP)
		{
			return;
		}
		#endif
		
		app_window_mp3_pause_skipb();
	}
	else
	{
		app_nav_mp3_start_fast(enSKIP_MINUS);
	}
}

void app_nav_mp3_pause (void)
{
	app_dac_mute_enable ();
	app_media_data.playback_state = PLAYING_MODE_PAUSE;
	app_media_data.status = enSTATE_PAUSE;

	app_nav_window_set(&nav_mp3_pause_window, (app_window_data.window_index+1));
}

void app_nav_mp3_play (void)
{
	skip_key_flag = enMP3_PLAY_FLAG;

#ifdef WAV_DEC
	wav_jump_adjust = FALSE;
#endif

	app_nav_usb_mp3_play();
}

void app_nav_usb_mp3_play (void)
{
	STREAM_ID s;
	U16 i;

	#ifdef USB_MP3_PLAY_TEST
	app_media_data.playback_mode = REPEAT_ALL;
	#endif

	if (app_media_data.total_tracks == 0)
	{
		return;
	}

	app_window_disp_track_init();

	#ifdef MP3_MODULE
	if (app_media_data.playback_state == PLAYING_MODE_PLAY)
	{
		return;
	}
	#endif

	if (app_media_data.playback_state != PLAYING_MODE_STOP)
	{
		//current in play or pause mode
		if (app_media_data.playback_state == PLAYING_MODE_PLAY)
		{
			app_dac_mute_enable ();
			app_media_data.playback_state = PLAYING_MODE_PAUSE;
			app_media_data.status = enSTATE_PAUSE;
	

			app_nav_window_set(&nav_mp3_pause_window,app_window_data.window_index+1);
			//For diplay USB Pause 1second time.
			app_timer_window_time_set(1000);
		}
		else if (app_media_data.playback_state == PLAYING_MODE_PAUSE)
		{			
			app_media_data.playback_state = PLAYING_MODE_PLAY;
			app_media_data.status = enSTATE_PLAYING;
			if (app_media_data.playing_stream_status == STREAM_MEDIA)
			{
				//audio out enable, reset idle timer
				app_timer_idle_time_reset ();
				app_dac_mute_disable ();
			}
		}
		
		return;
	}
	else
	{
		app_lcd_disp_program_icon();
	}

	app_dac_mute_enable ();

	//audio out enable, reset idle timer
	app_timer_idle_time_reset ();

	random_set_base (random ());

#ifdef MP3_PROGRAM_ENABLE
	if (app_media_data.playlist == PLAYLIST_ON)
	{
		if ( (app_media_data.playback_mode == RANDOM_IN_DIR) ||
			(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) ||
			(app_media_data.playback_mode == REPEAT_DIR) ||
			(app_media_data.playback_mode == DIR_RANDOM) ||
			(app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) )
		{
			
			app_media_data.playback_mode = NORMAL_PLAY;
			//app_lcd_disp_mode ();
			app_lcd_disp_mode_icon();
		}
		
		app_list_create_mp3_program_list ();

		i = 0;

		if ( (app_media_data.playback_mode == RANDOM_ALL) || (app_media_data.playback_mode == RANDOM_REPEAT_ALL) ) {
			// i is the start of random num
			i = random_track ( playing_info.total_of_playlist);
			app_media_data.random_played_tracks = 0;
		}
		else if ( (app_media_data.resume == RESUME_ON) && (app_media_data.resume_playback == TRUE) ) {
			
			app_media_data.resume_playback = FALSE;

			if ( app_list_get_list_content(app_media_data.resume_playing_index) == app_media_data.resume_track ) {

				app_media_data.resume_playback_request = TRUE;
				i = app_media_data.resume_playing_index;
			}
			else {
				app_media_data.resume_playback_request = FALSE;
			}
		}
			
		app_nav_mp3_play_track (i);
	}
	else
#endif
	{
		app_list_create_nomral_playlist ();

		if ( (app_media_data.playback_mode == RANDOM_ALL) || (app_media_data.playback_mode == RANDOM_REPEAT_ALL) )
		{

		#ifdef RESUME_PLAYBACK_ENABLE
			if (app_media_data.resume_playback == TRUE)
			{
				app_media_data.resume_playback_request = TRUE;
				app_media_data.resume_playing_index = app_media_data.resume_track;
				
				app_nav_mp3_play_track (app_media_data.resume_playing_index);
				random_start (app_media_data.resume_playing_index);
			}
			else
		#endif
			{
				app_media_data.random_played_tracks = 0;
				app_media_data.playing_index = random_track (playing_info.total_of_playlist);
				app_nav_mp3_play_track (app_media_data.playing_index);
			}
		}
		else if ( (app_media_data.playback_mode == RANDOM_IN_DIR) || (app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) )
		{
			i = app_list_playlist_tracks_in_dir (0);
			app_media_data.random_played_tracks = 0;
			i = random_track (i);
			app_nav_mp3_play_track (i);		
		}
	#ifdef MULTI_FOLDER_LEVEL
		else if ( (app_media_data.playback_mode == DIR_RANDOM) || (app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) )
		{
			i = app_list_playlist_total_dirs ();
			app_media_data.random_played_tracks = 0;
			i = random_track (i);
			i = app_list_playlist_dir_start_index (i);
			app_nav_mp3_play_track (i);	
		}
	#endif
		else if ( (app_media_data.resume == RESUME_ON) && (app_media_data.resume_playback == TRUE) )
		{	
		#ifndef RESUME_PLAYBACK_ENABLE
			app_media_data.resume_playback = FALSE;
		#else
			app_media_data.resume_playing_index = app_media_data.resume_track;
		#endif
			
			if (app_list_get_list_content(app_media_data.resume_playing_index) == app_media_data.resume_track )
			{
				app_media_data.resume_playback_request = TRUE;
				app_nav_mp3_play_track (app_media_data.resume_playing_index);
			}
			else
			{
				app_media_data.resume_playback_request = FALSE;
				app_nav_mp3_play_track (0);			
			}
		}
		else
		{
			app_nav_mp3_play_track (0);
		}
	}
}

void app_nav_mp3_playback_over (void)
{
	DBG_Printf("%s\n\r",__func__);

	app_cmd_DecoderExit();

	app_media_data.playing_stream_status = STREAM_IDLE;
	app_media_data.resume_playback = FALSE;
	app_media_data.resume_playback_request = FALSE;
	app_media_data.playback_state = PLAYING_MODE_STOP;
	app_media_data.status = enSTATE_STOP;
	
	//flush sdram buf
	AUDIO_FIFO_STREAM_FLUSH();

	//random base
	random_set_base (random ());

#ifdef MP3_MODULE
	app_nav_mp3_stop_skipf ();		
	cEV_State = MSTATE_STOP;

#else
	app_nav_mp3_stop_2(FALSE);

	//set nav window
	//app_media_data.ui_background = app_nav_dummy_process;
	usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

	app_nav_window_set(&nav_mp3_stop_window,0);
	app_window_disp_track_init();
#endif


#ifdef RESUME_PLAYBACK_ENABLE
#if 0//def RESUME_SAVE_TO_EEPROM
	// if playing back over ,delete resume function
	app_save_resume_info_reset();
#endif
#endif
}

void app_nav_mp3_stop_2 (int con)
{	
	DBG_Printf("%s\n\r",__func__);

	app_media_data.pgm_edit = FALSE;
	app_media_data.PlayDiscNum = 0;
	app_id3_state_init();

#ifdef LCD_DISP_SIMPLE
	album_skipf_first_press = FALSE;
#endif

	app_cmd_DecoderExit();

	app_lcd_stream_type_icon (STREAM_UNKNOWN);

#ifndef RESUME_PLAYBACK_ENABLE
	app_media_data.playing_track = 0;
#endif
	app_media_data.playing_stream_status = STREAM_IDLE;
	app_media_data.resume_track = app_media_data.playing_track;
	app_media_data.resume_playing_index = app_media_data.playing_index;
	app_media_data.resume_playback = TRUE;
 #ifndef RESUME_PLAYBACK_ENABLE
	app_media_data.resume_playback_request = FALSE;
#endif

	app_media_data.resume_playback_time = 0;
 	app_media_data.resume_sample_rate = app_media_data.playing_stream_sample_rate;
#ifndef RESUME_PLAYBACK_ENABLE
	app_media_data.playback_state = PLAYING_MODE_STOP;
	app_media_data.playback_mode = NORMAL_PLAY;
	app_media_data.disp_info = DISP_INFO_FILE_NAME;
#endif
	if (app_media_data.playing_stream_total_time == 0)
	{
		//unknown the stream total playing time
		app_media_data.resume_playback_time = 0;
	}


#if 1//def RESUME_PLAYBACK_ENABLE
	if (app_media_data.playback_state == PLAYING_MODE_STOP)
#endif
	{
		DBG_Puts("stop 2\n\r");

		app_media_data.resume_playback = FALSE;
		app_media_data.resume_track = 0;

		app_media_data.disp_info = DISP_INFO_FILE_NAME;

		app_lcd_disp_mode_icon();
		app_lcd_stream_type_icon (STREAM_UNKNOWN);

		if (con)
		{
			app_media_data.playback_mode = NORMAL_PLAY;

			if (app_window_current_get() != (WINDOW *) &nav_mp3_stop_window)
			{
				app_nav_window_set(&nav_mp3_stop_window, 0);
			}
			if (app_media_data.playlist == PLAYLIST_ON)
			{
				//app_cmd_mp3_program_clear();// during program mode, need to display program clear 2 sec
			}
			else
			{
				app_timer_window_time_set(10);
			}
		}

		//flush sdram buf
		AUDIO_FIFO_STREAM_FLUSH();

		return;
	}
	else
	{
		app_media_data.resume_playback = TRUE;

		if (app_media_data.playing_stream_vbr)
		{
		}
		else
		{
#ifdef RESUME_SAVE_TO_EEPROM
			app_save_resume_info_data();
#endif
		}

		//DBG_Printf("foder:%d,track %d,time %d\n", app_media_data.fold, app_media_data.resume_track, app_media_data.resume_playback_time);
	}

	app_media_data.playback_state = PLAYING_MODE_STOP;
	app_media_data.status = enSTATE_STOP;

#ifdef MP3_MODULE
	app_nav_mp3_stop_skipf ();
	cEV_State = MSTATE_STOP;
	app_media_data.status = enSTATE_STOP;

#else
	//set nav window
	//app_media_data.ui_background = app_nav_dummy_process;
	usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

	app_lcd_disp_stop();

	app_nav_window_set(&nav_mp3_stop_window, 0);
#endif

	app_window_disp_track_init();
}

void app_nav_mp3_stop (void)
{
	app_nav_mp3_stop_2(TRUE);
}

/*hold stop for long time, power down the machine*/
void app_nav_mp3_stop_hold (void)
{
}

void app_nav_mp3_enter (void)
{
}

void app_nav_mp3_repeat (void)
{
	#if 1
	U8 next_mode;

	//invalid in the shuffle
	if ( (app_media_data.playback_mode == RANDOM_ALL) || (app_media_data.playback_mode == RANDOM_REPEAT_ALL) )
	{
		//invalid in the shuffle
		if (app_window_current_get() != (WINDOW *) &mp3_mode_window)
		{
			return;
		}
		else
		{
			return;
		}
	}

	if (app_window_current_get() == (WINDOW *) &mp3_mode_window)
	{
		switch (app_media_data.playback_mode)
		{
		case NORMAL_PLAY:
			if (app_media_data.playlist == PLAYLIST_ON)
			{
				next_mode = REPEAT_ALL;
			}
			else
			{
				next_mode = REPEAT_ONE;
			}
			break;

		case REPEAT_ONE:
#ifdef FOLDER_SWITCH
			if (app_media_data.fold_switch)
			{
				next_mode = REPEAT_DIR;
			}
			else
#endif
			{
				next_mode = REPEAT_ALL;
			}
			break;

		case REPEAT_DIR:
			next_mode = REPEAT_ALL;
			break;

		case RANDOM_ALL:
			app_media_data.playback_mode = RANDOM_REPEAT_ALL;
			break;

		case RANDOM_REPEAT_ALL:
			app_media_data.playback_mode = REPEAT_ONE;
			break;

		case REPEAT_ALL:
		default:
			next_mode= NORMAL_PLAY;
			break;
		}

		app_media_data.playback_mode = next_mode;
	}

JUMP_DISPLAY_1:

	if (app_window_data.app_window[app_window_data.window_index].window != (WINDOW *) &mp3_mode_window)
	{
		app_nav_window_set(&mp3_mode_window, (app_window_data.window_index+1));
	}
	else
	{
		app_timer_window_time_set (mp3_mode_window.window_time);

		app_window_current_get()->draw_region ();
	}

	#endif
}

void app_nav_mp3_shuffle (void)
{

	if(app_media_data.playlist == PLAYLIST_ON)
	{
		return;
	}


	if (app_window_current_get() == (WINDOW *) &mp3_mode_window)
	{
		if (app_media_data.playback_mode == RANDOM_REPEAT_ALL)
		{
			app_media_data.playback_mode = NORMAL_PLAY;
		}
		else if (app_media_data.playback_mode == NORMAL_PLAY)
		{
			app_media_data.playback_mode = RANDOM_REPEAT_ALL;
		}
		else
		{
			return;
		}
	}


	if (app_window_data.app_window[app_window_data.window_index].window != (WINDOW *) &mp3_mode_window)
	{
		app_nav_window_set(&mp3_mode_window, (app_window_data.window_index+1));
	}
	else
	{
		app_timer_window_time_set (mp3_mode_window.window_time);

		//app_media_data.window->draw_title ();
		app_media_data.window->draw_region ();
	}
	
}

void app_nav_mp3_mode (void)
{
#if 0
	switch (app_media_data.playback_mode)
	{
		case NORMAL_PLAY:
			app_media_data.playback_mode = REPEAT_ONE;
			break;
		case REPEAT_ONE:
			app_media_data.playback_mode = REPEAT_ALL;
			break;
		case REPEAT_ALL:
			app_media_data.playback_mode = RANDOM_ALL;
			break;
		case RANDOM_ALL:
			app_media_data.playback_mode = RANDOM_REPEAT_ALL;
			break;
		case RANDOM_REPEAT_ALL:
		default:
			app_media_data.playback_mode = NORMAL_PLAY;
			break;
	}


	if (app_window_data.app_window[app_window_data.window_index].window != (WINDOW *) &mp3_mode_window) {

		if (app_media_data.playback_mode == NORMAL_PLAY) {
			app_lcd_disp_mode ();
		}
		else {
			app_nav_window_set(&mp3_mode_window,app_window_data.window_index+1);
		}
	}
	else {
		if (app_media_data.playback_mode == NORMAL_PLAY) {
			app_lcd_disp_mode ();
			app_window_cancel ();
		}
		else {
			app_timer_window_time_set (mp3_mode_window.window_time);

			//app_media_data.window->draw_title ();
			app_media_data.window->draw_region ();
		}
	}	
#endif
}

void app_nav_mp3_esp (void)
{
}

void app_nav_mp3_program_hold (void)
{
}

void app_nav_mp3_program_add (void)
{
}

static U8 pre_id3_state;
void app_id3_state_init(void)
{
#ifdef DISP_FILE_NAME_IN_PLAYING
	pre_id3_state = DISP_INFO_FILE_NAME;
#else
	pre_id3_state = DISP_INFO_PLAYBACK_TIME;
#endif
}

void app_id3_state_set(int state)
{
	pre_id3_state = state;
}

int app_id3_state_get(void)
{
	return pre_id3_state;
}

void app_nav_mp3_skipf (void)
{
	skip_key_flag = enMP3_SKIPF_FLAG;
	app_nav_usb_mp3_skipf();
}

void app_nav_usb_mp3_skipf (void)
{
	//At mormal playing, not skip forward  from max track to min track
	if ( (app_media_data.playback_mode== NORMAL_PLAY) &&
		 (app_media_data.playing_index >= playing_info.total_of_playlist-1))
	{
		return;
	}

	app_timer_window_time_set(0);
	app_media_data.playback_state = PLAYING_MODE_PLAY;		//if pause, clear pause
	app_media_data.status = enSTATE_PLAYING;

	app_cmd_DecoderExit();
	
	app_media_data.disp_info = DISP_INFO_PLAYBACK_TIME;
	
	if ( (app_media_data.playback_mode == RANDOM_ALL) ||
		(app_media_data.playback_mode == RANDOM_REPEAT_ALL) )
	{
		if (app_media_data.random_played_tracks >= (playing_info.total_of_playlist-1) )
		{
			random_set_base (random ());
			app_media_data.random_played_tracks = -1;		//start from 0
		}

		uMsgSend (UI_CD_INTERFACE,UI_DECODE_SKIP_F,0);
	}
	else if ((app_media_data.playback_mode == RANDOM_IN_DIR) ||
		(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) )
	{
		U16 tracks, dir;

		dir = app_list_playlist_dir (app_media_data.playing_index);
		tracks = app_list_playlist_tracks_in_dir (dir);

		if (app_media_data.random_played_tracks >= (tracks-1) )
		{
			random_set_base (random ());
			app_media_data.random_played_tracks = -1;		//start from 0
		}

		uMsgSend (UI_CD_INTERFACE,UI_DECODE_SKIP_F,0);
	}
	else if ( (app_media_data.playback_mode == DIR_RANDOM) ||
		(app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) )
	{
#ifdef MULTI_FOLDER_LEVEL
		U16 playlist_dirs,i;

		i = app_media_data.playing_index;

		i++;
		if (i >= playing_info.total_of_playlist) {
			i = 0;
		}

		if ( app_list_playlist_dir (app_media_data.playing_index) != app_list_playlist_dir (i) )
		{
			playlist_dirs = app_list_playlist_total_dirs ();

			if ( app_media_data.random_played_tracks >= (playlist_dirs-1) )
			{
				app_media_data.random_played_tracks = -1;		//start from 0
				random_set_base (random ());
			}
		}

		uMsgSend (UI_CD_INTERFACE,UI_DECODE_SKIP_F,0);
#endif	
	
	}
	else
	{
		uMsgSend (UI_CD_INTERFACE,UI_DECODE_SKIP_F,0);
	}
}

void app_nav_mp3_skipf_down (void)
{
	app_window_disp_track_init();

#ifdef LCD_DISP_SIMPLE
	album_skipf_first_press = FALSE;
#endif
}

#ifdef FAST_SEARCH_ENABLE

void app_cmd_jmp(long pos)
{
	U8 *pStreamBuf;
	U32 size;
	U32 len;
	U32 index;

	app_nav_lock_stream_mutex ();//force the hdd to idle state
	app_nav_unlock_stream_mutex ();

	{
		AUDIO_FIFO_STREAM_FLUSH();

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

		len = pMediaRead->lenth;
		index = app_media_data.stream_index;
		fifo_create_file_stream(&audio_env.stream_fifo, pos, len, index, 0);

		app_nav_lock_stream_mutex();

		uHddMsgSend (HDD_READ_FILE, pos, 0);

		app_media_mute_disable();
		app_nav_unlock_stream_mutex ();


		HDD_READ_FILE_EVENT_CLR;
		HDD_READ_FILE_EVENT_GET;
	}
}

static void app_cmd_wma_reinit(void)
{
	//app_media_data.ui_background = app_nav_dummy_process;
	usb_msc_process_id = USB_MSC_DUMMY_PROCESS;
	
	uiDecDecodeFrameExit ();

	app_cmd_jmp(0);

	app_media_data.playing_stream_pcm_sample = (long) app_media_data.resume_sample_rate * (long) app_media_data.playing_time;


	app_media_data.playing_stream_status = STREAM_WMA_INIT;

	uDecSend_Set(DECODE_SET, DECODE_WMA);

	//app_media_data.ui_background = app_nav_mp3_process;
	usb_msc_process_id = USB_MSC_MP3_PROCESS;
}

void app_cmd_set_playtime (U32 time)
{
	app_media_data.playing_time = time;
	app_media_data.playing_stream_pcm_sample =  app_media_data.resume_sample_rate * time;
}

void app_cmd_mp3_jump (long pos)
{
	long start_addr;
	U8 *pStreamBuf;
	U32 size;
	U32 len;
	U32 index;

	#ifdef WMA_DECODE_ENABLE
	if(app_media_data.playing_stream_type == STREAM_WMA)
	{
		wma_skip_flag = 1;
		app_cmd_wma_reinit();
		return;
	}
	#endif

	DBG_Printf("pos: %d, offset: %d\n\r", pos, app_media_data.playing_stream_media_offset);
	pos += app_media_data.playing_stream_media_offset;

	app_media_data.playing_stream_offset = 0;

	//force the hdd to idle state
	app_media_mute_enable ();
	app_nav_lock_stream_mutex ();
	app_nav_unlock_stream_mutex ();

	app_media_data.playing_stream_status = STREAM_IDLE;

	//app_sdram_stream_flush ();
	AUDIO_FIFO_STREAM_FLUSH();

	app_media_data.stream_index = app_media_data.playing_index;

	app_media_data.playing_track = app_list_get_list_content(app_media_data.playing_index);
	app_media_data.stream_track = app_list_get_list_content(app_media_data.stream_index);

	#ifdef MP3_MODULE
	cEV_Display_Info_Changed = TRUE;
	cEV_Display_Info_Changed_Info = 0x07;
	#endif

	if ((app_media_data.playing_stream_type == STREAM_MP3) ||
		(app_media_data.playing_stream_type == STREAM_WMA) ||
		(app_media_data.playing_stream_type == STREAM_WAV) ||
		(app_media_data.playing_stream_type == STREAM_FLAC))
	{
		#ifdef WAV_DEC
		if (app_media_data.playing_stream_type == STREAM_WAV)
		{
			pos = app_dec_wave_adjust_pos (pos);
			pos = (pos/(2048*6)) * (2048*6);
			wav_jump_adjust = TRUE;
		}
		if (0)
		{
		}
		#endif

		#ifdef FLAC_DEC
		else if (app_media_data.playing_stream_type == STREAM_FLAC)
		{
			pos = (pos/2048) * 2048;

		}
		else
		#endif
		{
			pos = (pos/2048) * 2048;

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

			len = pMediaRead->lenth;
			index = app_media_data.stream_index;
			fifo_create_file_stream(&audio_env.stream_fifo, pos, len , index, 0);


			app_media_data.playing_stream_status = STREAM_WAITING_DATA;
			app_media_data.coming_stream_status = STREAM_MEDIA;
			app_media_data.coming_stream_type = STREAM_DATA_CDROM;


			DBG_Printf ("Jump Lock\n\r");
			app_media_mute_enable ();
			app_nav_lock_stream_mutex ();
			
			DBG_Assert (pMediaRead != NULL);
			
			uHddMsgSend (HDD_READ_FILE, pos, 0);

			app_media_mute_disable ();
			app_nav_unlock_stream_mutex ();

			
			DBG_Printf ("Jump UnLock\n\r");
		}
	}
	else
	{
		DBG_Assert (FALSE);
	}

	//app_media_data.ui_background = app_nav_mp3_process;
	usb_msc_process_id = USB_MSC_MP3_PROCESS;
}
#endif


void app_nav_mp3_skipb (void)
{
	skip_key_flag = enMP3_SKIPB_FLAG;
	app_nav_usb_mp3_skipb_1();
}

void app_nav_usb_mp3_skipb_1 (void)
{	
	//At mormal playing, not skip back from min track to max track
	if ( (app_media_data.playing_time < 1) && (app_media_data.playback_mode== NORMAL_PLAY) &&
		(app_media_data.playing_index == 0) )
	{
		return;
	}

	app_media_data.playback_state = PLAYING_MODE_PLAY;		//if pause, clear pause
	app_media_data.status = enSTATE_PLAYING;

	app_cmd_DecoderExit();

	app_media_data.disp_info = DISP_INFO_PLAYBACK_TIME;

	if (app_media_data.playing_time >= 1)
	{
		app_nav_mp3_play_track (app_media_data.playing_index);
	}
	else
	{
		app_media_data.playing_stream_status = STREAM_IDLE;
		AUDIO_FIFO_STREAM_FLUSH();

		if ( (app_media_data.playback_mode == RANDOM_ALL) ||
			(app_media_data.playback_mode == RANDOM_REPEAT_ALL) )
		{
			if (app_media_data.random_played_tracks >= (playing_info.total_of_playlist-1) )
			{
				random_set_base (random ());
				//start from 0
				app_media_data.random_played_tracks = -1;
			}

			uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_F, 0);
		}
		else if ((app_media_data.playback_mode == RANDOM_IN_DIR) ||
			(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) )
		{	
			U16 tracks, dir;
			
			dir = app_list_playlist_dir (app_media_data.playing_index);
			tracks = app_list_playlist_tracks_in_dir (dir);
		
			if (app_media_data.random_played_tracks >= (tracks-1) ) {
				random_set_base (random ());
				app_media_data.random_played_tracks = -1;		//start from 0
			}
			
			uMsgSend (UI_CD_INTERFACE,UI_DECODE_SKIP_F,0);
		}
		else if ( (app_media_data.playback_mode == DIR_RANDOM) ||
			(app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) )
		{
#ifdef MULTI_FOLDER_LEVEL			

			U16 i,playlist_dirs, tracks;
		
			i = app_media_data.playing_index;

			if (i == 0) {
				i = playing_info.total_of_playlist - 1;
			}
			else {
				i--;
			}

			if (app_list_playlist_dir (i) ==
				app_list_playlist_dir (app_media_data.playing_index) )
			{
				app_nav_mp3_play_track (i);													
			}
			else {
				//app_media_data.random_played_tracks = 0;
				//random_set_base (random ());
							
				playlist_dirs = app_list_playlist_total_dirs ();

				app_media_data.random_played_tracks++;
		
				if ( app_media_data.random_played_tracks >= playlist_dirs) {
		
					app_media_data.random_played_tracks = 0;		//start from 0
					random_set_base (random ());
				}

				i = random_track (playlist_dirs);

				tracks = app_list_playlist_tracks_in_dir (i);
				i = app_list_playlist_dir_start_index (i);
			
				app_nav_mp3_play_track (i + tracks -1);											
			}
#endif
		}
		else
		{
			if (app_media_data.playback_mode != REPEAT_ONE)
			{
				if (app_media_data.playing_index == 0)
				{
					app_media_data.playing_index = playing_info.total_of_playlist - 1;
				}
				else
				{
					app_media_data.playing_index--;
				}
			}

			app_nav_mp3_play_track (app_media_data.playing_index);
		}
	}	
}

void app_nav_mp3_skipb_down (void)
{
	app_window_disp_track_init();

#ifdef LCD_DISP_SIMPLE
	album_skipf_first_press = FALSE;
#endif
}

void app_nav_mp3_skipf_hold (void)
{
}
void app_nav_mp3_skipb_hold (void)
{
}


extern U16 wSpkDeltaVol; 
extern U16 wNtcDeltaVol;

void app_nav_spk_level_init (void)
{
#ifdef SPEAK_LEVEL_DETECT
	wSpkDeltaVol = 0;

	#ifdef AUDIO_CODEC_USED_VOL
	AUDIOdevice.Set_Volume(audio_vari.volume);
	#endif
	#ifdef AUDIO_AMP_USED_VOL
	for (i=0;i<AMP_DEV_COUNT_MAX;i++)
	{
		AudioAmpDev[i].Set_Volume(audio_vari.volume);
	}
	#endif

#if defined AUDIO_CODEC_USED_EQ
	AUDIOdevice.Set_Bass_Treble(app_media_data.bass, app_media_data.treble);
#elif defined AUDIO_AMP_USED_EQ
	audioAmp_Dev.Set_Bass_Treble(app_media_data.bass, app_media_data.treble);
#endif

#endif
}



#ifdef MP3_FOLDER_KEY_SUPPORT
//1= next, 0=prev
static void app_cmd_mp3_folder_skip(U8 next)
{
	U16 i; 
	U16	total_folder;
	U16 dir, index;
	U16 tracks;

	if (app_media_data.playlist == PLAYLIST_ON)
	{
		return;
	}

	if (app_media_data.playback_state == PLAYING_MODE_STOP)
	{
		return;
	}

	#ifndef SKIP_ENABLE_IN_REPEAT_ONE
	if (app_media_data.playback_mode == REPEAT_DIR)
	{
		return;
	}
	#endif

	#ifndef SKIP_ENABLE_IN_REPEAT_FOLDER
	if (app_media_data.playback_mode == REPEAT_ONE)
	{
		return;
	}
	#endif

	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch)
	{
		return;
	}
	#endif

	if ( (app_media_data.playback_mode == RANDOM_IN_DIR) ||
		(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) ||
		(app_media_data.playback_mode == REPEAT_DIR) ||
		(app_media_data.playback_mode == DIR_RANDOM) ||
		(app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) )
	{
		return;
	}

	app_media_data.disp_info = DISP_INFO_PLAYBACK_TIME;
	//app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;
	app_window_disp_album_init();

	//---------------------------- Normal
	#ifdef SKIP_ENABLE_IN_REPEAT_FOLDER
	if ( (app_media_data.playback_mode == NORMAL_PLAY) ||
		(app_media_data.playback_mode == REPEAT_ALL) ||
		(app_media_data.playback_mode == REPEAT_ONE) ||
		(app_media_data.playback_mode == REPEAT_DIR) )
	#else
	if ( (app_media_data.playback_mode == NORMAL_PLAY) ||
		(app_media_data.playback_mode == REPEAT_ALL) )
	#endif
	{
		#ifdef CD_ENABLE
		#ifdef CD_SERVO_ENABLE
		if (global_vari.audio_source == MEDIA_CD)
		{
			dir = app_cd_get_folder_of_entry(app_media_data.playing_index);
		}
		else
		#endif
		#endif
		{
			dir = app_file_get_folder_of_entry(app_media_data.playing_index);
		}

		total_folder = app_media_data.total_folders;

		if (next == enSKIP_PLUS)
		{
			//DBG_Printf ("Next Dir in Normal Play\n\r");

			if (dir >= (total_folder - 1))
			{
				dir = 0;
			}
			else
			{
				dir++;
			}
		}
		else
		{
			//DBG_Printf ("Prev Dir in Normal Play\n\r");

			if (dir)
			{
				dir--;
			}
			else
			{
				dir = total_folder - 1;
			}
		}

		#ifdef CD_ENABLE
		#ifdef CD_SERVO_ENABLE
		if (global_vari.audio_source == MEDIA_CD)
		{
			index = app_cd_get_first_entrie_of_folder(dir);
			app_nav_mp3_play_track(index);
		}
		else
		#endif
		#endif
		{
			index = app_file_get_first_entrie_of_folder(dir);
			app_nav_mp3_play_track(index);
		}
	}
	else if ( (app_media_data.playback_mode == RANDOM_ALL) ||
		(app_media_data.playback_mode == RANDOM_REPEAT_ALL) )
	{
		app_nav_mp3_skipf();
		return;
	}
	else if ( (app_media_data.playback_mode == RANDOM_IN_DIR) ||
		(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) )
	{
		total_folder = app_list_playlist_total_dirs ();
		dir = app_list_playlist_dir (app_media_data.playing_index);

		dir++;

		if (dir >= total_folder)
		{
			dir = 0;
		}

		index = app_list_playlist_dir_start_index (dir);
	
		tracks = app_list_playlist_tracks_in_dir (dir);

		i = random_track (tracks);

		app_nav_mp3_play_track (index + i);			
	}

	#ifdef MULTI_FOLDER_LEVEL		
	else if ( (app_media_data.playback_mode == DIR_RANDOM) ||
		(app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) )
	{
		dirs = app_list_playlist_total_dirs ();
		
		app_media_data.random_played_tracks++;
		
		if ( app_media_data.random_played_tracks >= dirs)
		{
			app_media_data.random_played_tracks = 0;		//start from 0
			//random_set_base (random ());
		}

		i = random_track (dirs);

		i = app_list_playlist_dir_start_index (i);

		app_nav_mp3_play_track (i);
	}
	#endif
	else
	{
		DBG_Assert(FALSE);
	}

	//app_window_disp_album_init();
}

void app_nav_mp3_folder_next (void)
{
	app_cmd_mp3_folder_skip(enSKIP_PLUS);
}

void app_nav_mp3_folder_prev (void)
{
	app_cmd_mp3_folder_skip(enSKIP_MINUS);
}
#endif //MP3_FOLDER_KEY_SUPPORT



void app_nav_mp3_resume (void)
{
}

//key 1,2,3...
void app_nav_mp3_track (i)
{

	if ( (i == 0) || (i > app_media_data.total_tracks) )
	{
		//DBG_Assert (FALSE);
		//app_window_current_get()->draw_title ();
		app_window_current_get()->draw_region ();
		return;
	}

	if (app_media_data.playlist == PLAYLIST_ON)
	{
		if (!app_media_data.pgm_edit)
		{
			//in program state, does not support num key
			return;
		}
	}


	{
		I16 n,entry;

		if (app_window_current_get() == (WINDOW *)& nav_mp3_pause_window)
		{
			app_window_cancel_no_draw();
		}

		app_list_create_nomral_playlist ();
		
		app_cmd_DecoderExit();

		app_media_data.playing_stream_status = STREAM_IDLE;

		/*flush stream buf*/	
		AUDIO_FIFO_STREAM_FLUSH();

		app_nav_mp3_play_track (i-1);		
	}	
}

void app_nav_mp3_digit_key(void)
{
}


void app_nav_mp3_digit_enter(void)
{
}

void app_nav_mp3_intro (void)
{
}

void app_nav_mp3_stop_skip_dummy(void)
{
}

void app_nav_mp3_stop_skipf (void)
{
#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (SkipKey == enKEY_REMOTE_SKIPFAST)
	{
		SkipKey = enKEY_SKIP_UNKNOWN;
		return;
	}
#endif

	app_window_disp_track_init();

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	if ( (app_media_data.playlist == PLAYLIST_ON) && (!app_media_data.pgm_edit) )
	{
		app_list_create_mp3_program_list ();
	}
	else
	{
		app_list_create_mp3_entry_list ();
	}

	if (app_window_current_get() != (WINDOW *) &mp3_entry_nav_window)
	{
		app_nav_window_set(&mp3_entry_nav_window, app_window_data.window_index+1);
	}
	else
	{
		app_timer_window_time_set (mp3_entry_nav_window.window_time);

		//app_media_data.window->draw_title ();
		app_window_current_get()->draw_region ();
	}

	app_media_data.status = enSTATE_BROWSE_TRACK;
}

void app_nav_mp3_stop_skipf_hold (void)
{
#ifdef MINI_HIFI_SYSTEM

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;


	if ( (app_media_data.playlist == PLAYLIST_ON) && (!app_media_data.pgm_edit) ) {
		app_list_create_mp3_program_list ();
	}
	else {
		app_list_create_mp3_entry_list ();
	}

	app_nav_window_set(&mp3_entry_nav_window,app_window_data.window_index+1);

	app_media_data.status = enSTATE_BROWSE_TRACK;
	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_window_time_set (0);
	while (keysInQueue == 0)
	{
		app_nav_sleep (250);
		app_window_nav_next_roll ();
	}

	app_timer_window_time_set (app_window_current_get()->window_time);

#else

	app_media_data.disp_info = DISP_INFO_FILE_NAME;

	app_list_create_mp3_entry_list ();
	app_nav_window_set(&mp3_entry_nav_window,app_window_data.window_index+1);

	app_timer_window_time_set (2000);

	//app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;
	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	while (1)
	{
		if (keysInQueue > 0)
		{
			app_timer_window_time_set (app_media_data.window->window_time);
			return;
		}

		app_nav_sleep (250); 
		app_window_nav_next_roll ();
		app_timer_window_time_set (2000);
	}
	
	app_media_data.disp_info = DISP_INFO_FILE_NAME;
#endif
}

void app_nav_mp3_stop_skipb (void)
{
#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (SkipKey == enKEY_REMOTE_SKIPFAST)
	{
		SkipKey = enKEY_SKIP_UNKNOWN;
		return;
	}
#endif

	app_window_disp_track_init();

#ifdef MINI_HIFI_SYSTEM

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;


	if ( (app_media_data.playlist == PLAYLIST_ON) && (!app_media_data.pgm_edit) ) {
		app_list_create_mp3_entry_list ();
	}
	else {
		app_list_create_mp3_entry_list ();
	}

	app_lcd_cancel_roll_function();

	app_window_data.window_index++;
	app_nav_window_set(&mp3_entry_nav_window,app_window_data.window_index);

//	app_window_data.app_window[app_window_data.window_index].list_index = dynamic_list_entry - 1;
	app_window_data.app_window[app_window_data.window_index].list_index = 0;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_entry_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_entry_nav_window;

	app_timer_window_time_set (app_window_current_get()->window_time);
		
	//app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;

#else

#ifdef PHILIPS_AZ2858
	app_media_data.disp_info = DISP_INFO_FILE_NAME;
#else
	app_media_data.disp_info = DISP_INFO_FILE_NAME;
#endif

#ifdef PHILIPS_AZ1830
	if (app_media_data.playlist == PLAYLIST_ON) {
		app_list_create_mp3_program_list ();
	}
	else {
		app_list_create_mp3_entry_list ();
	}
#else
	app_list_create_mp3_entry_list ();
#endif

	app_lcd_cancel_roll_function();

	app_nav_window_set(&mp3_entry_nav_window,app_window_data.window_index+1);

	app_window_data.app_window[app_window_data.window_index].list_index = app_media_data.total_tracks - 1;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_entry_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_entry_nav_window;

	app_timer_window_time_set (app_media_data.window->window_time);

	//app_media_data.window->draw_title ();
	app_media_data.window->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;
	app_lcd_set_roll_index(3);
#endif
}

void app_nav_mp3_stop_skipb_hold (void)
{
#ifdef MINI_HIFI_SYSTEM

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;


	if ( (app_media_data.playlist == PLAYLIST_ON) && (!app_media_data.pgm_edit) ) {
		app_list_create_mp3_entry_list ();
	}
	else {
		app_list_create_mp3_entry_list ();
	}

	app_list_create_mp3_entry_list ();

	app_lcd_cancel_roll_function();
	app_window_data.window_index++;

//	app_window_data.app_window[app_window_data.window_index].list_index = dynamic_list_entry - 1;
	app_window_data.app_window[app_window_data.window_index].list_index = 0;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_entry_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_entry_nav_window;

	app_timer_window_time_set (2000);
		
	//app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;
	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_window_time_set (0);
	while (keysInQueue == 0)
	{
		app_nav_sleep (250);
		app_window_nav_next_roll ();
	}

	app_timer_window_time_set (app_window_current_get()->window_time);

#else

	app_media_data.disp_info = DISP_INFO_FILE_NAME;

	app_list_create_mp3_entry_list ();

	app_lcd_cancel_roll_function();
	app_window_data.window_index++;
	app_nav_window_set(&mp3_entry_nav_window,app_window_data.window_index);
	app_window_data.app_window[app_window_data.window_index].list_index = app_media_data.total_tracks - 1;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	//app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_entry_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_entry_nav_window;

	app_timer_window_time_set (2000);
		
	//app_media_data.window->draw_title ();
	app_media_data.window->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;
	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	while (1)
	{
		if (keysInQueue > 0)
		{
			app_window_time_set (app_window_current_get()->window_time);
			return;
		}

		app_nav_sleep (250); 
		app_window_nav_prev_roll ();
		app_timer_window_time_set (2000);
	}

	app_media_data.disp_info = DISP_INFO_FILE_NAME;
#endif
}



#ifdef MP3_FOLDER_KEY_SUPPORT
void app_nav_mp3_stop_folder_next (void)
{
#ifdef MINI_HIFI_SYSTEM
//	if (app_media_data.playlist == PLAYLIST_ON)
	if ( (app_media_data.playlist == PLAYLIST_ON) && (!app_media_data.pgm_edit) )
	{
		return;
	}
	
	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch)
	{
		return;
	}
	#endif
	
	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_list_create_mp3_folder_list ();
	app_nav_window_set(&mp3_folder_nav_window,app_window_data.window_index+1);

	app_media_data.status = enSTATE_BROWSE_ALBUM;
	app_window_disp_album_init();

#else

	#ifdef PHILIPS_AZ2858
	app_media_data.disp_info = DISP_INFO_ALBUM;
	#else
	app_media_data.disp_info = DISP_INFO_ALBUM;
	#endif

	app_list_create_mp3_folder_list ();
	app_nav_window_set(&mp3_folder_nav_window,app_window_data.window_index+1);

	app_media_data.status = enSTATE_BROWSE_ALBUM;
	app_window_disp_album_init();
#endif
}

void app_nav_mp3_stop_folder_next_hold (void)
{
#ifdef MINI_HIFI_SYSTEM
	if ( (app_media_data.playlist == PLAYLIST_ON) && (!app_media_data.pgm_edit) ) {
		return;
	}

	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch)
	{
		return;
	}
	#endif

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_list_create_mp3_folder_list ();
	app_nav_window_set(&mp3_folder_nav_window,app_window_data.window_index+1);

	app_timer_window_time_set (2000);
		
	app_media_data.status = enSTATE_BROWSE_ALBUM;

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_window_time_set (0);
	while (keysInQueue == 0)
	{
		app_window_nav_next_roll ();
		app_nav_sleep (250); 
	}

	app_timer_window_time_set (app_window_current_get()->window_time);

	app_window_disp_album_init();

#else

	app_media_data.disp_info = DISP_INFO_ALBUM;
	app_list_create_mp3_folder_list ();
	app_nav_window_set(&mp3_folder_nav_window,app_window_data.window_index+1);

	app_timer_window_time_set (2000);
		
	app_media_data.status = enSTATE_BROWSE_ALBUM;
	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_window_time_set (0);
	while (keysInQueue == 0)
	{
		app_window_nav_next_roll ();
		app_nav_sleep (250); 
	}

	app_timer_window_time_set (app_window_current_get()->window_time);

	app_media_data.disp_info = DISP_INFO_ALBUM;
	app_window_disp_album_init();
#endif
}

void app_nav_mp3_stop_folder_prev (void)
{
#ifdef MINI_HIFI_SYSTEM
	if ( (app_media_data.playlist == PLAYLIST_ON) && (!app_media_data.pgm_edit) )
	{
		return;
	}

	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch)
	{
		return;
	}
	#endif

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_list_create_mp3_folder_list ();

	app_lcd_cancel_roll_function();
	app_window_data.window_index++;
	app_nav_window_set(&mp3_folder_nav_window,app_window_data.window_index);
	app_window_data.app_window[app_window_data.window_index].list_index = app_media_data.total_folders - 1;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	//app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_folder_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_folder_nav_window;

	app_timer_window_time_set (app_window_current_get()->window_time);
		
	//app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_ALBUM;
	app_window_disp_album_init();

#else

	#ifdef PHILIPS_AZ2858
	app_media_data.disp_info = DISP_INFO_ALBUM;
	#else
	app_media_data.disp_info = DISP_INFO_ALBUM;
	#endif

	app_list_create_mp3_folder_list ();

	app_lcd_cancel_roll_function();
	app_window_data.window_index++;
	app_nav_window_set(&mp3_folder_nav_window,app_window_data.window_index);
	app_window_data.app_window[app_window_data.window_index].list_index = app_media_data.total_folders - 1;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	//app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_folder_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_folder_nav_window;

	app_timer_window_time_set (app_window_current_get()->window_time);
		
	//app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_ALBUM;
	app_window_disp_album_init();
#endif
}

void app_nav_mp3_stop_folder_prev_hold (void)
{
#ifdef MINI_HIFI_SYSTEM
	if ( (app_media_data.playlist == PLAYLIST_ON) && (!app_media_data.pgm_edit) )
	{
		return;
	}

	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch)
	{
		return;
	}
	#endif

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_list_create_mp3_folder_list ();

	app_lcd_cancel_roll_function();
	app_window_data.window_index++;
	app_nav_window_set(&mp3_folder_nav_window,app_window_data.window_index);
	app_window_data.app_window[app_window_data.window_index].list_index = app_media_data.total_folders - 1;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	//app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_folder_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_folder_nav_window;

	app_timer_window_time_set (2000);
		
	//app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();
	app_media_data.status = enSTATE_BROWSE_TRACK;

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_window_time_set (0);
	while (keysInQueue == 0) {
		app_window_nav_prev_roll ();
		app_nav_sleep (250); 
	}

	app_timer_window_time_set (app_window_current_get()->window_time);

	app_window_disp_album_init();

#else

	app_media_data.disp_info = DISP_INFO_ALBUM;
	app_list_create_mp3_folder_list ();

	app_lcd_cancel_roll_function();
	app_window_data.window_index++;
	app_window_data.app_window[app_window_data.window_index].list_index = app_media_data.total_folders - 1;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	//app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_folder_nav_window;
	app_media_data.window = (WINDOW *)&mp3_folder_nav_window;

	app_timer_window_time_set (2000);
		
	//app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;
	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_window_time_set (0);
	while (keysInQueue == 0)
	{
		app_window_nav_prev_roll ();
		app_nav_sleep (250); 
	}

	app_timer_window_time_set (app_window_current_get()->window_time);

	app_media_data.disp_info = DISP_INFO_ALBUM;
	app_window_disp_album_init();
#endif
}
#endif //MP3_FOLDER_KEY_SUPPORT



/************************************************************/
/*				DISC  NAV FUCTION							*/
/************************************************************/

void app_nav_readdisc_enter (void)
{
}

void app_nav_readdisc_play (void)
{
}


void app_nav_media_err_timeout (void)
{

	DBG_Printf ("USB Fatal Timeout Err Enter\n\r");

	USB_PowerOff(TRUE);

	DBG_Printf ("USB Fatal Timerout Err Exit\n\r");

	USB_PowerOn();
}

void app_nav_media_fata_error (void)
{
#ifdef USB_HOST_ENABLE
	app_nav_sleep (500);

	DBG_Printf ("USB Fatal Error Enter\n\r");

	if (gwUsbDeviceReady == FALSE)
	{
		/*error from disconnect*/
		wFsReadErr = FALSE;
		return;
	}
	
	DBG_Printf ("USB Fatal Err Recover\n\r");

	USB_PowerOff(TRUE);

	DBG_Printf ("USB Fatal Err 1\n\r");

	USB_PowerOn();

	app_media_data.auto_play = AUTO_PLAY_ON;
#endif //USB_HOST_ENABLE
}



void app_nav_usb_power_on(void)
{

	if (global_vari.audio_source == SOURCE_USB_MSC)
	{
		USB_POWER_ON;

		USB_Port_Connect_Switch(USB_PORT_CONNETED_TO_USB);
	}
}

void app_nav_rmt_convert_usb(void)
{
	U8 wBuf2[7] = "USBAPP";
	U8 rBuf[7];
	U8 page_addr;
	U8 req_disc;
	U8 *p;

	DBG_Printf("%s\n\r", __func__);

	#ifdef SOURCE_COMBINED_WITH_PLAY
	if (global_vari.audio_source == SOURCE_USB_MSC)
	{
		if (app_media_data.total_tracks > 0)
		{
			if ( (app_media_data.playback_state == PLAYING_MODE_STOP) ||
				(app_media_data.playback_state == PLAYING_MODE_PLAY) )
			{
				app_nav_mp3_play();
			}
			else
			{
				app_window_mp3_pause_play();
			}

			return;
		}
	}
	#endif


	if (global_vari.audio_source != SOURCE_USB_MSC)
	{
		#ifdef ADC_RECORD
		if (app_media_data.record_status == TRUE)
		{
			app_nav_adc_record_end();
		}
		#endif


		if (app_nav_media_finish())
		{
			return;
		}


		app_key_lock(TRUE);

		app_timer_window_time_set(0);
		app_nav_window_set(&dummy_window, app_window_data.window_index);

		#ifdef AUTO_PLAY_ENABLE
		app_media_data.auto_play = AUTO_PLAY_ON;
		#endif

		global_vari.audio_source = SOURCE_USB_MSC;
		//app_media_data.ui_background = app_nav_dummy_process;
		usb_msc_process_id = USB_MSC_DUMMY_PROCESS;

		app_media_data.Mute = FALSE;
		app_media_data.playback_mode = NORMAL_PLAY;
		app_media_data.playing_stream_sample_rate = 44100;
		#if (defined RECORD_ENABLE || defined ADC_RECORD)
		app_media_data.record_status = FALSE;
		#endif

		app_cmd_clear_snooze_flag();

		#if 0
		app_lcd_disp_icon_clear_partial();
		app_lcd_disp_mode_icon();
		app_lcd_stream_type_icon(STREAM_UNKNOWN);
		app_lcd_disp_stop();
		app_lcd_disp_media();
		app_lcd_disp_reading();
		#endif

		#ifdef USBHOST_OPEN_TIMER_ENABLE
		//app_timer_usb_open_time_set(2000);
		//app_timer_usb_open_time_set(1000);
		app_timer_usb_open_time_set(3000);
		#else
		usbmsc_USBhostOpen();
		#endif

		#ifdef USBHOST_PWR_ON_TIMER_ENABLE
		USB_POWER_OFF;
		//app_timer_usb_pwr_on_time_set(2000);
		app_timer_usb_pwr_on_time_set(3000);

		//USB_Port_Connect_Switch(USB_PORT_CONNETED_TO_IDLE);
		#else
		USB_POWER_ON;

		USB_Port_Connect_Switch(USB_PORT_CONNETED_TO_USB);
		#endif

		#ifdef TUNER_MODULE
		#ifdef TN_FM_AM_PRESET_SEPARATE
		app_media_data.tuner_preset = 0;
		tuner_fm_preset = 0;
		#ifdef TUNER_AM
		tuner_am_preset = 0;
		#endif
		#else
		//app_media_data.tuner_preset = 0;
		#endif
		#endif

		#ifdef EEPROM
		app_save_system_data();
		#endif

		/*
		* p = (U8 *) STRING_USB_APP;
		* CPU_SoftReset_Flag_Setting((U8 *) p, FALSE);
		*/

		if (gwUsbDeviceReady)
		{
			app_nav_usb_device_connected();
		}
		else
		{
			app_window_readingcd_window_init();
			app_nav_window_set(&readingcd_window, 0);
		}


		#if defined AUDIO_CODEC_USED_CHANNEL
		AUDIOdevice.Set_Channel(SOURCE_USB_MSC);
		#elif defined AUDIO_AMP_USED_CHANNEL
		audioAmp_Dev.Set_Channel(SOURCE_USB_MSC);
		#endif

		#ifdef AUDIO_CODEC_USED_VOL
		AUDIOdevice.Set_Volume(audio_vari.volume);
		#endif
		#ifdef AUDIO_AMP_USED_VOL
		int i;
		for (i=0;i<AMP_DEV_COUNT_MAX;i++)
		{
			AudioAmpDev[i].Set_Volume(audio_vari.volume);
		}
		#endif

		#if defined AUDIO_CODEC_USED_BASS_AND_TREBLE
		AUDIOdevice.Set_Bass_Treble(app_media_data.bass,app_media_data.treble);
		#elif defined AUDIO_AMP_USED_BASS_AND_TREBLE
		audioAmp_Dev.Set_Bass_Treble(app_media_data.bass,app_media_data.treble);
		#endif

		#ifdef VOLUME_AUTO_PLUS
		if (timer_alarm_power_on)
		{
			//app_nav_volume_auto_plus_for_timer_alarm();
			//app_timer_vol_auto_plus_time_set(5000);
		}
		else
		{
			//don't need this in stop.
			if ( (audio_vari.volume > VOLUME_AUTO_PLUS_DEFAULT) && volume_need_auto_plus)
			{
				/*volume_need_auto_plus = FALSE;
				audio_vari.volume_bak = audio_vari.volume;
				audio_vari.volume = VOLUME_MIDDLE;
				app_nav_volume_auto_plus();
				app_timer_vol_auto_plus_time_set(5000);*/
			}
			else
			{
			}
		}
		#endif


		#ifdef SPDIF_OUTPUT_ENABLE
		spdif_output_set(TRUE);
		#endif

		//AMPLIFIER_MUTE_OFF;
		app_key_lock(FALSE);
	}

	DBG_Printf("rmt convert usb Exit\n");
}




void app_nav_rmt_convert_sd(void)
{
#ifdef SD_ENABLE
	//U8 wBuf2[7] = "USBAPP";
	//U8 rBuf[7];
	U8 page_addr;
	U8 req_disc;
	U8 *p;

	DBG_Printf("sd enter\n");

	if (global_vari.audio_source != MEDIA_SDMMC)
	{
		if (app_nav_media_finish()) return;

		AMPLIFIER_MUTE_ON;

		//app_timer_window_time_set(0);
		//app_window_dummy_window_set();

		misClockFreq_Set(app_media_data.sys_freq);

#ifdef BOOT_FROM_FLASH
#if 1//ndef SDRAM_ENABLE
		misSdram_clk_enable();
		//Sdram_Init();
#endif
#endif

		//SD_POWER_ON;

#ifdef PCM_RECEIVE_ENABLE
		app_dac_receive_pcm_enable(FALSE);
#endif

		app_media_data.Mute = FALSE;
		//app_media_data.eq = EQ_CLASSIC;

		//app_media_data.ui_background = app_nav_dummy_process;
		usb_msc_process_id = USB_MSC_DUMMY_PROCESS;
		//app_timer_window_time_set (0);

		global_vari.audio_source = MEDIA_SDMMC;
		app_media_data.Mute = FALSE;
		app_media_data.ui_background = app_nav_dummy_process;
		//app_cmd_clear_timer_convert_flag();
		app_cmd_clear_snooze_flag();

		//app_cmd_DecoderExit();
		//app_nav_spk_level_init ();

		//app_lcd_disp_icon_clear_all();
		////app_lcd_disp_icon_clear_partial();
		////app_lcd_disp_mode_icon();
		//app_lcd_stream_type_icon(STREAM_UNKNOWN);
		//app_lcd_disp_stop();
		//app_lcd_disp_media();
		//app_lcd_disp_clear_line2();
		//app_lcd_disp_reading();

		app_window_readingcd_window_init();
		app_nav_window_set(&readingcd_window, 0);

		//delayms(500);
		//USB_PowerOn();
		//app_nav_sleep(500);

#ifdef TUNER_MODULE
#ifdef TN_FM_AM_PRESET_SEPARATE
		app_media_data.tuner_preset = 0;
		tuner_fm_preset = 0;
#ifdef TUNER_AM
		tuner_am_preset = 0;
#endif
#else
		app_media_data.tuner_preset = 0;
#endif
#endif

		#ifdef EEPROM
		app_save_system_data();
		#endif

		p = (U8 *) STRING_SD_APP;
		//CPU_SoftReset_Flag_Setting((U8 *) p, FALSE);

		/*if (sdmmcReady && sd1_FileSysPresent)
		{
			app_nav_usb_device_connected();
		}
		else*/
		{
			if (SD_Card_Detect_Get() == enSD_PLUG_IN)
			//if (0)
			{
				uHddMsgSend (SDMMC_INIT, 0, 0);
			}
			else
			{
				//app_lcd_disp_media_source(MEDIA_SDMMC, FALSE);
				//for exitting the current window quickly
				app_window_data.app_window[app_window_data.window_index].win_cn = (USB_READING_COUNT_NAX - 4);
			}
		}

#ifdef AUTO_PLAY_ENABLE
		app_media_data.auto_play = AUTO_PLAY_ON;
#else
		//if (!timer_alarm_power_up_state)
		{
			app_media_data.auto_play = AUTO_PLAY_OFF;
		}
#endif

#if defined AUDIO_CODEC_USED_CHANNEL
		AUDIOdevice.Set_Channel(MEDIA_SDMMC);
#elif defined AUDIO_AMP_USED_CHANNEL
		audioAmp_Dev.Set_Channel(MEDIA_SDMMC);
#endif

		#ifdef AUDIO_CODEC_USED_VOL
		AUDIOdevice.Set_Volume(audio_vari.volume);
		#endif
		#ifdef AUDIO_AMP_USED_VOL
		for (i=0;i<AMP_DEV_COUNT_MAX;i++)
		{
			AudioAmpDev[i].Set_Volume(audio_vari.volume);
		}
		#endif
	}
#endif
}

void app_nav_rmt_convert_tape(void)
{
#ifdef TAPE_MODULE
	//U8 wBuf2[7] = "TAPAPP";
	//U8 rBuf[7];
	U8 page_addr;
	U8 *p;

#ifdef BLUETOOTH
	if ( (BluetoothProcess >= BLUETOOTH_PHONE_DIAL) && (BluetoothProcess <= BLUETOOTH_CALLER_TRANSFER_AUDIO) ) {
		return;
	}
#endif

#ifdef TAPE_MODULE
	if (TAPE_RECORD) {
		if (!adc_rec_err) {
			app_window_release_tape_window_init();
			return;
		}
	}
#endif

	AMPLIFIER_MUTE_ON;
	delayms(50);			//for mute amp

	app_cmd_DecoderExit();
	global_vari.audio_source = MEDIA_TAPE;
	app_media_data.ui_background = app_nav_dummy_process;
	//app_timer_window_time_set (0);

	SoftResetSavingParameter(FALSE);

	p = (U8 *) STRING_TAPE_APP;
	//CPU_SoftReset_Flag_Setting((U8 *) p, TRUE);
#endif
}

void USB_Stop(void)
{
	DBG_Printf ("%s\n\r", __func__);

}

//#define UNPLUG_USB_SLOW
void USB_PowerOff(U8 con)
{
	DBG_Printf ("%s\n\r", __func__);

#ifdef USB_HOST_ENABLE
	int i;

	#ifdef UNPLUG_USB_SLOW
	uHddMsgSend (USB_MSC_CLOSE, 0, 0);
	#endif

	gwUsbDeviceReady = FALSE;
	DBG_TPrintf ("USB Stop\n\r");

	DBG_TPrintf ("USB Stop Exit\n\r");
	usbmsc_USBhostClose();
	gwUsbDeviceReady = FALSE;
	app_nav_sleep(20);

	app_nav_sleep(10);
	delayms(10);
	
	USB_POWER_OFF;
	if (con)
	{
		app_nav_sleep(3000);
	}
	else
	{
		app_nav_sleep(500);
	}

	DBG_TPrintf ("USB Power Off\n\r");
#endif
}

void USB_PowerOn(void)
{
#ifdef USB_HOST_ENABLE
	gwUsbDeviceReady = FALSE;

	USB_POWER_ON;
	delayms(25);

	delayms(5);

	DBG_TPrintf ("USB Start\n\r");

	usbmsc_USBhostOpen();
	delayms(25);
	DBG_TPrintf ("USB Start Exit\n\r");
#endif
}

#ifdef FAST_SEARCH_ENABLE
static void app_nav_mp3_process_fast(int ff)
{
	U16 playing_time;
	U16 fast_total_sec; 

	DBG_Printf("%s\n\r",__func__ );
	


	app_timer_idle_time_reset ();

	//For fast, not display id3 info
	app_timer_window_time_set (0);
	app_timer_key_hold_set_timer (FAST_SEARCH_INTERVAL_MS);

	if (app_media_data.playing_stream_type == STREAM_WMA)
	{
		if ( (app_media_data.playing_stream_status != STREAM_MEDIA) && (app_media_data.playing_stream_status != STREAM_WMA_INIT) )
		{
			DBG_Puts("wma skip exit\n\r");
			return;
		}
	}
	else
	{
		if ( (app_media_data.playing_stream_status != STREAM_MEDIA) && (app_media_data.playing_stream_status != STREAM_WAITING_DATA) )
		{
			//wait init state finish
			DBG_Puts("mp3 skip exit\n\r");
			return;
		}
	}

	if (app_media_data.fast_processing == FALSE)//not in the fast search state
	{
		app_media_data.fast_processing = TRUE;                   //set FF flag
		app_media_data.fast_step_ms = FAST_SEARCH_STEP_MS;       //every step
		app_media_data.fast_total_ms = app_media_data.fast_step_ms;

		if (app_media_data.playing_stream_type == STREAM_WMA)
		{
			uiDecDecodeFrameExit ();
		}
		
		app_dac_mute_enable ();
		
		app_media_mute_enable(); //quit read file
	}

	fast_total_sec =  app_media_data.fast_total_ms/1000;

	#ifdef FLAC_DEC
	//saving played time 
	app_media_data.playing_time_bak = app_media_data.playing_time;
	#endif


	//-------------------forward
	if (ff == enSKIP_PLUS)
	{
		//DBG_Puts("skip4\n\r");

		#ifdef FLAC_DEC
		mp3_fast_dir_flag = enSKIP_PLUS;
		#endif

		playing_time = app_media_data.playing_time + fast_total_sec;
		if (playing_time < (app_media_data.playing_stream_total_time - 1) )
		{
			app_media_data.fast_total_ms += app_media_data.fast_step_ms;
			app_lcd_disp_mp3_fast_time(playing_time);
		}
		else
		{
			DBG_Puts("skipf fast end\n\r");

			app_timer_key_hold_set_timer (0);

			//for continuing skipf
			DBG_Puts("jump the next track\n\r");
			app_media_data.playing_time = 0;
			app_media_data.fast_total_ms = 0;
			app_media_data.fast_processing = FALSE;
			uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_F, 0);
		}
	}
	//-------------------backward
	else
	{
		//DBG_Puts("skip5\n\r");

		#ifdef FLAC_DEC
		mp3_fast_dir_flag = enSKIP_MINUS;
		#endif

		if (app_media_data.playing_time > fast_total_sec)
		{
			app_media_data.fast_total_ms += app_media_data.fast_step_ms;
			playing_time = app_media_data.playing_time - fast_total_sec;
		}
		else
		{
			DBG_Printf("skipb fast end\n\r");

			app_media_data.fast_total_ms = app_media_data.playing_time * 1000;
			playing_time = 0;

		}

		app_lcd_disp_mp3_fast_time(playing_time);
	}
}


//the function only add time and display it
//void app_nav_mp3_process_fast_forward(void)
U32 app_nav_mp3_process_fast_forward(U32 param)
{
	#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (SkipKey == enKEY_REMOTE_SKIP)
	{
		//SkipKey = enKEY_SKIP_UNKNOWN;
		return;
	}
	#endif

	if (app_media_data.playing_stream_vbr)
	{
		DBG_Puts("vbr no support\n\r");
		return 100;
	}

	app_nav_mp3_process_fast(enSKIP_PLUS);

	return 100;
}

//void app_nav_mp3_process_fast_backward(void)
U32 app_nav_mp3_process_fast_backward(U32 param)
{
	#ifdef SKIPFAST_SEPARATED_FROM_SKIP
	if (SkipKey == enKEY_REMOTE_SKIP)
	{
		//SkipKey = enKEY_SKIP_UNKNOWN;	return 100;
		return 100;
	}
	#endif

	if (app_media_data.playing_stream_vbr)
	{
		DBG_Puts("vbr no support\n\r");		
		return 100;
	}

	app_nav_mp3_process_fast(enSKIP_MINUS);
	return 100;
}


/*
	total_time - playing_time		fast_total_time
	---------------------	=   --------------
	file_len - cur_pos				delta_pos


	delta_pos = (file_len - cur_pos)*fast_total_time/(total_time - playing_time)
*/
static void app_nav_mp3_start_fast(int ff)
{	
	U32 file_len = AUDIO_FIFO_STREAM_STREAM_LEN();
	U32 cur_pos = AUDIO_FIFO_STREAM_FIFO_RLEN();
	U16 fast_total_time = app_media_data.fast_total_ms/1000;
	U32 delta_pos;

	DBG_Printf("%s\n\r", __func__);

	delta_pos = (file_len - cur_pos)*fast_total_time/(app_media_data.playing_stream_total_time - app_media_data.playing_time);
	
	app_timer_idle_time_reset ();

	if (app_media_data.playing_stream_type == STREAM_WMA)
	{
		if ( (app_media_data.playing_stream_status != STREAM_MEDIA) && (app_media_data.playing_stream_status != STREAM_WMA_INIT) )
		{
			return;
		}
	}
	else
	{
		if ( (app_media_data.playing_stream_status != STREAM_MEDIA) && (app_media_data.playing_stream_status != STREAM_WAITING_DATA) )
		{
			//wait init state finish
			return;
		}
	}

	if (ff == enSKIP_PLUS)
	{

		if ( (app_media_data.playing_time + fast_total_time >= app_media_data.playing_stream_total_time - 1) )
		{
			#ifdef PLAYBACK_CIRCLES_DISABLE
			//At mormal playing, don't skip forward  from max track to min track
			if (app_media_data.playing_index < (app_media_data.total_tracks-1) )
			#endif
			{
				app_media_data.playing_time = 0;
				app_media_data.fast_total_ms = 0;

				#ifdef WMA_DECODE_ENABLE
				wma_skip_flag = 0;
				#endif

				#ifdef FLAC_DEC
				flac_dec_skip_flag = FALSE;
				#endif
			}

			uMsgSend (UI_CD_INTERFACE, UI_DECODE_SKIP_F, 0);
		}
		else
		{
			DBG_Printf("cur time = %d, jmp time = %d\n\r",app_media_data.playing_time,fast_total_time);
			app_media_data.playing_time += fast_total_time;
			cur_pos = app_media_data.playing_time * app_media_data.playing_stream_bitrate + app_media_data.playing_stream_media_offset;
			app_cmd_mp3_jump (cur_pos);
		}
	}
	else
	{

		if ( (fast_total_time >= app_media_data.playing_time -1) )
		{
			app_media_data.playing_time = 0;
			//app_media_data.fast_total_ms = 0;//for wma not seek
			app_cmd_mp3_jump (app_media_data.playing_stream_media_offset);
		}
		else
		{
			app_media_data.playing_time -= fast_total_time;
			cur_pos = app_media_data.playing_time * app_media_data.playing_stream_bitrate + app_media_data.playing_stream_media_offset;
			app_cmd_mp3_jump (cur_pos);
		}
	}

	app_media_data.playing_stream_pcm_sample = (long) app_media_data.playing_stream_sample_rate * app_media_data.playing_time;
	app_media_data.fast_processing = 0;

	app_lcd_disp_mp3_fast_time(app_media_data.playing_time);
	//After ff end, id3 display will display come back
	app_timer_window_time_set (app_media_data.window->window_time);
}


//the function jump to the pos 
void app_nav_mp3_start_fast_forward(void)
{
	app_nav_mp3_start_fast(enSKIP_PLUS);
}

void app_nav_mp3_start_fast_backward(void)
{
	app_nav_mp3_start_fast(enSKIP_MINUS);
}

void app_nav_skipf_released(void)
{
	DBG_Printf("skipf release\n");

	if (app_media_data.fast_processing == 0)
	{
#ifdef SKIPFAST_SEPARATED_FROM_SKIP
		if (SkipKey == enKEY_REMOTE_SKIPFAST)
		{
			SkipKey = enKEY_SKIP_UNKNOWN;
			return;
		}
#endif
		app_nav_mp3_skipf();
	}
	else
	{
		app_nav_mp3_start_fast(enSKIP_PLUS);
	}
}

void app_nav_skipb_released(void)
{
	DBG_Printf("skipb release\n");

	if (app_media_data.fast_processing == 0)
	{
#ifdef SKIPFAST_SEPARATED_FROM_SKIP
		if (SkipKey == enKEY_REMOTE_SKIPFAST)
		{
			SkipKey = enKEY_SKIP_UNKNOWN;
			return;
		}
#endif
		app_nav_mp3_skipb();
	}
	else
	{
		app_nav_mp3_start_fast(enSKIP_MINUS);
	}
}

#endif

#if 1//def FOR_ESD_PROTECT
void app_nav_save_pre_playtrack_info(void)
{
	resume_trk_info.playback_state = app_media_data.playback_state;
	resume_trk_info.resume_playback_time = app_media_data.playing_time;
	resume_trk_info.resume_playing_index = app_media_data.playing_index;
	resume_trk_info.resume_track = app_media_data.playing_track;
	resume_trk_info.resume_pos = (pMediaRead == NULL) ? 0 : pMediaRead->pos;
	
	 app_media_data.resume_sample_rate = app_media_data.playing_stream_sample_rate;
}
#endif

//4502(Full speed)
void USB_Port_Connect_Switch(U8 con)
{

}


//FSUSB30(High speed)
void USB_Port_High_Speed_Connect_Switch(U8 con)
{

}


void app_nav_auto_play_flag_set(U8 con)
{
	if (con)
	{
		app_media_data.auto_play = AUTO_PLAY_ON;
	}
	else
	{
		app_media_data.auto_play = AUTO_PLAY_OFF;
	}
}

void app_nav_set_volume (void)
{
	#ifdef AUDIO_CODEC_USED_VOL
	AUDIOdevice.Set_Volume(audio_vari.volume);
	#endif
	#ifdef AUDIO_AMP_USED_VOL
	int i;
	for (i=0;i<AMP_DEV_COUNT_MAX;i++)
	{
		AudioAmpDev[i].Set_Volume(audio_vari.volume);
	}
	#endif
}


void otk_psm_conn_ready (void);



U16 app_cmd_get_dec_freq(void)
{
	return app_media_data.playing_stream_sample_rate;
}


void app_window_disp_track_init(void)
{
#if 1	
	disp_folder_num = 0;
	flash_ticks = 0;
	pause_ticks = 0;
#endif	
}

void app_window_disp_album_init(void)
{
#if 0
	disp_folder_num = 1;
	flash_ticks = 0;
	pause_ticks = 0;
#endif	
}

/*********************************************************/
/*******************  tips window   **********************/
/*********************************************************/

const APP_NAV_KEY APP_NAV_NULL_KEY [] =
{
	{APP_KEY_SOURCE,NULL,NULL,0,NULL},
	{APP_KEY_PLAY,NULL,NULL,0,NULL},
	{APP_KEY_STOP,NULL,NULL,0,NULL},
};

const WINDOW nav_null_window =
{
	"nav_null_window",
	app_window_draw_region_none,         //draw region
	KEY_TABLE(APP_NAV_NULL_KEY),
	0,                                   //window time:500ms
	NULL,                                //window time handler
	WINDOW_RESERVES
};

/***********************************************
*******         Dummy window SET        ********
***********************************************/

const APP_NAV_KEY APP_NAV_DUMMY_KEY[] = 
{
	{APP_KEY_SOURCE,NULL,NULL,0,NULL},
	{APP_KEY_PLAY,NULL,NULL,0,NULL},
	{APP_KEY_STOP,NULL,NULL,0,NULL},
};

void app_dummy_window_timer(void)
{
	app_window_time_start ();

    U16 dummy_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (dummy_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif
}

const WINDOW dummy_window =
{                                   
	"dummy_window",
	app_window_draw_region_none,         //draw region
	KEY_TABLE(APP_NAV_DUMMY_KEY),
	1000,                                //window time:500ms
	app_dummy_window_timer,              //window time handler
	WINDOW_RESERVES
};



/*********************************************************/
/*****************  readingcd window   *******************/
/*********************************************************/

void app_window_readingcd_window_set(void)
{
	app_nav_window_set(&readingcd_window, 0);
}

const APP_NAV_KEY APP_NAV_READINGCD_KEY[] = 
{
    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

    {APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

	{APP_KEY_PLAY,NULL,NULL,0,NULL},
	{APP_KEY_STOP,NULL,NULL,0,NULL},
};

void app_readingcd_window_timer(void)
{
	app_window_time_start ();

    U16 readingcd_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (readingcd_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

    #ifdef DISPLAY_MODULE_ENABLE
	app_display_module.disp_usb_status(dispUSB_READING);
    #endif
}

void app_window_readingcd_display(void)
{
    printf_info("enter readingcd window\n\r");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_source(global_vari.audio_source);
    app_display_module.disp_usb_status(dispUSB_READING);
    #endif

}

const WINDOW readingcd_window =
{   
	"readingcd window",
	app_window_readingcd_display,         	//draw region
	KEY_TABLE(APP_NAV_READINGCD_KEY),
	1000,                                	//window time(ms)
	app_readingcd_window_timer,          	//window time handler
	WINDOW_RESERVES
};

void app_window_readingcd_window_init (void)
{
}

void app_nav_init_draw_playback_time(void)
{
	app_media_data.disp_info = DISP_INFO_PLAYBACK_TIME;
	LcdDisp_Roll = FALSE;

	app_lcd_cancel_roll_function();
	app_lcd_disp_mp3_playback_time();
	app_timer_window_time_set(1000);
}


/***********************************************
*******       mp3 play window SET       ********
***********************************************/

const APP_NAV_KEY APP_NAV_KEY_MP3[] = 
{
    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

    {APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

	{APP_KEY_PLAY,app_nav_mp3_play,NULL,0,NULL}, //play
	
	#if 1
	{APP_KEY_STOP,app_nav_mp3_stop,NULL,0,NULL},  //stop
	#else
	{APP_KEY_STOP,app_prompt_tone_2_play,NULL,0,NULL}, //stop
	#endif

	#ifdef FAST_SEARCH_ENABLE
	{APP_KEY_SKIPF,NULL,app_nav_skipf_released,800,app_nav_mp3_process_fast_forward}, //skipf
	{APP_KEY_SKIPB,NULL,app_nav_skipb_released,800,app_nav_mp3_process_fast_backward},//skipf
	#else
	{APP_KEY_SKIPF,app_nav_mp3_skipf,NULL,0,NULL}, //skipf
	{APP_KEY_SKIPB,app_nav_mp3_skipb,NULL,0,NULL}, //skipb
	#endif

//	{APP_KEY_PLUS,app_nav_mp3_plus,NULL,800,app_nav_mp3_plus_hold},
//	{APP_KEY_MINUS,app_nav_mp3_minus,NULL,800,app_nav_mp3_minus_hold},

	#ifdef MP3_FOLDER_KEY_SUPPORT
	{app_nav_mp3_folder_next,NULL,0,NULL}, //folder +
	{app_nav_mp3_folder_prev,NULL,0,NULL}, //folder -
	#endif

};

void app_nav_mp3_play_window_init(void)
{
	app_media_data.disp_info = DISP_INFO_PLAYBACK_TIME;
	LcdDisp_Roll = FALSE;

	app_nav_window_set(&nav_mp3_window, 0);
}

void app_mp3_play_window_timer(void)
{	
	app_window_time_start ();

    U16 mp3_play_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (mp3_play_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

	app_timer_idle_time_reset();

	if (app_window_current_get() != (WINDOW *) &nav_mp3_window)
	{
		return;
	}
    
    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_usb_status(dispUSB_PLAYING);

	#ifdef LCD_DISP_MUTE_ALWAYS
	if (app_media_data.Mute)
	{
		app_lcd_disp_mute();
		return;
	}
	#endif

    #endif

	#ifdef TIMER_ALARM_ENABLE
	if (!timer_alarm_power_on)
	{
		app_lcd_device.display_icon(ICON_TIMER1, timer_alarm[0].valid);
		app_lcd_device.display_icon(ICON_TIMER2, timer_alarm[1].valid);
	}
	#endif
	#ifdef FAST_SEARCH_ENABLE
	if (app_media_data.fast_processing)
	{
		return;
	}
	#endif
}

const WINDOW nav_mp3_window =
{                                     
	"mp3 window",
	app_window_draw_region_none,      //draw region
	KEY_TABLE(APP_NAV_KEY_MP3),
	500,                              //window time(ms)
	app_mp3_play_window_timer,        //window time handler
	WINDOW_RESERVES
};


/***********************************************
*******       mp3 mode window SET       ********
***********************************************/

void app_mp3_mode_window_timer(void)
{
	app_window_time_start ();

    U16 mp3_mode_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (mp3_mode_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif
}
	
const APP_NAV_KEY APP_NAV_MP3_MODE_KEY[] = 
{
//	{APP_KEY_SOURCE,NULL,app_nav_source_released,0,NULL},
//	{APP_KEY_POWER,NULL,app_nav_standby,0,NULL},
};

const WINDOW mp3_mode_window =
{                                    
	"mp3 mode window",
	app_window_draw_region_none,             	//draw region
	KEY_TABLE(APP_NAV_MP3_MODE_KEY),
	1000,                                   	//window time(ms)
	app_mp3_mode_window_timer,              	//window time handler
	WINDOW_RESERVES
};

/*********************************************************************************
 *						Protect window								    	*
 ********************************************************************************/

extern U8 skip_key_flag;

void app_window_mp3_protect_timer(void)
{
	static U8 mp3_protect_window_timer_count = 0;
	
	if (app_media_data.window != (WINDOW *)&nav_mp3_prot_window)
	{
		return;
	}

	mp3_protect_window_timer_count++;
	app_timer_window_time_set (app_media_data.window->window_time);

	if (mp3_protect_window_timer_count == 1)
	{
	}
	else if (mp3_protect_window_timer_count <= 2)
	{
		app_lcd_disp_protect();
	}
	else
	{
		//app_window_cancel();

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
		mp3_protect_window_timer_count = 0;
		app_window_cancel();
	}
}

const APP_NAV_KEY APP_NAV_MP3_PROT_KEY[] = 
{
//	{APP_KEY_SOURCE,NULL,app_nav_source_released,0,NULL},
//	{APP_KEY_POWER,NULL,app_nav_standby,0,NULL},
};

const WINDOW nav_mp3_prot_window =
{                                   
	"mp3 prot window",
	app_window_draw_region_none,         //draw region
	KEY_TABLE(APP_NAV_MP3_PROT_KEY),
	0,                                   //window time(ms)
	NULL,                                //window time handler
	WINDOW_RESERVES
};

/***********************************************
*******       mp3 stop window SET       ********
***********************************************/

const APP_NAV_KEY APP_NAV_KEY_MP3_STOP [] = {

    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

    {APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

	{APP_KEY_PLAY,app_nav_mp3_play,NULL,0,NULL},          //paly

//	{APP_KEY_SKIPF,app_nav_mp3_stop_skip_dummy,app_nav_mp3_stop_skipf,800,app_nav_mp3_stop_skipf_hold},//skip f
//	{APP_KEY_SKIPB,app_nav_mp3_stop_skip_dummy,app_nav_mp3_stop_skipb,800,app_nav_mp3_stop_skipb_hold},//skip b
	{APP_KEY_SKIPF,app_nav_mp3_stop_skipf,NULL,800,app_window_nav_next_hold_roll}, //skip f
	{APP_KEY_SKIPB,app_nav_mp3_stop_skipb,NULL,800,app_window_nav_prev_hold_roll}, //skip b

	#ifdef MP3_FOLDER_KEY_SUPPORT
	{APP_KEY_FOLDER_F,app_nav_mp3_stop_folder_next,NULL,800,app_nav_mp3_stop_folder_next_hold}, //Album +
	{APP_KEY_FOLDER_B,app_nav_mp3_stop_folder_prev,NULL,800,app_nav_mp3_stop_folder_prev_hold}, //Album -
	#endif

	#ifdef SHUFFLE_COMBINE_WITH_REPEAT
	{APP_KEY_REPEAT,app_nav_mp3_repeat,1500,app_nav_mp3_shuffle},   //shuffle
	#else
	{APP_KEY_REPEAT,app_nav_mp3_repeat,NULL,0,NULL},                //repeat
	{APP_KEY_SHUFFLE,app_nav_mp3_shuffle,NULL,0,NULL},              //shuffle
	#endif

	#ifdef FILE_DELETE_ENABLE
	//for the test of deleting mp3 files.
	{APP_KEY_EQ,app_nav_mp3_stop_delete,NULL,0,0}, //EQ
	#endif
	
};

void app_mp3_stop_window_timer(void)
{
	app_window_time_start ();

    U16 mp3_stop_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (mp3_stop_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

	#ifdef DISPLAY_MODULE_ENABLE
	app_display_module.disp_usb_status(dispUSB_STOP);
    #endif
}

const WINDOW nav_mp3_stop_window =
{                                   
	"mp3 stop window",
	app_window_draw_region_none,      		//draw region
	KEY_TABLE(APP_NAV_KEY_MP3_STOP),
	1000,                                	//window time(ms)
	app_mp3_stop_window_timer,    			//window time handler
	WINDOW_RESERVES
};

/***********************************************
*******       no song window SET        ********
***********************************************/

void app_no_songs_window_timer(void)
{
	app_window_time_start ();

    U16 no_songs_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (no_songs_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

	app_timer_idle_time_reset();
}

	
const WINDOW nav_no_songs =
{                                    
	"no songs window",
	app_window_draw_region_none,         	//draw region
	KEY_TABLE(APP_NAV_READINGCD_KEY),
	1000,                                 	//window time(ms)
	app_no_songs_window_timer,           	//window time handler
	WINDOW_RESERVES
};


void app_window_mp3_pause_play (void)
{
	U16 time,fast_playing_time,fast_phase;
	long pos;
	STREAM_ID s;
	long start_addr;

	{	
		app_media_data.playback_state = PLAYING_MODE_PLAY;
		app_media_data.status = enSTATE_PLAYING;

		if (app_media_data.playing_stream_status == STREAM_MEDIA)
		{
			app_dac_mute_disable ();
		}

		app_timer_idle_time_reset ();			//reset idle time

		#ifdef DISP_SOURCE_PLAY_PAUSE
		app_lcd_disp_playing_status();
		#endif

		app_window_cancel ();

		app_window_disp_track_init();
	}

	app_lcd_disp_play(TRUE);
}

/***********************************************
*******       mp3 pause window SET      ********
***********************************************/

void app_mp3_pause_window_timer(void)
{
	app_window_time_start ();

    U16 mp3_pause_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (mp3_pause_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

	#ifdef DISPLAY_MODULE_ENABLE
	app_display_module.disp_usb_status(dispUSB_PAUSE);
    #endif

}

const APP_NAV_KEY APP_NAV_KEY_MP3_PAUSE [] = {

	{APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

    {APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

	{APP_KEY_PLAY,app_window_mp3_pause_play,NULL,0,NULL},										//paly
	{APP_KEY_STOP,app_nav_mp3_stop,NULL,0,NULL},
	
	#if 1
	{APP_KEY_SKIPF,NULL,app_window_mp3_pause_skipf,0,NULL},	//skip f
	{APP_KEY_SKIPB,NULL,app_window_mp3_pause_skipb,0,NULL},	//skip b
	#else
	{APP_KEY_SKIPF,NULL,app_nav_mp3_pause_skipf,600,app_nav_mp3_process_fast_forward},  //skip f
	{APP_KEY_SKIPB,NULL,app_nav_mp3_pause_skipb,600,app_nav_mp3_process_fast_backward}, //skip b
	#endif
};

const WINDOW nav_mp3_pause_window =
{                                    
	"mp3 pause window",
	app_window_draw_region_none,          	//draw region
	KEY_TABLE(APP_NAV_KEY_MP3_PAUSE),
	1000,                                 	//window time(ms)
	app_mp3_pause_window_timer,       		//window time handler
	WINDOW_RESERVES
};

/***********************************************
*******       mp3 entry window SET      ********
***********************************************/

const APP_NAV_KEY APP_NAV_MP3_ENTRY_NAV_KEY [] =
{

    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

    {APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

	{APP_KEY_PLAY,app_window_mp3_cdfile_nav_play,NULL,0,NULL}, //play
	{APP_KEY_STOP,app_nav_mp3_stop,NULL,0,NULL},               //stop
	
	{APP_KEY_SKIPF,app_window_nav_next_roll,NULL,800,app_window_nav_next_hold_roll},  //skip f
	{APP_KEY_SKIPB,app_window_nav_prev_roll,NULL,800,app_window_nav_prev_hold_roll},  //skip b

	#ifdef MP3_FOLDER_KEY_SUPPORT
	{APP_KEY_FOLDER_F,app_window_mp3_entry_folder_next,NULL,800,app_window_mp3_entry_folder_next_hold}, //Ablum +
	{APP_KEY_FOLDER_B,app_window_mp3_entry_folder_prev,NULL,800,app_window_mp3_entry_folder_prev_hold}, //Ablum -
	#endif
};

void app_mp3_entry_window_timer(void)
{
	U16 temp;

	app_window_time_start ();

    U16 mp3_entry_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (mp3_entry_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

	#ifdef LCD_DISP_FILE_NAME_TRACK_SKIP_IN_STOP
	
	//if (app_media_data.disp_info != DISP_INFO_ENTRY_FOLDER_NUMBER)
	{
		temp = window_disp_state;
		switch (temp)
		{
		#ifdef LCD_DISP_FILE_NAME_TRACK_SKIP_IN_STOP
		case enDISP_TRACK_NAME_ROLLING_START:
			app_lcd_disp_mp3_Entry_FileName(LINE1, app_media_data.entry);
			app_lcd_disp_scroll_string((U8 *)song_information, LINE1, SCROLL_ONCE, 2);
			window_disp_state = enDISP_TRACK_NAME_ROLLING_END;
			break;

		case enDISP_TRACK_NAME_ROLLING_END:
			if (!LcdDisp_Roll)
			{
				app_lcd_disp_mp3_entry_number_and_file_name(app_media_data.entry, 0);
			}
			break;
		#endif

		#ifdef LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
		case enDISP_FOLDER_NAME_ROLLING_START:
			app_lcd_disp_mp3_Entry_FolderName(LINE1, app_media_data.entry);
			app_lcd_disp_scroll_string((U8 *)song_information, LINE1, SCROLL_ONCE, 2);
			window_disp_state = enDISP_FOLDER_NAME_ROLLING_END;
			break;

		case enDISP_FOLDER_NAME_ROLLING_END:
			if (!LcdDisp_Roll)
			{
				U16 track_index;
				U16 fold_index;
				#if 0
				app_lcd_disp_mp3_Entry_FolderName(LINE1, app_media_data.fold);
				#else
				if (global_vari.audio_source == MEDIA_CD)
				{
					fold_index = app_cd_get_folder_of_entry(app_media_data.entry);
				}
				else
				{
					fold_index = app_file_get_folder_of_entry(app_media_data.entry);
				}
		
				if (global_vari.audio_source == MEDIA_CD)
				{
					track_index = app_cd_get_first_entrie_of_folder(fold_index);
				}
				else
				{
					track_index = app_file_get_first_entrie_of_folder(fold_index);
				}
				app_lcd_disp_mp3_entry_number_and_file_name(track_index, 0);
				#endif
			}
			break;
		#endif
		}
	}

	#else

	if (mp3_entry_win_count > 10)
	{
		//10s timeout to TOC status
		app_nav_mp3_stop();
		return;
	}
	
	#endif
}

const WINDOW mp3_entry_nav_window =
{                                    
	"mp3 entry window",
	app_window_draw_region_none,          	//draw region
	KEY_TABLE(APP_NAV_MP3_ENTRY_NAV_KEY),
	1000,                                 	//window time(ms)
	app_mp3_entry_window_timer,       		//window time handler
	WINDOW_RESERVES
};



//#define MP3_FOLDER_KEY_SUPPORT

void app_window_mp3_cdfile_nav_play (void)
{
#if 1
	U16 m, i;
	U8 ret = FALSE;

	#ifdef REMOTE_PLAY_KEY_SWITCH_SOURCE_ENABLE
	if (app_nav_source_switch_play_flag_get())
	{
		return;
	}
	#endif

	m = app_window_data.window_index;
	i = app_window_data.app_window[m].list_index + app_window_data.app_window[m].list_cursor;
	
	app_lcd_cancel_roll_function();
	app_window_data.window_index = 0;
	app_window_data.app_window[0].window = (WINDOW *) &nav_null_window;
	app_media_data.window = (WINDOW *) &nav_null_window;

	#ifdef MP3_PROGRAM_ENABLE
	if (app_media_data.playlist == PLAYLIST_ON)
	{	
		if ( (app_media_data.playback_mode == RANDOM_IN_DIR) ||
			(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) ||
			(app_media_data.playback_mode == REPEAT_DIR) ||
			(app_media_data.playback_mode == DIR_RANDOM) ||
			(app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) )
		{
			app_media_data.playback_mode = NORMAL_PLAY;
			//app_lcd_disp_mode ();
			app_lcd_disp_mode_icon();
		}
		
		app_list_create_mp3_program_list ();

		for (m=0; m<playing_info.total_of_program_list; m++)
		{
			if (m == i) {
				break;
			}
		}

		if (m >= playing_info.total_of_program_list) {
			m = 0;
		}

		i = m;

		if ( (app_media_data.playback_mode == RANDOM_ALL) || (app_media_data.playback_mode == RANDOM_REPEAT_ALL) )
		{
			// i is the start of random num
			i = random_track ( playing_info.total_of_playlist );
			app_media_data.random_played_tracks = 0;
		}
			
		if (!app_media_data.pgm_edit) {
			app_nav_mp3_play_track (i);
		}
		else {
			app_nav_mp3_play_track (0);
		}
	}
	else
	#endif
	{
		app_list_create_mp3_nomral_playlist ();

		if (dynamic_list_attribute == ITEM_FOLDER)
		{
			#ifdef CD_ENABLE
			#ifdef CD_SERVO_ENABLE
			if (global_vari.audio_source == MEDIA_CD)
			{
				i = app_cd_get_first_entrie_of_folder (i);
			}
			else
			#endif
			#endif
			{
				i = app_file_get_first_entrie_of_folder (i);
			}

			if ( (U16)i != NO_ENTRY_IN_FOLDER )
			{
				if ( (app_media_data.playback_mode == RANDOM_ALL) ||
					(app_media_data.playback_mode == RANDOM_REPEAT_ALL) )
				{
					// i is the start of random num
					random_start (i);
					app_media_data.random_played_tracks = 0;
				}
				else if ( (app_media_data.playback_mode == RANDOM_IN_DIR) ||
					(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) )
				{
					#ifdef CD_ENABLE
					#ifdef CD_SERVO_ENABLE
					if (global_vari.audio_source == MEDIA_CD)
					{
						m = app_list_playlist_tracks_in_dir (app_cd_get_folder_of_entry(i) );
					}
					else
					#endif
					#endif
					{
						m = app_list_playlist_tracks_in_dir (app_file_get_folder_of_entry(i) );
					}
					
					app_media_data.random_played_tracks = 0;
					i += random_track (m);
				}
				#ifdef MULTI_FOLDER_LEVEL
				else if ( (app_media_data.playback_mode == DIR_RANDOM) ||
					(app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) )
				{
					m = app_list_playlist_dir (i);
					m = app_list_playlist_tracks_in_dir (m);
					i += random_track (m);
					app_media_data.random_played_tracks = 0;
				}
				#endif
			
				app_nav_mp3_play_track (i);
			}
			else
			{
				DBG_Assert (FALSE);
				app_nav_mp3_play_track (0);
			}
		}
		else if (dynamic_list_attribute == ITEM_ENTRY)
		{
			if ( (app_media_data.playback_mode == RANDOM_ALL) ||
				(app_media_data.playback_mode == RANDOM_REPEAT_ALL) )
			{
				i = random_track (app_media_data.total_tracks);
				app_media_data.random_played_tracks = 0;
				app_nav_mp3_play_track (i);
			}
			else if ( (app_media_data.playback_mode == RANDOM_IN_DIR) ||
				(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) )
			{


			U16 start_index, index, tracks, dir;

			dir = app_list_playlist_dir (i);
			tracks = app_list_playlist_tracks_in_dir (dir);
			start_index = app_list_playlist_dir_start_index (dir);

			DBG_Printf ("dir1 = %d\n\r", dir);
			DBG_Printf ("tracks1 = %d\n\r", tracks);
			DBG_Printf ("start_index1 = %d\n\r", start_index);
			DBG_Printf ("random_tracks1 = %d\n\r", app_media_data.random_played_tracks);
			DBG_Printf ("PLAY BACK MODE1 : %d\n\r", app_media_data.playback_mode);

			app_media_data.random_played_tracks = 0;

			index = random_track (tracks);
			DBG_Printf ("index = %d\n\r", index);

			ukParmSend(app_nav_mp3_play_track, start_index + index);
	
				
			}
			#ifdef MULTI_FOLDER_LEVEL
			else if ( (app_media_data.playback_mode == DIR_RANDOM) ||
				(app_media_data.playback_mode == DIR_RANDOM_REPEAT_ALL) )
			{			
				m = app_list_playlist_dir (i);
				random_start (m);
				app_media_data.random_played_tracks = 0;
				app_nav_mp3_play_track (i);
			}
			#endif
			else
			{
				app_nav_mp3_play_track (i);
			}
		}
		else
		{
			DBG_Assert (FALSE);
		}
	}

	app_media_data.pgm_edit = FALSE;
#endif
}



#ifdef MP3_FOLDER_KEY_SUPPORT
void app_window_mp3_folder_entry_skip_dummy(void)
{
}

//for pressing "skipf" key in mp3_folder_nav_window
void app_window_mp3_folder_entry_next (void)
{
#if 1//def MP3_FOLDER_KEY_SUPPORT
	I16 m,i,entry;

	if ( (app_media_data.playback_mode == RANDOM_ALL) ||
		(app_media_data.playback_mode == RANDOM_REPEAT_ALL) ||
		(app_media_data.playback_mode == RANDOM_IN_DIR) ||
		(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) )
	{
		return;
	}

	app_lcd_cancel_roll_function();
	if (app_media_data.disp_info == DISP_INFO_ENTRY_FOLDER_NUMBER)
	{
	}
	else
	{
		app_media_data.disp_info = DISP_INFO_FILE_NAME;
	}
	
	m = app_window_data.window_index;

	i = app_window_data.app_window[m].list_index + app_window_data.app_window[m].list_cursor;
			
	/*entry number*/
	//i = dynamic_mp3_list[i].index;
	i = app_list_get_list_content(i);

	#ifdef CD_ENABLE
	#ifdef CD_SERVO_ENABLE
	if (global_vari.audio_source == MEDIA_CD)
	{
		entry = app_cd_get_first_entrie_of_folder(i);
	}
	else
	#endif
	#endif
	{
		entry = app_file_get_first_entrie_of_folder(i);
	}
	
	//Get folder of second track or next folder of first track
	if (entry < (app_media_data.total_tracks-1))
	{
		entry++;
	}
	else
	{
		entry = 0;
	}

	app_window_data.window_index--;			//over prev window

	app_list_create_mp3_entry_list ();

	app_window_data.window_index++;
	app_window_data.app_window[app_window_data.window_index].list_index = entry;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_entry_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_entry_nav_window;

	app_timer_window_time_set (app_window_current_get()->window_time);
		
	app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;

	app_window_disp_track_init();
	disp_folder_flash = 0;

	#ifdef LCD_DISP_FILE_NAME_TRACK_SKIP_IN_STOP
	window_disp_state = enDISP_TRACK_NAME_ROLLING_START;
	#endif
#endif //MP3_FOLDER_KEY_SUPPORT
}

void app_window_mp3_folder_entry_next_hold (void)
{
#if 1//def MP3_FOLDER_KEY_SUPPORT
	I16 m,i,entry;

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;


	app_lcd_cancel_roll_function();
	m = app_window_data.window_index;

	i = app_window_data.app_window[m].list_index + app_window_data.app_window[m].list_cursor;
			
	/*entry number*/
	//i = dynamic_mp3_list[i].index;
	i = app_list_get_list_content(i);
	
	#ifdef CD_ENABLE
	#ifdef CD_SERVO_ENABLE
	if (global_vari.audio_source == MEDIA_CD)
	{
		entry = app_cd_get_first_entrie_of_folder(i);
	}
	else
	#endif
	#endif
	{
		entry = app_file_get_first_entrie_of_folder(i);
	}

	app_window_data.window_index--;			//over prev window

	app_list_create_mp3_entry_list ();

	app_window_data.window_index++;
	app_window_data.app_window[app_window_data.window_index].list_index = entry;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_entry_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_entry_nav_window;

	app_timer_window_time_set (app_window_current_get()->window_time);
		
	app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;

	disp_folder_flash = 0;

	while (keysInQueue == 0)
	{
		app_window_nav_next_roll ();
		app_timer_window_time_set (0);
		app_nav_sleep (250); 
	}

	app_timer_window_time_set (app_window_current_get()->window_time);
#endif //MP3_FOLDER_KEY_SUPPORT
}

//for pressing "skipb" key in mp3_folder_nav_window
void app_window_mp3_folder_entry_prev (void)
{
#if 1//def MP3_FOLDER_KEY_SUPPORT
	I16 m,i,entry;

	if ( (app_media_data.playback_mode == RANDOM_ALL) ||
		(app_media_data.playback_mode == RANDOM_REPEAT_ALL) ||
		(app_media_data.playback_mode == RANDOM_IN_DIR) ||
		(app_media_data.playback_mode == RANDOM_IN_DIR_REPEAT_ALL) )
	{
		return;
	}

	if (app_media_data.disp_info == DISP_INFO_ENTRY_FOLDER_NUMBER) {
	}
	else {
		app_media_data.disp_info = DISP_INFO_FILE_NAME;
	}
	
	app_lcd_cancel_roll_function();
	m = app_window_data.window_index;

	i = app_window_data.app_window[m].list_index + app_window_data.app_window[m].list_cursor;
			
	/*entry number*/
	//i = dynamic_mp3_list[i].index;
	i = app_list_get_list_content(i);
	
	#ifdef CD_ENABLE
	#ifdef CD_SERVO_ENABLE
	if (global_vari.audio_source == MEDIA_CD)
	{
		entry = app_cd_get_first_entrie_of_folder(i);
	}
	else
	#endif
	#endif
	{
		entry = app_file_get_first_entrie_of_folder(i);
	}

	if (entry > 0)
	{
		entry--;
	}
	else
	{
		entry = app_media_data.total_tracks - 1;
	}

	app_window_data.window_index--;			//over prev window

	app_list_create_mp3_entry_list ();

	app_window_data.window_index++;
	app_window_data.app_window[app_window_data.window_index].list_index = entry;
	app_window_data.app_window[app_window_data.window_index].list_cursor =0;
	app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_entry_nav_window;
	
	app_media_data.window = (WINDOW *) &mp3_entry_nav_window;

	app_timer_window_time_set (app_window_current_get()->window_time);
		
	app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;

	app_window_disp_track_init();
	disp_folder_flash = 0;

	#ifdef LCD_DISP_FILE_NAME_TRACK_SKIP_IN_STOP
	window_disp_state = enDISP_TRACK_NAME_ROLLING_START;
	#endif
#endif //MP3_FOLDER_KEY_SUPPORT
}

void app_window_mp3_folder_entry_prev_hold (void)
{
#if 1//def MP3_FOLDER_KEY_SUPPORT
	I16 m,i,entry;

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	m = app_window_data.window_index;

	i = app_window_data.app_window[m].list_index + app_window_data.app_window[m].list_cursor;
			
	/*entry number*/
	i = app_list_get_list_content(i);
	
	#ifdef CD_ENABLE
	#ifdef CD_SERVO_ENABLE
	if (global_vari.audio_source == MEDIA_CD)
	{
		entry = app_cd_get_first_entrie_of_folder(i);
	}
	else
	#endif
	#endif
	{
		entry = app_file_get_first_entrie_of_folder(i);
	}

	if (entry > 0)
	{
		entry--;
	}
	else
	{
		entry = app_media_data.total_tracks - 1;
	}

	app_window_data.window_index--;			//over prev window

	app_list_create_mp3_entry_list ();

	app_lcd_cancel_roll_function();
	app_window_data.window_index++;
	app_window_data.app_window[app_window_data.window_index].list_index = entry;
	app_window_data.app_window[app_window_data.window_index].list_cursor =0;
	app_window_data.app_window[app_window_data.window_index].window = (WINDOW *) &mp3_entry_nav_window;
	
	app_window_current_get() = (WINDOW *) &mp3_entry_nav_window;

	app_timer_window_time_set (app_window_current_get()->window_time);
		
	app_window_current_get()->draw_title ();
	app_window_current_get()->draw_region ();

	app_media_data.status = enSTATE_BROWSE_TRACK;

	disp_folder_flash = 0;

	while (keysInQueue == 0)
	{
		app_window_nav_prev_roll ();
		app_timer_window_time_set (0);
		app_nav_sleep (250); 
	}

	app_timer_window_time_set (app_window_current_get()->window_time);
#endif //MP3_FOLDER_KEY_SUPPORT
}

void app_window_mp3_folder_album_next_hold (void)
{
#if 1

	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch)
	{
		return;
	}
	#endif

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_key_hold_set_timer(200);	
	app_window_nav_next_roll ();

	app_timer_window_time_set (app_window_current_get()->window_time);

#else

	U8 temp = app_media_data.disp_info;

	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch)
	{
		return;
	}
	#endif

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;
	while (keysInQueue == 0)
	{
		app_window_nav_next_roll ();
		app_timer_window_time_set (0);
		app_nav_sleep (250); 
	}

	app_timer_window_time_set (app_window_current_get()->window_time);
#endif
}

void app_window_mp3_folder_album_prev_hold (void)
{
#if 1

	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch)
	{
		return;
	}
	#endif

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;

	app_timer_key_hold_set_timer(200);	
	app_window_nav_prev_roll ();

	app_timer_window_time_set (app_window_current_get()->window_time);

#else

	U8 temp = app_media_data.disp_info;

	#ifdef FOLDER_SWITCH
	if (!app_media_data.fold_switch)
	{
		return;
	}
	#endif

	app_media_data.disp_info = DISP_INFO_ENTRY_FOLDER_NUMBER;
	while (keysInQueue == 0)
	{
		app_window_nav_prev_roll ();
		app_timer_window_time_set (0);
		app_nav_sleep (250); 
	}

	app_timer_window_time_set (app_window_current_get()->window_time);
#endif
}

/***********************************************
*******      mp3 folder window SET      ********
***********************************************/

void app_window_mp3_folder_entry_roll_timer (void)
{
	U16 temp;
	int window_timer_count;

	DBG_Printf("%s\n\r", __func__);

	window_timer_count = app_window_data.app_window[app_window_data.window_index].win_cn ++;
	app_timer_window_time_set(app_window_current_get()->window_time);

#ifdef LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
	//if (app_media_data.disp_info != DISP_INFO_ENTRY_FOLDER_NUMBER)
	{
		temp = window_disp_state;
		switch (temp)
		{
		#if 0//def LCD_DISP_FILE_NAME_TRACK_SKIP_IN_STOP
		case enDISP_TRACK_NAME_ROLLING_START:
			window_disp_state = enDISP_TRACK_NAME_ROLLING_END;
			break;
			
		case enDISP_TRACK_NAME_ROLLING_END:
			if (!LcdDisp_Roll)
			{
			}
			break;
		#endif
			
		#ifdef LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
		case enDISP_FOLDER_NAME_ROLLING_START:
			app_lcd_disp_mp3_folder_index_folder_name(LINE1,  app_media_data.fold);
			app_lcd_disp_scroll_string((U8 *)song_information, LINE1, SCROLL_ONCE, 2);
			window_disp_state = enDISP_FOLDER_NAME_ROLLING_END;
			break;
			
		case enDISP_FOLDER_NAME_ROLLING_END:
			if (!LcdDisp_Roll)
			{
				U16 track_index;

				#if 0			
				app_lcd_disp_mp3_folder_index_folder_name(LINE1,  app_media_data.fold);
				#else
				if (global_vari.audio_source == MEDIA_CD)
				{
					track_index = app_cd_get_first_entrie_of_folder(app_media_data.fold);
				}
				else
				{
					track_index = app_file_get_first_entrie_of_folder(app_media_data.fold);
				}
				app_lcd_disp_mp3_entry_number_and_file_name(track_index, 0);
				#endif
			}
			break;
		#endif
		}
	}

#else

	if (window_timer_count >= 20)
	{
		app_nav_mp3_stop();
		return;
	}
	
	#ifndef LCD_DISP_SIMPLE
	app_timer_window_time_set (mp3_entry_nav_window.window_time);
	app_lcd_disp_stop_pathname_roll(song_information, 1);
	#endif

#endif
}

const APP_NAV_KEY APP_NAV_MP3_FOLDER_NAV_KEY [] = 
{
	{APP_KEY_SOURCE,NULL,app_window_mp3_cdfile_nav_play,0,NULL},
	{APP_KEY_POWER,NULL,app_nav_mp3_stop,0,NULL},

	#if 0
	{APP_KEY_SKIPF,app_window_mp3_folder_entry_skip_dummy,app_window_mp3_folder_entry_next,800,app_window_mp3_folder_entry_next_hold},  //hold on key
	{APP_KEY_SKIPB,app_window_mp3_folder_entry_skip_dummy,app_window_mp3_folder_entry_prev,800,app_window_mp3_folder_entry_prev_hold},  //hold off key
	#else
	{APP_KEY_SKIPF,app_window_mp3_folder_entry_next,NULL,800,app_window_nav_next_hold_roll},	//skip f
	{APP_KEY_SKIPB,app_window_mp3_folder_entry_prev,NULL,800,app_window_nav_prev_hold_roll},	//skip b
	#endif

	{APP_KEY_PLUS,app_nav_mp3_plus,NULL,800,app_nav_mp3_plus_hold},      //plus
	{APP_KEY_MINUS,app_nav_mp3_minus,NULL,800,app_nav_mp3_minus_hold},   //minus

	#ifdef LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
	{APP_KEY_FOLDER_F,app_window_nav_folder_next_roll,NULL,800,app_window_mp3_folder_album_next_hold}, //Album +
	{APP_KEY_FOLDER_B,app_window_nav_folder_prev_roll,NULL,800,app_window_mp3_folder_album_prev_hold}, //Album -
	#else
	{APP_KEY_FOLDER_F,app_window_nav_next_roll,NULL,800,app_window_mp3_folder_album_next_hold},  //Album +
	{APP_KEY_FOLDER_B,app_window_nav_prev_roll,NULL,800,app_window_mp3_folder_album_prev_hold},  //Album -
	#endif

};

const WINDOW mp3_folder_nav_window =
{                                    
	"mp3_folder_nav_window",
	app_window_draw_region_none,            //draw region
	KEY_TABLE(APP_NAV_MP3_FOLDER_NAV_KEY),
	500,                                    //window time
	app_window_mp3_folder_entry_roll_timer, //window timer handler
	WINDOW_RESERVES
};

#endif //MP3_FOLDER_KEY_SUPPORT

/***********************************************
*******       no disc window SET        ********
***********************************************/

void app_no_disc_window_timer (void)
{
	app_nav_auto_play_flag_set(FALSE);

	app_window_time_start ();

    U16 no_disc_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (no_disc_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }	
    #endif

	if  (app_window_current_get() != (WINDOW *) &no_disc_window)
	{
		return;
	}
}

const WINDOW no_disc_window =
{
	"no disc window",
	app_window_draw_region_none,         	//draw region
	KEY_TABLE(APP_NAV_READINGCD_KEY),
	1000,                                	//window time(ms)
	app_no_disc_window_timer,     			//window time handler
	WINDOW_RESERVES
};


#else
U8 decode_block_process_timer;
U16	dynamic_list_entry;
U16	dynamic_list_attribute;
APP_MEDIA_DATA app_media_data;

void app_nav_sleep (U16 ms) 
{
	/*
	*TX_SLEEP_MS(ms);
	*/
	TASK_SLEEP_MS(ms);
}

void uiDecDecodeFrameExit (void)
{	
	if (decode_flag_get())
	{
		decode_exit_req_set();
	}

	decode_exit_wait_forever_msgSend();
}

U16 file_name [MAX_DISP_FILE_FOLDER_NAME+1];
U16 folder_name[MAX_DISP_FILE_FOLDER_NAME+1];
#endif //( (defined USB_HOST_ENABLE) && (defined USB_HOST_MSC_ENABLE) )
