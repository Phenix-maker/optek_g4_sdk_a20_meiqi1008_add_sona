#ifndef __APP_MEDIA_CONFIG_H__
#define __APP_MEDIA_CONFIG_H__

#include "fifo.h"
#include "system_task.h"


#if 1
enum {
 	STREAM_CODEC_UNKNOWN = 0,
	STREAM_MP3,
	STREAM_MP2,
	STREAM_MP1,
	STREAM_WMA,
	STREAM_AAC_RAW,
	STREAM_BIN,
	STREAM_CDDA,
	STREAM_JPG,
	STREAM_WAV,
	STREAM_HE_AAC,
	STREAM_FLAC,
	STREAM_APE,
	STREAM_WMA_PRO,
	STREAM_WMA_LOSSLESS,
	STREAM_ADTS,
	STREAM_ADIF,
	STREAM_LOAS,
	STREAM_M4A,
	STREAM_SBC,
	STREAM_AC3,
	STREAM_EC3,
	STREAM_DTSHD
};
#endif

//enum {
//	FILE_AUDIO,
//	FILE_PHOTO,
//	FILE_RECORD
//};


/*
*MAX_DISP_FILE_FOLDER_NAME SUPPORT
*64
*32
*/
#define MAX_DISP_FILE_FOLDER_NAME    32

extern U16 file_name [MAX_DISP_FILE_FOLDER_NAME+1];
extern U16 folder_name[MAX_DISP_FILE_FOLDER_NAME+1];

/*
 * MARCO DEFINEs
 ***********************************************************************
 */

#define USB_IRP_EVENT_SET            os_event_set(USB_IRP_EVENT)
#define USB_IRP_EVENT_iSET           os_event_iset(USB_IRP_EVENT)
#define USB_IRP_EVENT_GET            os_event_get(USB_IRP_EVENT)
#define USB_IRP_EVENT_CLR            os_event_clr(USB_IRP_EVENT)
#define USB_IRP_EVENT_iCLR           os_event_iclr(USB_IRP_EVENT)

#define USB_HTD_EVENT_SET            os_event_set(USB_HTD_EVNET)
#define USB_HTD_EVENT_iSET           os_event_iset(USB_HTD_EVNET)
#define USB_HTD_EVENT_GET            os_event_get(USB_HTD_EVNET)
#define USB_HTD_EVENT_CLR            os_event_clr(USB_HTD_EVNET)

#define HDD_READ_FILE_EVENT_SET      os_event_set(HDD_READ_FILE_EVENT)
#define HDD_READ_FILE_EVENT_GET      os_event_get(HDD_READ_FILE_EVENT)

#define HDD_READ_FILE_EVENT_TIME_OUT_GET(xTicksWaitTime)\
                                     os_event_time_out_get(HDD_READ_FILE_EVENT, xTicksWaitTime)

#define HDD_READ_FILE_EVENT_CLR      os_event_clr(HDD_READ_FILE_EVENT)
#define HDD_READ_FILE_EVENT_iSET     os_event_iset(HDD_READ_FILE_EVENT)



#define STREAM_ID			U16

#define DEPTH_OF_HDD_QUEUE          16
#define DEPTH_OF_USBMSG_QUEUE       16
#define DEPTH_OF_SD_QUEUE           8
#define DEPTH_OF_IPOD_QUEUE         16
#define WIDTH_OF_HDD_QUEUE			4
#define WIDTH_OF_USBMSG_QUEUE       8

#define USB_LUN_OPEN_MAX    (5)
#define MIC_FIFO_BUF_SIZE   (64)


extern void *mutex_usb;
#define USB_MUTEX_LOCK        	os_mutex_lock(mutex_usb)
#define USB_MUTEX_UNLOCK      	os_mutex_unlock(mutex_usb)

extern void *mutex_fs;
#define FS_THREAD_LOCK        os_mutex_lock(mutex_fs)
#define FS_THREAD_UNLOCK      os_mutex_unlock(mutex_fs)

extern void *mutex_stream;
#define STREAM_MUTEX_LOCK     os_mutex_lock(mutex_stream)
#define STREAM_MUTEX_UNLOCK   os_mutex_unlock(mutex_stream)


extern void *qHDD;
#define QUEUE_HDD_SEND           os_queue_send(qHDD, data)
#define QUEUE_HDD_iSEND          os_queue_isend(qHDD, data)
#define QUEUE_HDD_RECEIVE        os_queue_receive(qHDD, data)

extern void *gUsbQueue;
#define QUEUE_USB_SEND           os_queue_send(gUsbQueue, data)
#define QUEUE_USB_iSEND          os_queue_isend(gUsbQueue, data)
#define QUEUE_USB_RECEIVE        os_queue_receive(gUsbQueue, data)

#define USBhost_adc_pcm_data_read

#define USB_POWER_ON
#define USB_POWER_OFF

//sample freq list
#define USBHOST_AUDIO_SRC_16K_BIT     (1<<0)
#define USBHOST_AUDIO_SRC_32K_BIT     (1<<1)
#define USBHOST_AUDIO_SRC_44P1K_BIT   (1<<2)
#define USBHOST_AUDIO_SRC_48K_BIT     (1<<3)
#define USBHOST_AUDIO_SRC_64K_BIT     (1<<4)
#define USBHOST_AUDIO_SRC_88P2K_BIT   (1<<5)
#define USBHOST_AUDIO_SRC_96K_BIT     (1<<6)
#define USBHOST_AUDIO_SRC_128K_BIT    (1<<7)
#define USBHOST_AUDIO_SRC_176P4K_BIT  (1<<8)
#define USBHOST_AUDIO_SRC_192K_BIT    (1<<9)



/*
 * VARIBLE DECLAREs
 ************************************************************
 */
extern volatile U8 usbDevType;
extern U8 gUSBMesg;

extern U8 USB_SPEAKER_STREAM_START[];
extern U8 USB_SPEAKER_STREAM_END[];

extern FIFO UsbDevUpFifo;//JJ+ for USB MIC
extern U8 micFifo_buf[MIC_FIFO_BUF_SIZE];

extern const U16 ROOT_DIR[];
extern const U16 PARENT_DIR[];
extern const U16 ALL_FILE[];
extern const U16 RECORD_FOLDER[];

extern const U16 SYSTEM_FOLDER[];
extern const U16 upgrade_bin[];
extern const U16 usb_test_file[];



/*
 * FUNCTION DECLAREs
 **************************************************************
 */
void uHddMsgSend (U16 msg, U32 parm1, U16 parm2);
void ukParmSend (FUNCTION key_func, U32 parm1);


enum {
	AUDIO_OUT_L_R = 0,
	AUDIO_OUT_L_R_Sub,
	AUDIO_OUT_L_R_Center_Sub,
	AUDIO_OUT_LS_RS,
	AUDIO_OUT_Center_Sub,
	AUDIO_OUT_L_R_Sub_Center_LS_RS,
	AUDIO_OUT_L_R_Center_Sub_LS_RS
};


typedef struct {
	U32 source;
	U32 parm1;
	U32 parm2;
} USB_UI_MSG;


extern U8 audio_iso_mute;

//enum {
//	UI_FS_UNKNOW,
//	UI_FS_FIT_TIMER,
//	UI_FS_SEARCH_END,
//	UI_FS_STREAM_UNDERFLOW,
//};


#endif //__APP_MEDIA_CONFIG_H__