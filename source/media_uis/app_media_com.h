#ifndef __APP_MEDIA_COM_H__
#define __APP_MEDIA_COM_H__

#include "ide_file.h"
#include "system_task.h"


extern U8 MP3_STREAM_START[];
extern U8 MP3_STREAM_END[];


#define MIN_STREAM_SIZE		(4096/2)

enum {
	USB_DEV_UNPLUG,
	USB_DEV_PLUG,
	USB_OVERCURRENT,
	USB_NO_MSG = 0xFF
};

typedef	enum {
	enUSBH_DEV_UNKOWN,
	enUSBH_DEV_MSC,
	enUSBH_DEV_PTP,
	enUSBH_DEV_MTP,
	enUSBH_DEV_IPOD_AUDIO,
	enUSBH_DEV_AOA,
	enUSBH_DEV_AUDIO //USB Host Audio
} enum_USBH_DEVICE;

enum {
	USB_PORT_CONNETED_TO_IPHONE,
	USB_PORT_CONNETED_TO_USB,
	USB_PORT_CONNETED_TO_USB_1,
	USB_PORT_CONNETED_TO_USB_VIDEO,
	USB_PORT_CONNETED_PC_TO_USB_SPEAKER,
	USB_PORT_CONNETED_PC_TO_IPHONE,
	UART_PORT_CONNETED_TO_IPHONE,
	USB_PORT_CONNETED_TO_IDLE
};

enum {
	HDD_READ_FILE,
	HDD_INIT,
	HDD_POWER_ON,
	HDD_POWER_OFF,
	HDD_OPEN_FILE,
	HDD_CLOSE_FILE,
	HDD_POWER_ON_RESET,
	HDD_SEARCH_AUDIO_ENTRY,
	HDD_SEARCH_PHOTO_ENTRY,
	HDD_SEARCH_RECORD_ENTRY,

	HDD_CREATE_VOICE_FILE,
	HDD_WRITE_VOICE_FILE,
	HDD_CLOSE_VOICE_FILE,
	HDD_FILE_SEEK,
	HDD_FILE_REINIT,
	USB_MSC_INIT,
	HDD_WRITE_ENC_FILE,
	USB_MSC_CLOSE,
	USB_LUN_OPEN,
	USB_LUN_CLOSE,

	USB_LUN_AUDIO_ENTRY,
	HDD_WRITE_CPY_FILE,
	HDD_DDELETE_ENTRY,
	HDD_DELTE_FOLDER,
	HDD_DELETE_ALL,
	HDD_DELETE_PROG,
	USBH_MTP_INIT,
	USBH_MTP_SEARCH_AUDIO_ENTRY,
	USBH_MTP_READ_FILE,
	USBH_MTP_READ_PARTIAL_FILE,

	USBH_MTP_WRITE_ENC_FILE,
	USBH_MTP_WRITE_CPY_FILE,
	USB_DEV_PLUG_IN,
	USB_DEV_HUB_PLUG_IN,
	SDMMC_CONNECT,
	SDMMC_INIT,
	SDMMC_CLOSE,
	IPHONE_CONNECT,
	IPHONE_CLOSE,
	USB_LUN_FATAL_ERR,

	HDD_SERCH_F_SPACE
};

enum {
	ITEM_NORMAL,
	ITEM_SEPERATE,
	ITEM_ENTRY,
	ITEM_FOLDER,
	ITEM_ARTIST,
	ITEM_TITLE,
	ITEM_ALBUM,
	ITEM_GENRE,
	ITEM_PLAYLIST_ENTRY
};

enum {
	DISP_INFO_UNKOWN,
	DISP_INFO_FILE_NAME,
	DISP_INFO_FOLDER_NAME,
	DISP_INFO_WAITING,
	DISP_INFO_ALBUM,
	DISP_INFO_ARTIST,
	DISP_INFO_TITLE,
	DISP_INFO_ENTRY_FOLDER_NUMBER,
	DISP_INFO_ID3,
	DISP_INFO_REMAIN_TIME,
	DISP_INFO_FOLDER_FILE_NAME,
	DISP_INFO_1ST_PLAYBACK_TIME,
	DISP_INFO_PLAYBACK_TIME,
	DISP_INFO_FOLDER_NUMBER_PLAYBACK_TIME,
	DISP_INFO_PAUSE_TIME,
	DISP_INFO_MUTE
};

enum {
	AUTO_PLAY_ON,
	AUTO_PLAY_OFF
};

enum {
	RESUME_ON,
	RESUME_OFF
};

enum {
	PLAYLIST_OFF,
	PLAYLIST_ON
};

enum {
	PLAYING_MODE_STOP,
	PLAYING_MODE_PLAY,
	PLAYING_MODE_PAUSE
};

enum {
	NORMAL_PLAY = 0,
	REPEAT_ONE,
	REPEAT_DIR,
	REPEAT_ALL,
	REPEAT_ARTIST,
	RANDOM_ALL,
	RANDOM_REPEAT_ALL,
	RANDOM_IN_DIR,					//random in dir
	RANDOM_IN_DIR_REPEAT_ALL,
	RANDOM_ARTIST,
	DIR_RANDOM,						//random dir
	DIR_RANDOM_REPEAT_ALL,			//random dir repeat
	INTRO_PLAY
};

enum {
	INTRO_ON,
	INTRO_OFF
};

enum {
	enMP3_PLAY_UNKNOWN_FLAG,
	enMP3_PLAY_FLAG,
	enMP3_SKIPF_FLAG,
	enMP3_SKIPB_FLAG
};

enum {
	enSKIP_MINUS,
	enSKIP_PLUS,
	enSKIP_UNKNOWN,
};

enum {
	WSTREAM_BUSY,
	WSTREAM_IDLE
};

enum {
	NO_SKIP_REQUEST,
	SKIP_FORWARD,
	SKIP_BACKWARD,
	FAST_FORWARD,
	FAST_BACKWARD
};

typedef enum {
	AUDIO_SRC_8BIT = 0,
	AUDIO_SRC_16BIT,
	AUDIO_SRC_24BIT,
	AUDIO_SRC_32BIT_SBIT_H24, //Significant bit is high 24b
	AUDIO_SRC_UNKNOWBIT
}AUDIO_SRC_BIT;


enum {
	//0
	STREAM_UNKNOWN,
	STREAM_MEDIA,
	STREAM_SEARCH_IDV1,
	STREAM_SEARCH_IDV2,
	STREAM_MP3_INIT,
	STREAM_MP3_FIND_SYNC,
	STREAM_MP3_WAIT_DATA,
	STREAM_WMA_INIT,
	STREAM_WMA_WAIT_DATA,
	STREAM_WMA_SEEK_INIT,

	//10
	STREAM_CDDA_DECODE,
	STREAM_WAITING_DATA,
	STREAM_DECOCDE_FINISH,
	STREAM_DECOCDE_EXIT,
	STREAM_IDLE,
	STREAM_UDF_FILE_ENTRY_DESCRIPTOR,
	STREAM_WMA_INIT_WAIT,
	STREAM_MP3_INIT_WAIT,
	STREAM_DEC_INIT,
	STREAM_DEC_INIT_WAIT,
};


#if 1//def FOR_ESD_PROTECT
typedef struct {
	U8 need_resume_play;
	U8 playback_state;
	U16 resume_track;
	U16 resume_playing_index;
	U16 resume_playback_time;
	U32 resume_pos;

} RESUME_TRK_INFO;

extern RESUME_TRK_INFO resume_trk_info;



typedef struct
{
    U8 playing_stream_status;
    U32 playing_stream_sample_rate;

	#if 1
	U16 playing_track;				/*current playing track*/
	U16 stream_track;				/*current reading stream track*/
	U16 playing_index;				/*current playing index in playlist*/
	U16 stream_index;				/*current reading stream index in playlist*/
	U16 playing_stream_type;		/*playing track stream type*/
	U16 coming_stream_type;			/*reading stream type*/
	U16 fold;
	U16 entry;
	U16 total_tracks;
	U16 total_folders;

	#if 1//(defined RECORD_ENABLE || defined ADC_RECORD)
	//U8 CdRipping;
	U8 record_status;
	U8 record_media;
	U32 record_bitrate;
	U32 record_time;
	U8 record_format;
	#endif

	U8 playing_stream_chans;
	U8 playing_stream_sample_percent;
	U8 playing_stream_sample_bits;
	U16 playing_stream_total_time;
	U32 playing_stream_offset;				//content start pos in stream
	U32 playing_stream_media_offset;		//comtent start pos in media

	U8 fast_processing;

#if	1//def FAST_SEARCH_ENABLE
	U16 fast_step_ms;		//ms
	U32 fast_total_ms;		//ms
#endif

	U8 playlist;

	U8 fold_switch;

	U8  resume_playback;
	U16 resume_track;
	U16 resume_playing_index;

	U16 resume_playback_time;
	U8  resume_playback_request;
	U8  resume_jump_request;
	U32 resume_sample_rate;
	U8  fast_jump_request;

	U8 stream_source;
	U8 disp_info;
	U8 fatal_err;

	U8 disc_content_ready;
	U8 playback_state;
	U8 playback_mode;
	U8 playback_intro;

	U8 status;
	U8 auto_play;
	U8 resume;
	U8 pgm_edit;

	I16 total_playlist_entry;
	U8 toc_ready;
	U16 playing_time;

	I16 random_played_tracks;

	U8 PlayDiscNum;
	U8 playing_stream_vbr;
	U32 playing_stream_pcm_sample;
	//U16 playing_time;

	I8 coming_stream_status;
	//I8 playing_stream_status;

	WINDOW	*window;
	U8 Mute;
	U32 playing_stream_bitrate;

	AUDIO_SRC_BIT audio_src_bit;
	
	U8 wMediaStatus;
	U32 cdrom_pre_baddr;

	#endif

} APP_MEDIA_DATA;

extern APP_MEDIA_DATA app_media_data;



extern U8 skip_key_flag;
extern volatile MYFILE *pMediaRead;
#define MAX_DISP_LENTH			64

#define NO_ENTRY_IN_FOLDER	0xFFFF

extern U8 codec_init_fail;
extern volatile U8 wRequestDecodeFrameExit;

#endif


extern U8 gUSBMesg;

void _unicode_to_oem (U16 *buf, U8 *d);

extern const WINDOW readingcd_window;
extern const WINDOW no_disc_window;
extern const WINDOW mp3_mode_window;
extern const WINDOW nav_mp3_window;
extern const WINDOW nav_mp3_prot_window;
extern const WINDOW nav_mp3_pause_window;
extern const WINDOW mp3_entry_nav_window;
extern const WINDOW nav_mp3_stop_window;
extern const WINDOW dummy_window;

static void app_nav_mp3_start_fast(int ff);

void app_window_mp3_cdfile_nav_play (void);

void app_nav_mp3_play (void);
void app_nav_usb_mp3_play (void);

void app_nav_mp3_play_track(U16 index);
void app_nav_usb_mp3_play_track (U16 index);

void app_nav_mp3_skipf (void);
void app_nav_mp3_skipb (void);

void app_nav_usb_mp3_skipf (void);
void app_nav_usb_mp3_skipb (void);
void app_nav_usb_mp3_skipb_1 (void);

void app_nav_mp3_playback_over (void);

void app_nav_mp3_stop (void);
void app_nav_mp3_stop_2 (int con);

void app_nav_mp3_process (system_msg_stru *process_msg);

void app_nav_mp3_play_window_init(void);
void USB_Port_Connect_Switch(U8 con);
void app_id3_state_init(void);

void app_nav_init_draw_playback_time(void);
void app_window_disp_track_init(void);

void app_nav_usb_hub_plug_in(void);
void app_nav_usb_device_connected();
void app_nav_usb_device_disconnected();
void app_nav_media_fata_error (void);

void USB_PowerOn(void);
void USB_PowerOff(U8 con);

#endif /* __APP_MEDIA_COM_H__ */
