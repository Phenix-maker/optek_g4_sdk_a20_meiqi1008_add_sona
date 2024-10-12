/*
 * DisplayModule.h
 *
 *  Created on: 2024/1/29
 *      Author: Windows
 */

#ifndef __DISPLAY_MOUDLE_H__
#define __DISPLAY_MOUDLE_H__

#include "c_def.h"

enum
{
    dispBT_CLOSED,
    dispBT_OPENING,
	dispBT_OPENED,

	dispBT_CONNECTED,
	dispBT_DISCONNECTED,
	dispBT_PAIRING,
	dispBT_RELINK,
	dispBT_UPGRAGE,

	dispBT_ERROR,

	dispBT_AVRCP_STOPPED,
	dispBT_AVRCP_PLAYING,
	dispBT_AVRCP_PAUSED,
    dispBT_AVRCP_FWD_SEEK,
    dispBT_AVRCP_REV_SEEK,

    dispBT_AVRCP_REV_ERROR,
	dispBT_AVRCP_UNKNOWN,	


	dispBT_UNKNOWN = 0xFF
};

enum {
	dispUSB_UNKNOW	= -1,
	dispUSB_INIT,
	dispUSB_OPEN,
	dispUSB_INFO,
	dispUSB_READING,
	dispUSB_PLAYING,
	dispUSB_PAUSE,
	dispUSB_STOP

};

enum {
	dispDeCode_UNKNOW	= -1,
	dispDeCode_DD_AC3,
	dispDeCode_DDp_EC3,
	dispDeCode_ATMOS,
	dispDeCode_DTS,
	dispDeCode_DTS_X,
	dispDeCode_PCM,

	dispDeCode_BT_SBC,
	dispDeCode_BT_HFP,
	dispDeCode_BT_LeAudio,

	dispDeCode_MP3,
	dispDeCode_WMA

};

typedef struct {
	void (*init) (void);
	void (*open) (void);
	void (*close) (void);
	void (*fini) (void);
	void (*disp_source) (U8 audio_source);
	void (*disp_bt_status) (U32 flash_time);
	void (*disp_usb_status) (U8 usb_status);
	void (*disp_decode_type) (U8 decode_type);
	void (*disp_music_play) (U32 music_level);
	void (*disp_standby_status) (U8 standby_status);
	void (*disp_volume_status) (U8 value);
} APP_DISPLAY_MOUDLE;

extern const APP_DISPLAY_MOUDLE app_display_module;

void app_display_none_void(void);
void app_display_module_init(void);
void app_display_module_open(void);
void app_display_module_close(void);
void app_display_module_finish(void);
void app_display_module_disp_source(U8 audio_source);
void app_display_module_disp_bt_status(U32 flash_time);
void app_display_module_disp_usb_status(U8 usb_status);
void app_display_module_disp_decode_type(U8 decode_type);
void app_display_module_disp_music_play(U32 music_level);
void app_display_module_disp_standby_status(U8 standby_status);
void app_display_module_disp_volume_status(U8 value);

#define Display_Module		app_display_module_init, \
							app_display_module_open, \
							app_display_module_close, \
							app_display_module_finish, \
							app_display_module_disp_source, \
							app_display_module_disp_bt_status, \
							app_display_module_disp_usb_status, \
							app_display_module_disp_decode_type, \
							app_display_module_disp_music_play, \
							app_display_module_disp_standby_status, \
							app_display_module_disp_volume_status



#endif /* __DISPLAY_MOUDLE_H__ */
