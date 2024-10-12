#ifndef _APP_MEDIA_H_
#define _APP_MEDIA_H_

#include "ide_file.h"
#include "app_media_config.h"

typedef struct {
	U32	msg;
	U32	parm1;
	U32	parm2;
	U32	parm3;
} HDD_MSG;


#define MAX_FOLDER_NUM        500
#define MAX_REC_NUM           999


/**
 * when playging 192kHz 24-bit wave songs at 64Mhz, if selecting AUDIO_192K_DOWN_SAMPLE, it is ok
 * when playging 192kHz 24-bit wave songs at 64Mhz, if not selecting AUDIO_192K_DOWN_SAMPLEit is ok
 */
#if 1
#define IDE_BLOCK_SIZE        (1024*4)
#else
#define IDE_BLOCK_SIZE        (1024*64)
#endif


/* support AAC/SBC */
#if (defined DOLBY_AC3_DEC_TEST && defined WAV_DEC)
#define SEARCH_MASK    (SEARCH_MP3_MASK | SEARCH_AC3_MASK | SEARCH_WAV_MASK)
#elif (defined DOLBY_AC3_DEC_TEST)
#define SEARCH_MASK    (SEARCH_MP3_MASK | SEARCH_AC3_MASK | SEARCH_WAV_MASK)
#elif (defined WAV_DEC)
#define SEARCH_MASK    (SEARCH_MP3_MASK | SEARCH_WAV_MASK)
#else
#define SEARCH_MASK    (SEARCH_MP3_MASK)
#endif


enum {
	HDD_MEDIA_POWER_ON,
	HDD_MEDIA_POWER_OFF
};

extern U16 reading_percentage;
extern U16 reading_latest_fs;
extern U8 wFsReadErr;
extern volatile U8 wFileSysPresent;
extern volatile U8 gwReqDelCpyFile;
extern volatile U8 sd1_FileSysPresent;
extern volatile U8 usb_lunCount;
extern volatile U8 usb_lunIndex;
extern U8 usb_lunOpenOk;
extern U8 usb_lunNum[USB_LUN_OPEN_MAX];

extern U8 ide_buf[];
extern U8 hp_conn_req;
extern U8 media_write_err_cnt;

extern volatile MYFILE *pMediaRead;


extern U8 PHASE_KEY_STATUS;
extern U8 HIGH_PASS_MODE_KEY_STATUS;
extern U8 FULL_RANGE_OUTPUT_KEY_STATUS;

void app_media_mute_enable (void);
void app_media_mute_disable (void);
void app_media_new_album (void);
U8 app_media_mute_status (void);
U8 app_media_status_get(void);

U32 app_media_file_create_data_set(void);
U32 app_media_file_create_time_set(void);

#endif //_APP_MEDIA_H_
