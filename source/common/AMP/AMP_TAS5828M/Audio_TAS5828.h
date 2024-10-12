#ifndef _AUDIO_TAS5828M_H_
#define _AUDIO_TAS5828M_H_

#define TAS5828M_ADDRESS     	(0x61<<(1)) //1k  原来的是0xc0
#define TAS5828M_ADDRESS_1		(0X62<<(1)) //4.7K

#define DEVICE_CTRL_2         0x03
#define DIG_VOL_Register      0x4C

void audio_amp_tas5828m_init(void);
void audio_amp_tas5828m_open(void);
void audio_amp_tas5828m_set_channel(U8 channel);
void audio_amp_tas5828m_set_volume(U8 vol);
void audio_amp_tas5828m_set_bass_treble(U8 bass, U8 treble);
void audio_amp_tas5828m_Set_Eq_Mode(U8 mode);
void audio_amp_tas5828m_set_mute(U8 con);


#define AUDIO_AMP_DEV_TAS5828M	audio_amp_tas5828m_init, \
								audio_amp_tas5828m_open, \
								audio_amp_tas5828m_set_channel, \
								audio_amp_tas5828m_set_volume, \
								audio_amp_tas5828m_set_bass_treble, \
								audio_amp_tas5828m_Set_Eq_Mode, \
								audio_amp_tas5828m_set_mute,\

#endif