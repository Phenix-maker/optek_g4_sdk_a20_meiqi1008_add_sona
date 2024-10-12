#ifndef __AUDIO_TASK_H__
#define __AUDIO_TASK_H__


#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"
#include "sdk_config.h"

#include "audio_codec.h"
// #include "se_link_24bit.h"



/**
 *************************************************************************************
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */
/*
*2 ->16-bit ouput
*4 ->32-bit or 24-bit output
*/ 
#if (IIS_OUTPUT_CHANNEL_BIT_NUM == 24)
#define AUDIO_OUTPUT_CHANNEL_BYTE_SIZE    4 /*24-bit ouput/32-bit output*/
#else
#define AUDIO_OUTPUT_CHANNEL_BYTE_SIZE    2 /*16-bit ouput*/
#endif

#ifdef DOLBY_DDPLUS71_DEC_ENABLE
#define AUDIO_OUTPUT_CHANNEL     AUDIO_OUT_STREAM_CHANNEL
#define AUDIO_OUTPUT_BUF_SIZE    (0x600*AUDIO_OUTPUT_CHANNEL_BYTE_SIZE*AUDIO_OUTPUT_CHANNEL*AUDIO_OUT_STREAM_SAMPLE_SPEED)
#elif defined DTS_DMP_DEC_EANBLE
#define AUDIO_OUTPUT_CHANNEL     AUDIO_OUT_STREAM_CHANNEL
#define AUDIO_OUTPUT_BUF_SIZE    (0x600*AUDIO_OUTPUT_CHANNEL_BYTE_SIZE*AUDIO_OUTPUT_CHANNEL*AUDIO_OUT_STREAM_SAMPLE_SPEED)
#elif defined MP3_DECODE_ENABLE
#define AUDIO_OUTPUT_CHANNEL     AUDIO_OUT_STREAM_CHANNEL
#define AUDIO_OUTPUT_BUF_SIZE    (0x4E4*AUDIO_OUTPUT_CHANNEL_BYTE_SIZE*AUDIO_OUTPUT_CHANNEL*AUDIO_OUT_STREAM_SAMPLE_SPEED)
#else
#define AUDIO_OUTPUT_CHANNEL     AUDIO_OUT_STREAM_CHANNEL
#define AUDIO_OUTPUT_BUF_SIZE    (0x192*AUDIO_OUTPUT_CHANNEL_BYTE_SIZE*AUDIO_OUTPUT_CHANNEL)
//#define AUDIO_OUTPUT_BUF_SIZE    ((192*AUDIO_OUTPUT_CHANNEL_BYTE_SIZE*AUDIO_OUTPUT_CHANNEL*AUDIO_OUT_STREAM_SAMPLE_SPEED)/4)
//#define AUDIO_OUTPUT_BUF_SIZE    (192*AUDIO_OUTPUT_CHANNEL_BYTE_SIZE*AUDIO_OUTPUT_CHANNEL*AUDIO_OUT_STREAM_SAMPLE_SPEED)
//#define AUDIO_OUTPUT_BUF_SIZE    ((192*AUDIO_OUTPUT_CHANNEL_BYTE_SIZE*AUDIO_OUTPUT_CHANNEL*AUDIO_OUT_STREAM_SAMPLE_SPEED)/2)
//#define AUDIO_OUTPUT_BUF_SIZE    ((192*AUDIO_OUTPUT_CHANNEL_BYTE_SIZE*AUDIO_OUTPUT_CHANNEL*AUDIO_OUT_STREAM_SAMPLE_SPEED)/2/2/2)
#endif


#ifdef I2S_TDM_DATA_WIDTH_24_BIT
#define DEST_DATA_LEN 768
#else
#if (defined ADC_DAC_DATA_WIDTH_24_BIT && (defined AUX_INPUT_FROM_IIS0 || defined AUX_INPUT_FROM_IIS1))
#define DEST_DATA_LEN 768
#else
#define DEST_DATA_LEN 384
#endif
#endif
extern U16 steam_data_len;



/**
 *************************************************************************************
 * STRUCT DEFINITION
 *************************************************************************************
 */

/**
 * @enum enumDECODE_MSG_ID
 * @brief Decode status enumeration
 */
typedef enum {
	DECODE_SET,
	DECODE_START,
	DECODE_INIT,
	DECODE_EXIT
} enumDECODE_MSG_ID;


/**
 * @struct AUDIO_TASK_MSG_STRU
 * @brief audio task message struct
 * 		consising of msg_id, decodehandle and param
 */
typedef struct {
	enumDECODE_MSG_ID msg_id;

    union 
    {
        DECODE_DEVICE_STRU * decodeHandle;
        int param;
    };
} AUDIO_TASK_MSG_STRU;


/**
 * @struct AUDIO_TASK_ENV_STRU
 * @brief audio task enviroment struct
 * 		consisting of output_buffer, taskhandle, queue,
 *		index, decodehandle, decode_flag, decode_exit_req
 */
typedef struct {
	#ifndef OUTPUT_USED_FIFO
	U32 ouput_buffer[2][AUDIO_OUTPUT_BUF_SIZE/4];
	#endif

	void * pxCreatedTask;
	void * queue;
	U32 index;
	DECODE_DEVICE_STRU *decodeHandle;
	volatile U32 decoding_flag;
	volatile U32 decoding_exit_req;
	volatile U32 dec_pause_flag;
	U32 dec_frame_in_samples;// define by each decoder if value equal to 0
} AUDIO_TASK_ENV_STRU;



/*
 *************************************************************************************
 * FUNCTION DECLARATION
 ****************************************************************************************
 */
extern AUDIO_TASK_ENV_STRU audio_task_env;


void audio_task_init(void);

void decode_set_msgSend ( DECODE_DEVICE_STRU *decodeHandle );
void decode_set_msgSend_from_isr(DECODE_DEVICE_STRU *decodeHandle);

void decode_init_msgSend (void);
void decode_init_msgSend_from_isr (void);

void decode_start_msgSend ( void );
void decode_start_msgSend_form_isr ( void );

void decode_start_w_init_msgSend ( void );
void decode_start_w_init_from_isr(void);

void decode_exit_msgSend ( void );
void decode_exit_wait_forever_msgSend ( void );

int decode_exit_req_set ( void );
int decode_exit_req_get ( void );

int decode_type_get ( void );
int decode_type_is_set( void );

int decode_pause_status_get ( void );
int decode_pause_enable ( void );
int decode_pause_disable ( void );

int decode_frame_samples_set ( U16 samples );
int decode_frame_samples_get ( void );

#endif //__AUDIO_TASK_H__
