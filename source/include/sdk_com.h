#ifndef _SDK_COM_H_
#define _SDK_COM_H_

#include "os_type.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"
#include "window.h"
#include "app_media_com.h"


#define WIDTH_OF_AUDIO_INPUT_RX_QUEUE   4
#define DEPTH_OF_AUDIO_INPUT_RX_QUEUE   16



/* tasks priority allocation */
#define TASK_AUDIO_RCV_PRIORITY	    (12)
#define TASK_AUDIO_PRIORITY         (11)
#define TASK_BTDM_PRIORITY  		(13)
#define TASK_HOST_PRIORITY	        (9)
#define TASK_USB_PRIORITY           (8)
#define TASK_HDD_PRIORITY_HI        (7)
#define TASK_SYSTEM_PRIORITY        (6)
#define TASK_APPFUNC_PRIORITY       (5)
#define TASK_HDD_PRIORITY           (3)
#define TASK_IDLE_PRIORITY          (0)


/* Events: <= 32 */
#define AUDIO_DECODE_EVENT                  (1<<0)
#define EVENT_TX_DMA7_COMPLETE              (1<<1)
#define UART_TX_EVENT_FIFO_EMPTY            (1<<2)
#define EVENT_RX_DMA8_COMPLETE              (1<<3)
#define AUDIO_RECEIVE_EVENT                 (1<<4)
#define USB_URB_EVENT                       (1<<5)
#define USB_IRP_EVENT                       (1<<6)
#define USB_ISR_EVNET                       (1<<7)
#define USB_HTD_EVNET                       (1<<8)
#define UART_RX_EVENT_CHAR                  (1<<9)
#define UART_RX_EVENT_LINE_DATA             (1<<10)
#define HDD_READ_FILE_EVENT                 (1<<11)
#define UART1_TX_EVENT_FIFO_EMPTY           (1<<12)
#define UARTB_TX_EVENT_HALF_EMPTY           (1<<13)
#define IAP2_BT_RX_PKT			            (1<<14)
#define SD_DMA_TX_EVENT			            (1<<15)
#define BT_A2DP_TX_EVENT			        (1<<16)
#define UART0_RX_EVENT			            (1<<17)
#define BT_CMD_EVENT			            (1<<18)

#define BT_CONTROLLER_EVENT			        (1<<19)
#define BT_CONTROLLER_RX_EVENT			    (1<<20)
#define BT_HOST_INTERNAL_MSG			    (1<<21)
#define STREAM_EVENT			            (1<<22)

#define HP_DECODE_EVENT						(1<<23)
#define DAPP_EVENT							(1<<24)
#define MP3_ENC_EVENT                       (1<<25)


typedef enum {
 	SOURCE_UNKNOWN = -1,
	SOURCE_INTERNAL = 0,
	SOURCE_AUX,
    SOURCE_HDMI,
    SOURCE_OPTICAL,
	SOURCE_ARC,
    SOURCE_USB_MSC, 		//USB Host
    SOURCE_USB_SPEAKER, 	//USB Slave
	SOURCE_COAXIAL,
    SOURCE_SD,
	SOURCE_BT,
	SOURCE_HFP,
	SOURCE_STANDBY,
	SOURCE_OPTEKLINK_SLAVE,
	MEDIA_LAST
} enumSOURCE_ID;

typedef struct{
	/*
	* rev and eventGroup flags are SOC system used,must not modify
	* */
    enumSOURCE_ID rev;
#ifdef RTOS_ENABLE
	EventGroupHandle_t eventGroup;
#endif
	//GLOBAL flags setting
	enumSOURCE_ID audio_source;
	enumSOURCE_ID last_audio_source;
	
	U8 optek_link_role;	
} GLOBAL_VARI_STRU;

extern GLOBAL_VARI_STRU global_vari;

void sdk_common_init(void);

/*
 ****************************************************************************************
 * TYPES DEFINITION
 ****************************************************************************************
 */
typedef void (*pFUNCTION) (void);
typedef void (*pFUNCTION_W_ONE_PARM) (U32 );
typedef void (*pFUNCTION_W_TWO_PARM) (U32 , U32 );

/* Enumeration of External Interface status codes */
enum extif_status
{
    /* EIF status OK */
    EXTIF_STATUS_OK,
    /* EIF status KO */
    EXTIF_STATUS_ERROR,
};

/**
 ****************************************************************************************
 * @brief Function called when packet transmission/reception is finished.

 * @param[in]  param  parameter returned to callback when operation is over.
 * @param[in]  status Ok if action correctly performed, else reason status code.
 *
 *****************************************************************************************
 */
typedef void (*extif_callback) (uint32_t param, uint8_t status);

/**
 * Transport layer communication interface.
 */
typedef struct
{
    /**
     *************************************************************************************
     * @brief Starts a data reception.
     *
     * @param[out] bufptr      Pointer to the RX buffer
     * @param[in]  size        Size of the expected reception
     * @param[in]  callback    Pointer to the function called back when transfer finished
     * @param[in]  param       parameter returned to callback when reception is finished
     *
	 *************************************************************************************
     */
    void (*read) (uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param);

    /**
     *************************************************************************************
     * @brief Starts a data transmission.
     *
     * @param[in]  bufptr      Pointer to the TX buffer
     * @param[in]  size        Size of the transmission
     * @param[in]  callback    Pointer to the function called back when transfer finished
     * @param[in]  param       parameter returned to callback when transmission is finished
     *
	 *************************************************************************************
     */
    void (*write)(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param);

    /**
     *************************************************************************************
     * @brief Enable Interface receive flow.
     *************************************************************************************
     */
    void (*flow_on)(void);

    /**
     *************************************************************************************
     * @brief Disable Interface receive flow.
     *************************************************************************************
     */
    void (*flow_off)(void);
    
} extif_api_stru;

//struct of external interface api

#if 1
/**
 * @enum 
 * @brief 
 */
enum {
	UI_FIT_TIMER,
	UI_STREAM_NEW_TRACK,
	UI_STREAM_UNDERFLOW,
	UI_STREAM_OVERFLOW,
	UI_STREAM_EOF,
	UI_DECODE_STREAM_EOF,	
	UI_CDROM_ADDR_ERR,
	UI_CDROM_NO_SYN,
	UI_CDROM_SECTOR_FIND,
	UI_ISO9660_UDF_ERR,
	
	UI_ISO9660_UDF_SUCCESS,
	UI_CDDA_EF_ERR,
	UI_CDDA_UNTISHOCK_ERR,
	UI_CDDA_ADDR,
	UI_CDDA_START_ADDR,
	UI_LCD_LINE1,
	UI_LCD_ALL_LINE,
	UI_NO_DISC,
	UI_TOC_READY,
	UI_READ_SESSION,

	UI_ISO9660_UDF_UPGRADE_FILE,
	UI_PCM_OUT_UNDERFLOW,
	UI_DECODE_SKIP_F,
	UI_NEW_SUBQ,
	UI_CDROM_UDF_DESCRIPTOR_ADDR_ERR,
	UI_CDROM_UDF_DESCRIPTOR_WRITE_END,
	UI_MP3_INIT_SUCCESS,
	UI_MP3_INIT_FAIL,
	UI_WMA_INIT_SUCCESS,
	UI_WMA_INIT_FAIL,
	
	UI_WMA_INIT_DRM,
	UI_CDROM_CRC_ERROR,
	UI_CD_LEAD_OUT,
	UI_USB_CONNECT,
	UI_USB_DISCONNECT,
	UI_FS_SEARCH_END,
	UI_FS_ERR,
	UI_USB_ABSENT,
	UI_USB_OVERCURRENT,
	UI_TRAY_LOADER_READY,
	
	UI_RDS_POLLING,
	UI_DEC_INIT_SUCCESS,
	UI_DEC_INIT_FAIL,	
	UI_WAVE_INIT_SUCCESS,
	UI_WAVE_INIT_FAIL,
	UI_FLAC_INIT_SUCCESS,
	UI_FLAC_INIT_FAIL,
	UI_DECODE_FINISH,
	UI_SPDIF_STREAM_OVERFLOW,	
	UI_SPDIF_STREAM_UNDERFLOW,

	UI_DECODE_SKIP_B,
	
    #ifdef DSPEQ_TUNING_ENABLE
	UI_FIT_SET_EQ
    #endif
};

#else

enum {
	UI_SPDIF_STREAM_OVERFLOW,
	UI_SPDIF_STREAM_UNDERFLOW,
};

#endif

enum {
    AUDIO_IN_STREAM_PREPARE_UNKNOWN,
    AUDIO_IN_STREAM_PREPARE_WAITING,
    AUDIO_IN_STREAM_PREPARE_READY,
    AUDIO_IN_STREAM_PREPARE_IDLE,
};

enum {
	SAMPLE_SPEED_1X = 1,
	SAMPLE_SPEED_2X = 2,
	SAMPLE_SPEED_4X = 4,
	SAMPLE_SPEED_8X = 8,
};

typedef struct
{
    U16 status;
    U16 rx_enable_flag;
    U16 prepare_status;
	U8 stream_type;
	U8 channel;
	U8 sample_speed;
    U8 bit_width;
    U32 sample_rate;
    U32 bit_rate;
    U32 offset;	
} AUDIO_IN_STREAM_STRUC;

extern AUDIO_IN_STREAM_STRUC audio_in_stream;


enum {
	AUDIO_OUT_DEC_UNKNOWN,
	AUDIO_OUT_DEC_PREPARE,
	AUDIO_OUT_DEC_DECODING,
	AUDIO_OUT_DEC_EXIT,
};

typedef struct
{
    U16 status;
    U16 dec_status;
    U32 sample_rate;
    U32 bit_rate;
    U32 offset;
    U8 bit_width;
    U8 channel;
    U8 decode_out_channel; //changed by codec decoding
    U8 sample_speed;

	U8 input_channel;
} AUDIO_OUT_STREAM_STRUC;

extern AUDIO_OUT_STREAM_STRUC audio_out_stream;



#endif //_SDK_COM_H_

