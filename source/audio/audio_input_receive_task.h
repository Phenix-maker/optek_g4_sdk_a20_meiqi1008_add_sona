#ifndef _AUDIO_INPUT_RECEIVE_TASK_H_
#define _AUDIO_INPUT_RECEIVE_TASK_H_
#include "sdk_config.h"
#include "audio_codec.h"
#include "audio_task.h"


typedef enum {
	AUDIO_INPUT_RX_SOURCE_HDMI = 1,
	AUDIO_INPUT_RX_SOURCE_OPTICAL,
	AUDIO_INPUT_RX_SOURCE_COAXIAL,
	AUDIO_INPUT_RX_SOURCE_ARC,
	AUDIO_INPUT_RX_SOURCE_AUX
} enAUDIO_INPUT_RX_SOURCE_SELECT;


//2->16bit output, 4->32/24bit output
#if (IIS_INPUT_CHANNEL_BIT_NUM == 24)
#define AUDIO_INPUT_CHANNEL_BYTE_SIZE    4
#else
#define AUDIO_INPUT_CHANNEL_BYTE_SIZE    2
#endif

#ifdef I2S_TDM_DATA_WIDTH_24_BIT
#define AUDIO_INPUT_CHANNEL          4
#else
#if (defined ADC_DAC_DATA_WIDTH_24_BIT && (defined AUX_INPUT_FROM_IIS0 || defined AUX_INPUT_FROM_IIS1))
#define AUDIO_INPUT_CHANNEL          4
#else
#define AUDIO_INPUT_CHANNEL          2
#endif
#endif

//#define AUDIO_INPUT_BUF_SIZE         ((96*AUDIO_INPUT_CHANNEL_BYTE_SIZE*AUDIO_INPUT_CHANNEL*AUDIO_IN_STREAM_SAMPLE_SPEED)/4)
#define AUDIO_INPUT_BUF_SIZE         (192*AUDIO_INPUT_CHANNEL_BYTE_SIZE*AUDIO_INPUT_CHANNEL*1)
#define SPDIF_FRAME_SIZE             (AUDIO_INPUT_BUF_SIZE) //a block has 192 frames, audio samples (SPDIF_FRAME_SIZE/2 bytes)

#define EX_AUDIO_STREAM_THRESHOLD    (SPDIF_FRAME_SIZE*4) //4 spdif frames


//for decoding the size of a frame
//192*4 bytes: 4ms at 48kHz or 1ms at 192kHz
//It is best to ensure that the width of output interval is 4 ms.
//the macro of AD_PCM_BLOCK_SIZE is the length of 2 channels.
//#define AD_PCM_BLOCK_SIZE            ((192*AUDIO_INPUT_CHANNEL_BYTE_SIZE*2*AUDIO_IN_STREAM_SAMPLE_SPEED)/2/2/2/2)
#define AD_PCM_BLOCK_SIZE            (192*AUDIO_INPUT_CHANNEL_BYTE_SIZE*2)

typedef struct {
	TaskHandle_t pxCreatedTask;
	QueueHandle_t queue;
	U32 source;
	U32 *buf;
	U32 byte_length;
} AUDIO_INPUT_RX_TAST_MSG;


extern U32 audio_input_rx_buf[2][AUDIO_INPUT_BUF_SIZE/4];
extern U32 audio_rx_postProcessBuf[AD_PCM_BLOCK_SIZE/4];
extern AUDIO_INPUT_RX_TAST_MSG audio_input_rx_task_msg;
extern U8 audio_read_buf[2][AD_PCM_BLOCK_SIZE];


#endif //_AUDIO_INPUT_RECEIVE_TASK_H_
