#ifndef __XA_SBC_DEC_DEC_H__
#define __XA_SBC_DEC_DEC_H__

#include "audio_input_receive_task.h"

typedef struct {
    U8 *read_buffer[2];
    U8 *rx_postProcessBuf;
    U16 discard_frame_cnt;//discard same frame when decoding begains
    U8 first_frame;//for sync
    U8 index;
} SBC_DEC_ENV_STRU;

extern SBC_DEC_ENV_STRU sbc_dec_env;
//for decoding the size of one frame
//dec_frame_size
#define BT_SBC_BLOCK_SIZE        AD_PCM_BLOCK_SIZE

#endif /* __XA_SBC_DEC_DEC_H__ */
