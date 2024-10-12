#ifndef _AUX_DEC_H_
#define _AUX_DEC_H_

#include "audio_input_receive_task.h"

//for decoding the size of one frame
//dec_frame_size
//#define AUX_PCM_BLOCK_SIZE        384//AD_PCM_BLOCK_SIZE
#define AUX_PCM_BLOCK_SIZE        (256*2*2)//(128*2*2)//(240*4)//960//AD_PCM_BLOCK_SIZE
#endif//_AUX_DEC_H_
