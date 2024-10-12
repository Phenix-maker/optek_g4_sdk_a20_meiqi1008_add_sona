#ifndef _APP_PROCESS_H_
#define _APP_PROCESS_H_


enum {
	USB_MSC_UNKNOW_PROCESS = -1,
	USB_MSC_DUMMY_PROCESS,
	USB_MSC_READ_FS_PROCESS,
	USB_MSC_MP3_PROCESS
};


extern U8 randomAllFinish;
extern volatile U8 noDiscNum;
extern U8 audio_sample_mode;
extern U8 usb_msc_process_id;


void app_nav_nodisc_process (system_msg_stru *process_msg);
void app_nav_usb_read_fs_process (system_msg_stru *process_msg);
void app_nav_mp3_process (system_msg_stru *process_msg);
void app_nav_dummy_process (system_msg_stru *process_msg);
void app_no_dummy_process (system_msg_stru *process_msg);


#endif //_APP_PROCESS_H_
