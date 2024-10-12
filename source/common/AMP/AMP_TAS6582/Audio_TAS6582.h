#ifndef _AUDIO_TAS6582_H__
#define _AUDIO_TAS6582_H__
#include "c_def.h"

#define TAS6582_ADDRESS     	(0xc0)//(0x61<<(1)) //1k  原来的是0xc0
// #define TAS6582_ADDRESS_1		(0X62<<(1)) //4.7K


void audio_amp_tas6582_class_H(U8 enable);

#define TAS6582_DEVICE_CTRL_1         0x03
#define TAS6582_DEVICE_CTRL_2         0x04

#define TAS6582_DIG_VOL_Register      0x4C

void audio_amp_tas6582_init(void);
void audio_amp_tas6582_open(void);
void audio_amp_tas6582_set_channel(U8 channel);
void audio_amp_tas6582_set_volume(U8 vol);
void audio_amp_tas6582_set_bass_treble(U8 bass, U8 treble);
void audio_amp_tas6582_Set_Eq_Mode(U8 mode);
void audio_amp_tas6582_set_mute(U8 con);


#define AUDIO_AMP_DEV_TAS6582	audio_amp_tas6582_init, \
								audio_amp_tas6582_open, \
								audio_amp_tas6582_set_channel, \
								audio_amp_tas6582_set_volume, \
								audio_amp_tas6582_set_bass_treble, \
								audio_amp_tas6582_Set_Eq_Mode, \
								audio_amp_tas6582_set_mute

#endif//_AUDIO_TAS6582_H__