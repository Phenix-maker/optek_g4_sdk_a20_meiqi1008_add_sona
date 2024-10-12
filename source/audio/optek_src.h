#ifndef __OPTEK_SRC_H__
#define __OPTEK_SRC_H__

#include "audio_codec.h"


#define COEF_20K_LOWPASS_CNT    2


//The macro below for 16-bit output
#define AUDIO_OUT_BUF_SIZE    (1024)



#if defined MI2S_OUTPUT_2CHANEL
extern U32 optek_src_buf[(AUDIO_OUT_BUF_SIZE/4)*2*2];
#elif defined MI2S_OUTPUT_4CHANEL
extern U32 optek_src_buf[(AUDIO_OUT_BUF_SIZE/4)*4*2];
#elif defined MI2S_OUTPUT_6CHANEL
extern U32 optek_src_buf[(AUDIO_OUT_BUF_SIZE/4)*6*2];
#elif defined MI2S_OUTPUT_8CHANEL
extern U32 optek_src_buf[(AUDIO_OUT_BUF_SIZE/4)*8*2];
#else
extern U32 optek_src_buf[(AUDIO_OUT_BUF_SIZE/4)*2*2];
#endif



extern const int COEF_20K_LOWPASS_44P1K_SAMPLERATE_TBL[6*COEF_20K_LOWPASS_CNT];
extern const int COEF_10K_LOWPASS_44P1K_SAMPLERATE_TBL[6*COEF_20K_LOWPASS_CNT];
extern const int COEF_5K_LOWPASS_44P1K_SAMPLERATE_TBL[6*COEF_20K_LOWPASS_CNT];

extern const int COEF_20K_LOWPASS_48K_SAMPLERATE_TBL[6*COEF_20K_LOWPASS_CNT];
extern const int COEF_10K_LOWPASS_48K_SAMPLERATE_TBL[6*COEF_20K_LOWPASS_CNT];
extern const int COEF_5K_LOWPASS_48K_SAMPLERATE_TBL[6*COEF_20K_LOWPASS_CNT];

extern void *app_20k_lowpass_filter;


void optek_src_init (void);
void optek_output(U16 *buf, U32 len);

U8 optek_src_output(U16 *buf, U32 byte_len, U8 out_channel);

#endif //__OPTEK_SRC_H__
