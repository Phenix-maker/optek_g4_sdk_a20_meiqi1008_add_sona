#ifndef __MP3_DECODE_H
#define __MPE_DECODE_H

#include "audio_input_receive_task.h"

//for decoding the size of one frame
//dec_frame_size
#define MP3_DECODE_BLOCK_SIZE        (0x4E4*AUDIO_INPUT_CHANNEL_BYTE_SIZE*2)

typedef struct {
    U8 *read_buffer[2];
    U8 *rx_postProcessBuf;
    U16 discard_frame_cnt;//discard same frame when decoding begains
    U8 first_frame;//for sync
    U8 index;
} MP3_DEC_ENV_STRU;

extern MP3_DEC_ENV_STRU mp3_dec_env;

#endif //__MPE_DECODE_H
